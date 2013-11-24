
#include "tiki/resource/resourceloader.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/io/datastream.hpp"
#include "tiki/io/filesystem.hpp"
#include "tiki/resource/factorybase.hpp"
#include "tiki/resource/resource.hpp"
#include "tiki/resource/resourcefile.hpp"
#include "tiki/resource/resourcestorage.hpp"

namespace tiki
{
	struct ResourceLoaderContext
	{
		ResourceLoaderContext()
		{
			streamOwner	= false;
			pStream		= nullptr;

			pFactory	= nullptr;
			pResource	= nullptr;

			resourceCount	= 0u;
			pResourceHeader	= nullptr;
		}

		bool				streamOwner;
		DataStream*			pStream;

		FactoryBase*		pFactory;
		Resource*			pResource;

		uint				resourceCount;
		ResourceHeader*		pResourceHeader;

		ResourceId			resourceId;
		ResourceSectorData	sectionData;
	};

	struct ResourceLoaderInternalContext
	{
	};

	void ResourceLoader::create( FileSystem* pFileSystem, ResourceStorage* pStorage )
	{
		m_pFileSystem	= pFileSystem;
		m_pStorage		= pStorage;

		m_factories.create( MaxFactoryCount );
		m_bufferAllocator.create( InitializationDataBufferSize, 128u );
	}

	void ResourceLoader::dispose()
	{
		m_pFileSystem = nullptr;

		m_factories.dispose();
		m_bufferAllocator.dispose();
	}

	void ResourceLoader::registerFactory( FactoryBase& factory )
	{
		m_factories.set( factory.getType(), &factory );
	}

	void ResourceLoader::unregisterFactory( FactoryBase& factory )
	{
		m_factories.remove( factory.getType() );
	}

	ResourceLoaderResult ResourceLoader::loadResource( const Resource** ppTargetResource, const char* pFileName, crc32 resourceKey, fourcc resourceType )
	{
		TIKI_ASSERT( ppTargetResource != nullptr );
		TIKI_ASSERT( pFileName != nullptr );
		TIKI_ASSERT( resourceKey != InvalidCrc32 );

		if ( m_pFileSystem->exists( pFileName ) == false )
		{
			return ResourceLoaderResult_FileNotFound;
		}

		ResourceLoaderContext context;

		context.pStream = m_pFileSystem->open( pFileName, DataAccessMode_Read );
		if ( context.pStream == nullptr )
		{
			return ResourceLoaderResult_CouldNotAccessFile;
		}

		ResourceFileHeader fileHeader;
		if ( context.pStream->read( &fileHeader, sizeof( fileHeader ) ) != sizeof( fileHeader ) )
		{			
			return ResourceLoaderResult_WrongFileFormat;
		}

		if ( fileHeader.tikiFourcc != ResourceFileHeader::TikiMagicHostEndian || fileHeader.version != ResourceFileHeader::CurrentFormatVersion )
		{
			return ResourceLoaderResult_WrongFileFormat;
		}
		
		context.resourceCount = fileHeader.resourceCount;

		const uint resourceHeaderSize = sizeof( ResourceHeader ) * context.resourceCount;
		context.pResourceHeader = static_cast< ResourceHeader* >( m_bufferAllocator.allocate( resourceHeaderSize ) );
		
		if ( context.pResourceHeader == nullptr )
		{
			cancelOperation( context );
			return ResourceLoaderResult_OutOfMemory;
		}

		if ( context.pStream->read( context.pResourceHeader, resourceHeaderSize ) != resourceHeaderSize )
		{
			return ResourceLoaderResult_WrongFileFormat;
		}

		for (uint i = 0u; i < context.resourceCount; ++i)
		{
			const ResourceHeader& header = context.pResourceHeader[ i ];

			if ( header.key == resourceKey )
			{
				if ( header.type != resourceType )
				{
					return ResourceLoaderResult_WrongResourceType;
				}

				context.resourceId.key		= resourceKey;
				context.resourceId.fileName	= pFileName;

				ResourceLoaderResult result = createResource( context, header );
				if ( result != ResourceLoaderResult_Success )
				{
					cancelOperation( context );
				}

				return result;
			}
		}

		return ResourceLoaderResult_ResourceNotFound;
	}

	void ResourceLoader::unloadResource( const Resource* pResource, fourcc resourceType )
	{
		TIKI_ASSERT( m_pFileSystem != nullptr );

		TIKI_ASSERT( pResource != nullptr );

		const ResourceSectorData& sectorData = pResource->m_sectorData;
		for (uint i = 0u; i < sectorData.sectorCount; ++i)
		{
			memory::freeAlign( sectorData.ppSectorPointers[ i ] );
		}
		memory::freeAlign( sectorData.ppSectorPointers );

		//pFactory->destroyResource( const_cast< Resource* >( pResource ) );
	}

	FactoryBase* ResourceLoader::findFactory( fourcc resourceType ) const
	{
		FactoryBase* pFactory;
		if ( m_factories.findValue( &pFactory, resourceType ) )
		{
			return pFactory;
		}

		return nullptr;
	}

	ResourceLoaderResult ResourceLoader::createResource( ResourceLoaderContext& context, const ResourceHeader& header )
	{
		context.pFactory = findFactory( header.type );
		if ( context.pFactory == nullptr )
		{
			uint64 type = (uint64)header.type << 32u;
			TIKI_TRACE_ERROR( "No Factory found for Resource of type: %s\n", &type );
			return ResourceLoaderResult_CouldNotCreateResource;
		}

		context.pResource = context.pFactory->createResource();
		if ( context.pResource == nullptr )
		{
			return ResourceLoaderResult_CouldNotCreateResource;
		}

		const uint sectionHeaderSize = sizeof( SectionHeader ) * header.sectionCount;
		SectionHeader* pSectionHeaders = static_cast< SectionHeader*>( m_bufferAllocator.allocate( sectionHeaderSize ) );
		if ( pSectionHeaders == nullptr )
		{
			return ResourceLoaderResult_OutOfMemory;
		}

		context.pStream->setPosition( header.offsetInFile );
		context.pStream->read( pSectionHeaders, sectionHeaderSize );

		context.sectionData.sectorCount			= header.sectionCount;
		context.sectionData.ppSectorPointers	= static_cast< void** >( memory::allocAlign( sizeof( void* ) * header.sectionCount ) );

		void* pInitData = nullptr;
		for (uint i = 0u; i < header.sectionCount; ++i)
		{
			const SectionHeader& sectionHeader = pSectionHeaders[ i ];

			void* pSectionData = memory::allocAlign( sectionHeader.sizeInBytes, 1u << sectionHeader.alignment );

			context.pStream->setPosition( sectionHeader.offsetInFile );
			context.pStream->read( pSectionData, sectionHeader.sizeInBytes );

			context.sectionData.ppSectorPointers[ i ] = pSectionData;

			if ( resource::getSectionAllocatorType( sectionHeader.allocatorType_allocatorId ) == AllocatorType_InitializaionMemory )
			{
				TIKI_ASSERT( pInitData == nullptr );
				pInitData = pSectionData;
			}
		}

		for (uint i = 0u; i < header.sectionCount; ++i)
		{
			const SectionHeader& sectionHeader = pSectionHeaders[ i ];
			
			const uint referenceItemSize = sizeof( ReferenceItem ) * sectionHeader.referenceCount;
			ReferenceItem* pReferenceItems = static_cast< ReferenceItem* >( m_bufferAllocator.allocate( referenceItemSize ) );
			if ( pReferenceItems == nullptr )
			{
				return ResourceLoaderResult_OutOfMemory;
			}
			context.pStream->read( pReferenceItems, referenceItemSize );

			for (uint j = 0u; j < sectionHeader.referenceCount; ++j)
			{
				const ReferenceItem& item = pReferenceItems[ j ];

				void* pPointer = nullptr;
				switch ( item.type )
				{
				case ReferenceType_Pointer:
					break;

				case ReferenceType_String:
					break;

				case ReferenceType_ResourceLink:
					break;

				default:
					return ResourceLoaderResult_WrongFileFormat;
				}

				uint64* pTargetSectionData = addPtrCast< uint64 >( context.sectionData.ppSectorPointers[ item.targetId ], item.offsetInSection );
				*pTargetSectionData = (uint64)pPointer;
			}

			m_bufferAllocator.free( pReferenceItems );
		}

		m_bufferAllocator.free( pSectionHeaders );

		m_pStorage->allocateResource( context.pResource, context.resourceId, context.sectionData );
		context.pFactory->initializeResource( context.pResource, pInitData );

		return ResourceLoaderResult_Success;
	}
	
	ResourceLoaderResult ResourceLoader::initializeResource( ResourceLoaderContext& context )
	{
		return ResourceLoaderResult_Success;
	}

	void ResourceLoader::cancelOperation( ResourceLoaderContext& context )
	{
		TIKI_ASSERT( context.pFactory != nullptr );

		if ( context.pResource != nullptr )
		{
			context.pFactory->destroyResource( context.pResource );
			context.pResource = nullptr;
		}

		if ( context.pStream != nullptr )
		{
			context.pStream->close();
			context.pStream = nullptr;
		}

		context.pFactory = nullptr;
		m_bufferAllocator.clear();
	}
}