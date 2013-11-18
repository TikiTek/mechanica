
#include "tiki/resource/resourceloader.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/io/datastream.hpp"
#include "tiki/io/filesystem.hpp"
#include "tiki/resource/factorybase.hpp"
#include "tiki/resource/resource.hpp"
#include "tiki/resource/resourcefile.hpp"

namespace tiki
{
	void ResourceLoader::create( FileSystem* pFileSystem )
	{
		m_pFileSystem = pFileSystem;

		m_bufferAllocator.create( InitializationDataBufferSize, 128u );
	}

	void ResourceLoader::dispose()
	{
		m_pFileSystem = nullptr;

		m_bufferAllocator.dispose();
	}

	ResourceLoaderResult ResourceLoader::loadResource( const Resource** ppTargetResource, uint targetCapacity, const char* pFileName, crc32 resourceKey, const FactoryBase* pFactory )
	{
		TIKI_ASSERT( m_pFileSystem != nullptr );

		TIKI_ASSERT( pFactory != nullptr );
		TIKI_ASSERT( pFileName != nullptr );
		TIKI_ASSERT( ppTargetResource != nullptr );

		if ( m_pFileSystem->exists( pFileName ) == false )
		{
			return ResourceLoaderResult_FileNotFound;
		}

		Context context;

		context.pStream = m_pFileSystem->open( pFileName, DataAccessMode_Read );
		if ( context.pStream == nullptr )
		{
			return ResourceLoaderResult_CouldNotAccessFile;
		}

		context.pResource = pFactory->createResource();
		if ( context.pResource == nullptr )
		{			
			return ResourceLoaderResult_CouldNotCreateResource;
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
		TIKI_ASSERT( context.pResourceHeader != nullptr );

		if ( context.pStream->read( context.pResourceHeader, resourceHeaderSize ) != resourceHeaderSize )
		{
			return ResourceLoaderResult_WrongFileFormat;
		}

		ResourceSectorData sectionData;

		ResourceId resourceId;
		resourceId.crcName	= resourceKey;
		resourceId.fileName	= pFileName;
		
		context.pResource->initialize( resourceId, sectionData );
	}

	void ResourceLoader::unloadResource( const Resource* pResource, const FactoryBase* pFactory )
	{
		TIKI_ASSERT( m_pFileSystem != nullptr );

		TIKI_ASSERT( pResource != nullptr );
		TIKI_ASSERT( pFactory != nullptr );

		const ResourceSectorData& sectorData = pResource->m_sectorData;
		for (uint i = 0u; i < sectorData.sectorCount; ++i)
		{
			memory::freeAlign( sectorData.ppSectorPointers[ i ] );
		}
		memory::freeAlign( sectorData.ppSectorPointers );

		pFactory->destroyResource( const_cast< Resource* >( pResource ) );
	}

	void ResourceLoader::cancelOperation( Context& context ) const
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
		// todo free context.pResourceHeader
	}

}