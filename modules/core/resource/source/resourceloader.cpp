
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
			streamOwner			= false;
			pStream				= nullptr;

			pFactory			= nullptr;
			pResource			= nullptr;

			resourceCount		= 0u;
			pResourceHeaders	= nullptr;
		}

		fourcc					resourceType;

		bool					streamOwner;
		DataStream*				pStream;
		
		const FactoryContext*	pFactory;
		Resource*				pResource;

		uint					resourceCount;
		ResourceHeader*			pResourceHeaders;

		ResourceId				resourceId;
		ResourceSectorData		sectionData;
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

	void ResourceLoader::registerResourceType( fourcc type, const FactoryContext& factoryContext )
	{
		m_factories.set( type, &factoryContext );
	}

	void ResourceLoader::unregisterResourceType( fourcc type )
	{
		m_factories.remove( type );
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
		context.resourceType	= resourceType;
		context.streamOwner		= true;

		context.pStream = m_pFileSystem->open( pFileName, DataAccessMode_Read );
		if ( context.pStream == nullptr )
		{
			cancelOperation( context );
			return ResourceLoaderResult_CouldNotAccessFile;
		}

		ResourceFileHeader fileHeader;
		if ( context.pStream->read( &fileHeader, sizeof( fileHeader ) ) != sizeof( fileHeader ) )
		{			
			cancelOperation( context );
			return ResourceLoaderResult_WrongFileFormat;
		}

		if ( fileHeader.tikiFourcc != ResourceFileHeader::TikiMagicHostEndian || fileHeader.version != ResourceFileHeader::CurrentFormatVersion )
		{
			cancelOperation( context );
			return ResourceLoaderResult_WrongFileFormat;
		}
		
		context.resourceCount = fileHeader.resourceCount;

		const uint resourceHeaderSize = sizeof( ResourceHeader ) * context.resourceCount;
		context.pResourceHeaders = static_cast< ResourceHeader* >( m_bufferAllocator.allocate( resourceHeaderSize ) );
		
		if ( context.pResourceHeaders == nullptr )
		{
			cancelOperation( context );
			return ResourceLoaderResult_OutOfMemory;
		}

		if ( context.pStream->read( context.pResourceHeaders, resourceHeaderSize ) != resourceHeaderSize )
		{
			cancelOperation( context );
			return ResourceLoaderResult_WrongFileFormat;
		}

		for (uint i = 0u; i < context.resourceCount; ++i)
		{
			const ResourceHeader& header = context.pResourceHeaders[ i ];

			if ( header.key == resourceKey )
			{
				if ( header.type != resourceType )
				{
					cancelOperation( context );
					return ResourceLoaderResult_WrongResourceType;
				}

				context.resourceId.key		= resourceKey;
				context.resourceId.fileName	= pFileName;

				ResourceLoaderResult result = createResource( context, header );
				if ( result != ResourceLoaderResult_Success )
				{
					cancelOperation( context );

					*ppTargetResource = nullptr;
				}
				else
				{
					context.pStream->close();
					context.pStream = nullptr;

					*ppTargetResource = context.pResource;
				}

				return result;
			}
		}

		cancelOperation( context );
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

		const FactoryContext* pFactory = findFactory( resourceType );		
		if ( pFactory == nullptr )
		{
			return;
		}

		Resource* pNonConstResource = const_cast< Resource* >( pResource );
		pNonConstResource->dispose( *pFactory );
		pFactory->pDisposeResource( pNonConstResource );
	}

	const FactoryContext* ResourceLoader::findFactory( fourcc resourceType ) const
	{
		const FactoryContext* pFactoryContext;
		if ( m_factories.findValue( &pFactoryContext, resourceType ) )
		{
			return pFactoryContext;
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

		context.pResource = context.pFactory->pCreateResource();
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

		uint initDataSectionIndex = TIKI_SIZE_T_MAX;
		for (uint i = 0u; i < header.sectionCount; ++i)
		{
			const SectionHeader& sectionHeader = pSectionHeaders[ i ];

			void* pSectionData = memory::allocAlign( sectionHeader.sizeInBytes, 1u << sectionHeader.alignment );

			context.pStream->setPosition( sectionHeader.offsetInFile );
			context.pStream->read( pSectionData, sectionHeader.sizeInBytes );

			context.sectionData.ppSectorPointers[ i ] = pSectionData;

			if ( resource::getSectionAllocatorType( sectionHeader.allocatorType_allocatorId ) == AllocatorType_InitializaionMemory )
			{
				TIKI_ASSERT( initDataSectionIndex == TIKI_SIZE_T_MAX );
				initDataSectionIndex = i;
			}
		}

		if ( initDataSectionIndex == TIKI_SIZE_T_MAX )
		{
			return ResourceLoaderResult_CouldNotInitialize;
		}
		
		for (uint i = 0u; i < header.sectionCount; ++i)
		{
			const SectionHeader& sectionHeader = pSectionHeaders[ i ];

			if ( sectionHeader.referenceCount == 0u )
			{
				continue;
			}
			
			const uint referenceItemSize = sizeof( ReferenceItem ) * sectionHeader.referenceCount;
			ReferenceItem* pReferenceItems = static_cast< ReferenceItem* >( m_bufferAllocator.allocate( referenceItemSize ) );
			if ( pReferenceItems == nullptr )
			{
				return ResourceLoaderResult_OutOfMemory;
			}
			context.pStream->setPosition( sectionHeader.offsetInFile + sectionHeader.sizeInBytes );
			context.pStream->read( pReferenceItems, referenceItemSize );

			for (uint j = 0u; j < sectionHeader.referenceCount; ++j)
			{
				const ReferenceItem& item = pReferenceItems[ j ];

				void* pPointer = nullptr;
				switch ( item.type )
				{
				case ReferenceType_Pointer:
					pPointer = addPtr( context.sectionData.ppSectorPointers[ item.targetId ], item.offsetInTargetSection );
					break;

				case ReferenceType_String:
					break;

				case ReferenceType_ResourceLink:
					break;

				default:
					return ResourceLoaderResult_WrongFileFormat;
				}

				uint64* pTargetSectionData = addPtrCast< uint64 >( context.sectionData.ppSectorPointers[ i ], item.offsetInSection );
				*pTargetSectionData = (uint64)pPointer;
			}

			m_bufferAllocator.free( pReferenceItems );
		}

		m_pStorage->allocateResource( context.pResource, context.resourceId );
		
		ResourceInitData initData;
		initData.pData	= context.sectionData.ppSectorPointers[ initDataSectionIndex ];
		initData.size	= pSectionHeaders[ initDataSectionIndex ].sizeInBytes;

		const ResourceLoaderResult result = initializeResource( context, initData );

		m_bufferAllocator.free( pSectionHeaders );

		return result;
	}
	
	ResourceLoaderResult ResourceLoader::initializeResource( ResourceLoaderContext& context, const ResourceInitData& initData )
	{		
		if ( context.pResource->create( context.resourceId, context.sectionData, initData, *context.pFactory ) == false )
		{
			return ResourceLoaderResult_CouldNotInitialize;
		}

		return ResourceLoaderResult_Success;
	}

	void ResourceLoader::cancelOperation( ResourceLoaderContext& context )
	{
		if ( context.pResource != nullptr )
		{
			TIKI_ASSERT( context.pFactory != nullptr );
			unloadResource( context.pResource, context.resourceType );
			context.pResource = nullptr;
		}

		if ( context.pStream != nullptr && context.streamOwner )
		{
			context.pStream->close();
			context.pStream = nullptr;

			m_bufferAllocator.clear();
		}

		context.pFactory = nullptr;
	}
}