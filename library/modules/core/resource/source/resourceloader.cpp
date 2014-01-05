
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

	ResourceLoaderResult ResourceLoader::loadResource( Resource** ppTargetResource, const char* pFileName, crc32 resourceKey, fourcc resourceType )
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

		if ( sectorData.stringCount != 0u )
		{
			memory::freeAlign( sectorData.ppStringPointers[ 0u ] );
		}

		for (uint i = 0u; i < sectorData.linkCount; ++i)
		{
			Resource* pResource = sectorData.ppLinkedResources[ i ];

			if ( pResource != nullptr )
			{
				unloadResource( pResource, pResource->getType() );
			}		
		} 

		memory::freeAlign( sectorData.ppLinkedResources );

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

		const uint pointerCount	= header.sectionCount + header.stringCount + header.linkCount;
		void** ppPointers		= static_cast< void** >( memory::allocAlign( sizeof( void* ) * pointerCount ) );
		context.sectionData.sectorCount			= header.sectionCount;
		context.sectionData.stringCount			= header.stringCount;
		context.sectionData.linkCount			= header.linkCount;
		context.sectionData.ppLinkedResources	= reinterpret_cast< Resource** >( ppPointers );
		context.sectionData.ppStringPointers	= reinterpret_cast< char** >( ppPointers + header.linkCount );
		context.sectionData.ppSectorPointers	= ppPointers + header.linkCount + header.stringCount;

		context.pStream->setPosition( header.offsetInFile );
		const uint sectionHeaderSize = sizeof( SectionHeader ) * header.sectionCount;
		const uint stringItemSize = sizeof( StringItem ) * header.stringCount;
		const uint resourceLinkSize = sizeof( ResourceLinkItem ) * header.linkCount;

		SectionHeader* pSectionHeaders = static_cast< SectionHeader*>( m_bufferAllocator.allocate( sectionHeaderSize ) );
		if ( pSectionHeaders == nullptr )
		{
			return ResourceLoaderResult_OutOfMemory;
		}
		context.pStream->read( pSectionHeaders, sectionHeaderSize );

		StringItem* pStringItems = static_cast< StringItem* >( m_bufferAllocator.allocate( stringItemSize ) );
		if ( pStringItems == nullptr )
		{
			return ResourceLoaderResult_OutOfMemory;
		}
		context.pStream->read( pStringItems, stringItemSize );

		ResourceLinkItem* pResourceLinks = static_cast< ResourceLinkItem* >( m_bufferAllocator.allocate( resourceLinkSize ) );
		if ( pResourceLinks == nullptr )
		{
			return ResourceLoaderResult_OutOfMemory;
		}
		context.pStream->read( pResourceLinks, resourceLinkSize );

		// load section data
		uint initDataSectionIndex = TIKI_SIZE_T_MAX;
		for (uint i = 0u; i < header.sectionCount; ++i)
		{
			const SectionHeader& sectionHeader = pSectionHeaders[ i ];

			void* pSectionData = memory::allocAlign( sectionHeader.sizeInBytes, 1u << sectionHeader.alignment );

			context.pStream->setPosition( header.offsetInFile + sectionHeader.offsetInResource );
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

		// load strings
		if ( header.stringCount > 0u )
		{
			char* pBlock = static_cast< char* >( memory::allocAlign( header.stringSizeInBytes ) );
			if ( pBlock == nullptr )
			{
				return ResourceLoaderResult_OutOfMemory;
			}
			context.pStream->setPosition( header.offsetInFile + header.stringOffsetInResource );
			context.pStream->read( pBlock, header.stringSizeInBytes );

			for (uint i = 0u; i < header.stringCount; ++i)
			{
				const StringItem& stringItem = pStringItems[ i ];
				TIKI_ASSERT( i != 0u || stringItem.offsetInBlock == 0u );

				context.sectionData.ppStringPointers[ i ] = pBlock + stringItem.offsetInBlock;
			} 
		}

		// load resource links
		for (uint i = 0u; i < header.linkCount; ++i)
		{
			const ResourceLinkItem& link = pResourceLinks[ i ];

			// TODO: implement - resource in same file
			//if ( link.fileKey == 0u )
			//{
			//	continue;
			//}

			ResourceLoaderResult result = loadResource(
				&context.sectionData.ppLinkedResources[ i ],
				context.sectionData.ppStringPointers[ link.fileKey ], //link.fileKey,
				link.resourceKey,
				link.resourceType
			);

			if ( result != ResourceLoaderResult_Success )
			{
				context.sectionData.ppLinkedResources[ i ] = nullptr;
			}
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
			context.pStream->setPosition( header.offsetInFile + sectionHeader.offsetInResource + sectionHeader.sizeInBytes );
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
					pPointer = context.sectionData.ppStringPointers[ item.targetId ];
					break;

				case ReferenceType_ResourceLink:
					pPointer = context.sectionData.ppLinkedResources[ item.targetId ];
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