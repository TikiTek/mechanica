
#include "tiki/resource/resourcemanager.hpp"

#include "tiki/base/file.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/iopath.hpp"
#include "tiki/resource/factorybase.hpp"
#include "tiki/resource/resource.hpp"

namespace tiki
{
	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
	}

	bool ResourceManager::create( const ResourceManagerParameters& params )
	{
		m_resourceStorage.create( params.maxResourceCount );
		m_resourceLoader.create( params.pFileSystem, &m_resourceStorage );

		return true;
	}

	void ResourceManager::dispose()
	{
		m_resourceLoader.dispose();
		m_resourceStorage.dispose();
	}

	void ResourceManager::registerResourceType( fourcc type, const FactoryContext& factoryContext )
	{
		m_resourceLoader.registerResourceType( type, factoryContext );
	}

	void ResourceManager::unregisterResourceType( fourcc type )
	{
		m_resourceLoader.unregisterResourceType( type );
	}

	const Resource* ResourceManager::loadGenericResource( fourcc type, crc32 resourceKey, const char* pFileName )
	{
		const Resource* pResource = nullptr;
		ResourceLoaderResult result = m_resourceLoader.loadResource( &pResource, pFileName, resourceKey, type );
		
		switch ( result )
		{
		case ResourceLoaderResult_Success:
			break;

		case ResourceLoaderResult_CouldNotAccessFile:
			TIKI_TRACE_ERROR( "Could not access File: %s\n", pFileName );
			break;

		case ResourceLoaderResult_CouldNotCreateResource:
			TIKI_TRACE_ERROR( "Could not create Resource.\n" );
			break;

		case ResourceLoaderResult_CouldNotInitialize:
			TIKI_TRACE_ERROR( "Could not initialize Resource.\n" );
			break;

		case ResourceLoaderResult_FileNotFound:
			TIKI_TRACE_ERROR( "File not found: %s\n", pFileName );
			break;

		case ResourceLoaderResult_OutOfMemory:
			TIKI_TRACE_ERROR( "Out of Memory.\n" );
			break;

		case ResourceLoaderResult_ResourceNotFound:
			TIKI_TRACE_ERROR( "Resource not found: %u\n", resourceKey );
			break;

		case ResourceLoaderResult_UnknownError:
			TIKI_TRACE_ERROR( "Unknown error.\n" );
			break;

		case ResourceLoaderResult_WrongFileFormat:
			TIKI_TRACE_ERROR( "Wrong File format.\n" );
			break;

		case ResourceLoaderResult_WrongResourceType:
			TIKI_TRACE_ERROR( "Wrong Resource type: %u\n", type );
			break;

		default:
			TIKI_BREAK( "Case not handle.\n" );
		}

		return pResource;	
	}

	void ResourceManager::unloadGenericResource( fourcc type, const Resource* pResource )
	{
		TIKI_ASSERT( pResource != nullptr );
		m_resourceLoader.unloadResource( pResource, type );
	}
}