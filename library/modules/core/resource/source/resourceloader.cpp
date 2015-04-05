
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
		crc32					crcFileName;

		bool					streamOwner;
		DataStream*				pStream;
		
		ResourceFileHeader		fileHeader;
		uint					resourceCount;
		ResourceHeader*			pResourceHeaders;		
		uint					resourceHeaderIndex;

		const FactoryContext*	pFactory;
		Resource*				pResource;

		ResourceId				resourceId;
		ResourceSectionData		sectionData;
		ResourceInitData		initializationData;
	};

	struct ResourceLoaderInternalContext
	{
	};

	void ResourceLoader::create( FileSystem* pFileSystem, ResourceStorage* pStorage )
	{
		m_pFileSystem	= pFileSystem;
		m_pStorage		= pStorage;

		reloadResourceMapping();

		m_factories.create( MaxFactoryCount );
		m_bufferAllocator.create( InitializationDataBufferSize, 128u );
	}

	void ResourceLoader::dispose()
	{
		m_pFileSystem = nullptr;

		m_nameMapper.dispose();
		m_factories.dispose();
		m_bufferAllocator.dispose();
	}

	void ResourceLoader::reloadResourceMapping()
	{
		m_nameMapper.dispose();

		if ( m_pFileSystem->exists( "resourcenamemap.rnm" ) )
		{
			DataStream* pStream = m_pFileSystem->open( "resourcenamemap.rnm", DataAccessMode_Read );
			if ( pStream != nullptr )
			{
				Array< uint8 > binaryData;
				binaryData.create( (uint)pStream->getLength() );
				pStream->read( binaryData.getBegin(), binaryData.getCount() );
				pStream->close();

				m_nameMapper.create( binaryData.getBegin() );

				binaryData.dispose();
			}
		}
	}

	void ResourceLoader::registerResourceType( fourcc type, const FactoryContext& factoryContext )
	{
		m_factories.set( type, &factoryContext );
	}

	void ResourceLoader::unregisterResourceType( fourcc type )
	{
		m_factories.remove( type );
	}

	ResourceLoaderResult ResourceLoader::loadResource( Resource** ppTargetResource, crc32 crcFileName, crc32 resourceKey, fourcc resourceType )
	{
		TIKI_ASSERT( ppTargetResource != nullptr );
		TIKI_ASSERT( resourceKey != TIKI_INVALID_CRC32 );

		ResourceLoaderContext context;
		ResourceLoaderResult result = initializeLoaderContext( context, crcFileName, resourceKey, resourceType );
		if ( result != ResourceLoaderResult_Success )
		{
			cancelOperation( context );
			return result;
		}

		result = createResource( context );
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

	void ResourceLoader::unloadResource( const Resource* pResource, fourcc resourceType )
	{
		TIKI_ASSERT( m_pFileSystem != nullptr );
		TIKI_ASSERT( pResource != nullptr );

		Resource* pNonConstResource = const_cast< Resource* >( pResource );
		disposeResource( pNonConstResource, resourceType, true );
	}

	ResourceLoaderResult ResourceLoader::reloadResource( Resource* pResource, crc32 crcFileName, crc32 resourceKey, fourcc resourceType )
	{
		TIKI_ASSERT( m_pFileSystem != nullptr );
		TIKI_ASSERT( pResource != nullptr );

		ResourceLoaderContext context;
		ResourceLoaderResult result = initializeLoaderContext( context, crcFileName, resourceKey, resourceType );
		if ( result != ResourceLoaderResult_Success )
		{
			cancelOperation( context );
			return result;
		}

		result = loadResourceData( context );
		if ( result != ResourceLoaderResult_Success )
		{
			cancelOperation( context );
			return result;
		}

		disposeResource( pResource, resourceType, false );

		context.pResource = pResource;
		result = initializeResource( context );
		if ( result != ResourceLoaderResult_Success )
		{
			cancelOperation( context );
		}

		context.pStream->close();
		context.pStream = nullptr;

		return result;
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
	
	ResourceLoaderResult ResourceLoader::initializeLoaderContext( ResourceLoaderContext& context, crc32 crcFileName, crc32 resourceKey, fourcc resourceType )
	{
		const char* pFileName = m_nameMapper.getResourceName( crcFileName );
		if ( pFileName == nullptr || m_pFileSystem->exists( pFileName ) == false )
		{
			return ResourceLoaderResult_FileNotFound;
		}

		context.resourceType		= resourceType;
		context.streamOwner			= true;
		context.crcFileName			= crcFileName;
		context.resourceId.key		= resourceKey;
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		context.resourceId.fileName	= pFileName;
#endif

		context.pFactory = findFactory( resourceType );
		if ( context.pFactory == nullptr )
		{
			uint64 type = (uint64)resourceType << 32u;
			TIKI_TRACE_ERROR( "No Factory found for Resource of type: %s\n", &type );
			return ResourceLoaderResult_CouldNotCreateResource;
		}

		context.pStream = m_pFileSystem->open( pFileName, DataAccessMode_Read );
		if ( context.pStream == nullptr )
		{
			return ResourceLoaderResult_CouldNotAccessFile;
		}

		if ( context.pStream->read( &context.fileHeader, sizeof( context.fileHeader ) ) != sizeof( context.fileHeader ) )
		{
			return ResourceLoaderResult_WrongFileFormat;
		}

		if ( context.fileHeader.tikiFourcc != ResourceFileHeader::TikiMagicHostEndian || context.fileHeader.version != ResourceFileHeader::CurrentFormatVersion )
		{
			return ResourceLoaderResult_WrongFileFormat;
		}

		context.resourceCount = context.fileHeader.resourceCount;

		const uint resourceHeaderSize = sizeof( ResourceHeader ) * context.resourceCount;
		context.pResourceHeaders = static_cast< ResourceHeader* >( m_bufferAllocator.allocate( resourceHeaderSize ) );

		if ( context.pResourceHeaders == nullptr )
		{
			return ResourceLoaderResult_OutOfMemory;
		}

		if ( context.pStream->read( context.pResourceHeaders, resourceHeaderSize ) != resourceHeaderSize )
		{
			return ResourceLoaderResult_WrongFileFormat;
		}

		for (uint i = 0u; i < context.resourceCount; ++i)
		{
			const ResourceHeader& header = context.pResourceHeaders[ i ];

			if ( header.key == resourceKey )
			{				
				context.resourceHeaderIndex = i;
				return ResourceLoaderResult_Success;
			}
		}

		return ResourceLoaderResult_ResourceNotFound;
	}

	ResourceLoaderResult ResourceLoader::createResource( ResourceLoaderContext& context )
	{
		const ResourceHeader& header = context.pResourceHeaders[ context.resourceHeaderIndex ];

		if ( header.type != context.resourceType )
		{
			return ResourceLoaderResult_WrongResourceType;
		}

		context.pResource = context.pFactory->pCreateResource();
		if ( context.pResource == nullptr )
		{
			return ResourceLoaderResult_CouldNotCreateResource;
		}

		m_pStorage->allocateResource( context.pResource, context.resourceId );

		ResourceLoaderResult result = loadResourceData( context );
		if ( result != ResourceLoaderResult_Success )
		{
			m_pStorage->freeReferenceFromResource( context.pResource );
			return result;
		}

		result = initializeResource( context );
		if ( result != ResourceLoaderResult_Success )
		{
			m_pStorage->freeReferenceFromResource( context.pResource );
			return result;
		}

		return result;
	}
	
	ResourceLoaderResult ResourceLoader::loadResourceData( ResourceLoaderContext& context )
	{
		const ResourceHeader& header = context.pResourceHeaders[ context.resourceHeaderIndex ];

		const uint pointerCount	= header.sectionCount + header.stringCount + header.linkCount;
		void** ppPointers		= static_cast< void** >( TIKI_MEMORY_ALLOC( sizeof( void* ) * pointerCount ) );
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

			void* pSectionData = TIKI_MEMORY_ALLOC_ALIGNED( sectionHeader.sizeInBytes, 1u << sectionHeader.alignment );

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
			char* pBlock = static_cast< char* >( TIKI_MEMORY_ALLOC( header.stringSizeInBytes ) );
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

			ResourceLoaderResult result = ResourceLoaderResult_UnknownError;
			if ( link.fileKey == context.crcFileName )
			{
				// todo
			}
			else
			{
				result = loadResource(
					&context.sectionData.ppLinkedResources[ i ],
					link.fileKey,
					link.resourceKey,
					link.resourceType
					);
			}

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

		context.initializationData.pData	= context.sectionData.ppSectorPointers[ initDataSectionIndex ];
		context.initializationData.size		= pSectionHeaders[ initDataSectionIndex ].sizeInBytes;

		return ResourceLoaderResult_Success;
	}

	ResourceLoaderResult ResourceLoader::initializeResource( ResourceLoaderContext& context )
	{		
		if ( context.pResource->create( context.resourceId, context.sectionData, context.initializationData, *context.pFactory ) == false )
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

	void ResourceLoader::disposeResource( Resource* pResource, fourcc resourceType, bool freeResourceObject )
	{
		const ResourceSectionData&  sectionData = pResource->m_sectionData;

		for (uint i = 0u; i < sectionData.sectorCount; ++i)
		{
			TIKI_MEMORY_FREE( sectionData.ppSectorPointers[ i ] );
		}

		if ( sectionData.stringCount != 0u )
		{
			TIKI_MEMORY_FREE( sectionData.ppStringPointers[ 0u ] );
		}

		for (uint i = 0u; i < sectionData.linkCount; ++i)
		{
			Resource* pLinkResource = sectionData.ppLinkedResources[ i ];

			if ( pLinkResource != nullptr )
			{
				unloadResource( pLinkResource, pLinkResource->getType() );
			}		
		} 

		TIKI_MEMORY_FREE( sectionData.ppLinkedResources );

		const FactoryContext* pFactory = findFactory( resourceType );		
		if ( pFactory == nullptr )
		{
			return;
		}

		pResource->dispose( *pFactory );

		if ( freeResourceObject )
		{
			pFactory->pDisposeResource( pResource );
		}
	}
}