
#include "tiki/resource/resourceloader.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/io/datastream.hpp"
#include "tiki/io/filesystem.hpp"
#include "tiki/resource/factorybase.hpp"
#include "tiki/resource/resource.hpp"
#include "tiki/resource/resourcefile.hpp"

namespace tiki
{
	struct ResourceLoaderContext
	{
		ResourceLoaderContext()
		{
			pStream		= nullptr;
			pFactory	= nullptr;

			pResource	= nullptr;

			resourceCount	= 0u;
			pResourceHeader	= nullptr;
		}

		DataStream*		pStream;
		FactoryBase*	pFactory;

		Resource*		pResource;

		uint			resourceCount;
		ResourceHeader*	pResourceHeader;
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

	ResourceLoaderResult ResourceLoader::loadResource( const Resource** ppTargetResource, const char* pFileName, crc32 resourceKey )
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

		//context.pResource = pFactory->createResource();
		//if ( context.pResource == nullptr )
		//{			
		//	return ResourceLoaderResult_CouldNotCreateResource;
		//}

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
				return loadResource( context, i );
			}
		} 

		return ResourceLoaderResult_FileNotFound;

		ResourceSectorData sectionData;

		ResourceId resourceId;
		resourceId.key	= resourceKey;
		resourceId.fileName	= pFileName;
		
		context.pResource->initialize( resourceId, sectionData );
	}

	void ResourceLoader::unloadResource( const Resource* pResource )
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

	ResourceLoaderResult ResourceLoader::loadResource( ResourceLoaderContext& context, uint resourceIndex )
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