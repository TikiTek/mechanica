#include "tiki/resource/resource_manager.hpp"

#include "tiki/asset_converter_interface/asset_converter_interface.hpp"
#include "tiki/base/debug_property.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/path.hpp"
#include "tiki/resource/factory_base.hpp"
#include "tiki/resource/resource.hpp"
#include "tiki/resource/resource_request.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_BOOL( s_enableAssetConverterWatch, "EnableAssetConverterWatch", true );

	ResourceManager::ResourceManager()
	{
#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		m_pAssetConverter = nullptr;
#endif
	}

	ResourceManager::~ResourceManager()
	{
	}

	bool ResourceManager::create( const ResourceManagerParameters& parameters )
	{
		m_resourceStorage.create( parameters.maxResourceCount );
		m_resourceLoader.create( parameters.pFileSystem, &m_resourceStorage );

		if ( !m_resourceRequests.create( parameters.maxRequestCount ) )
		{
			dispose();
			return false;
		}

		m_loadingMutex.create();
		if( parameters.enableMultiThreading )
		{
			if( !m_loadingThread.create( staticThreadEntry, this, 1024 * 1024, "ResourceManager" ) )
			{
				dispose();
				return false;
			}
		}

#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		m_pFileSystem = parameters.pFileSystem;

		AssetConverterParamter assetConverterParameters;
		assetConverterParameters.pProject = parameters.pProject;
		m_pAssetConverter = createAssetConverter( assetConverterParameters );
		if ( m_pAssetConverter != nullptr &&
			 s_enableAssetConverterWatch )
		{
			m_pAssetConverter->startWatch();
		}
#endif

		return true;
	}

	void ResourceManager::dispose()
	{
#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		if ( m_pAssetConverter != nullptr )
		{
			if ( s_enableAssetConverterWatch )
			{
				m_pAssetConverter->stopWatch();
			}

			disposeAssetConverter( m_pAssetConverter );
			m_pAssetConverter = nullptr;
		}
#endif

		if( m_loadingThread.isCreated() )
		{
			m_loadingThread.requestExit();
			m_loadingThread.waitForExit();
			m_loadingThread.dispose();
		}
		m_loadingMutex.dispose();

		m_resourceRequests.dispose();

		m_resourceLoader.dispose();
		m_resourceStorage.dispose();
	}

	void ResourceManager::update()
	{
#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		if ( m_pAssetConverter != nullptr && s_enableAssetConverterWatch )
		{
			Array< DynamicString > files;
			if ( m_pAssetConverter->getChangedFiles( files ) )
			{
				for (uint i = 0u; i < files.getCount(); ++i)
				{
					const DynamicString& file = files[ i ];
					const DynamicString fileName = path::getFilename( file );
					const crc32 resourceKey = crcString( fileName );

					Resource* pResource = nullptr;
					m_resourceStorage.findResource( &pResource, resourceKey );

					if ( pResource != nullptr )
					{
						const fourcc resourceType = pResource->getType();

						const ResourceLoaderResult result = m_resourceLoader.reloadResource( pResource, resourceKey, resourceKey, resourceType );
						traceResourceLoadResult( result, fileName.cStr(), resourceKey, resourceType );
					}
				}

				files.dispose();
			}
		}
#endif

		if( !m_loadingThread.isCreated() )
		{
			while( !m_runningRequests.isEmpty() )
			{
				ResourceRequest& data = *m_runningRequests.getBegin();
				m_runningRequests.removeSortedByValue( data );

				if( !updateResourceLoading( &data ) )
				{
					m_runningRequests.push( data );
				}
			}
		}
	}

	void ResourceManager::registerResourceType( fourcc type, const FactoryContext& factoryContext )
	{
		m_resourceLoader.registerResourceType( type, factoryContext );
	}

	void ResourceManager::unregisterResourceType( fourcc type )
	{
		m_resourceLoader.unregisterResourceType( type );
	}

	void ResourceManager::unloadGenericResource( const Resource** ppResource )
	{
		TIKI_ASSERT( ppResource != nullptr );

		if ( *ppResource == nullptr )
		{
			return;
		}

		m_resourceLoader.unloadResource( *ppResource, (*ppResource)->getType() );
		*ppResource = nullptr;
	}

	void ResourceManager::endResourceLoading( const ResourceRequest& request )
	{
		TIKI_ASSERT( !request.isLoading() );
		m_resourceRequests.removeUnsortedByValue( request );
	}

	const Resource* ResourceManager::loadGenericResource( const char* pFileName, fourcc type, crc32 resourceKey )
	{
		const ResourceRequest& request = beginGenericResourceLoading( pFileName, type, resourceKey );

		while (request.isLoading())
		{
			update();
			Thread::sleepCurrentThread( 250 );
		}

		const Resource* pResource = request.m_pResource;
		endResourceLoading( request );
		return pResource;
	}

	const ResourceRequest& ResourceManager::beginGenericResourceLoading( const char* pFileName, fourcc type, crc32 resourceKey )
	{
		TIKI_ASSERT( pFileName != nullptr );
		const crc32 crcFileName = crcString( pFileName );

		ResourceRequest& request = m_resourceRequests.push();
		request.m_fileNameCrc	= crcFileName;
		request.m_resourceType	= type;
		request.m_resourceKey	= resourceKey;
		request.m_pResource		= nullptr;
		request.m_isLoading		= true;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		request.m_pFileName		= pFileName;
#endif

		m_loadingMutex.lock();
		m_runningRequests.push( request );
		m_loadingMutex.unlock();

		return request;
	}

	void ResourceManager::traceResourceLoadResult( ResourceLoaderResult result, const char* pFileName, crc32 resourceKey, fourcc resourceType )
	{
		switch ( result )
		{
		case ResourceLoaderResult_Success:
			TIKI_TRACE_DEBUG( "[resource] File loaded: %s\n", pFileName );
			break;

		case ResourceLoaderResult_CouldNotAccessFile:
			TIKI_TRACE_ERROR( "[resource] Could not access File: %s\n", pFileName );
			break;

		case ResourceLoaderResult_CouldNotCreateResource:
			TIKI_TRACE_ERROR( "[resource] Could not create Resource.\n" );
			break;

		case ResourceLoaderResult_CouldNotInitialize:
			TIKI_TRACE_ERROR( "[resource] Could not initialize Resource.\n" );
			break;

		case ResourceLoaderResult_FileNotFound:
			TIKI_TRACE_ERROR( "[resource] File not found: %s\n", pFileName );
			break;

		case ResourceLoaderResult_OutOfMemory:
			TIKI_TRACE_ERROR( "[resource] Out of Memory.\n" );
			break;

		case ResourceLoaderResult_ResourceNotFound:
			TIKI_TRACE_ERROR( "[resource] Resource not found: %u\n", resourceKey );
			break;

		case ResourceLoaderResult_UnknownError:
			TIKI_TRACE_ERROR( "[resource] Unknown error.\n" );
			break;

		case ResourceLoaderResult_WrongFileFormat:
			TIKI_TRACE_ERROR( "[resource] Wrong File format.\n" );
			break;

		case ResourceLoaderResult_WrongResourceType:
			TIKI_TRACE_ERROR( "[resource] Wrong Resource type: %u\n", resourceType );
			break;

		default:
			TIKI_BREAK( "Case not handle.\n" );
		}
	}

	void ResourceManager::threadEntry( const Thread& thread )
	{
		while (!thread.isExitRequested())
		{
			ResourceRequest* pData = nullptr;

			m_loadingMutex.lock();
			if (!m_runningRequests.isEmpty())
			{
				pData = &*m_runningRequests.getBegin();
				m_runningRequests.removeSortedByValue(*m_runningRequests.getBegin());
			}
			m_loadingMutex.unlock();

			if ( pData == nullptr )
			{
				Thread::sleepCurrentThread( 500 );
				continue;
			}

			while( !updateResourceLoading( pData ) )
			{
				Thread::sleepCurrentThread( 100 );
			}
		}
	}

	int ResourceManager::staticThreadEntry( const Thread& thread )
	{
		ResourceManager* pManager = (ResourceManager*)thread.getArgument();
		pManager->threadEntry( thread );

		return 0;
	}

	bool ResourceManager::updateResourceLoading( ResourceRequest* pData )
	{
		ResourceRequest& request = *pData;

#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		if( m_pAssetConverter != nullptr &&
			m_pAssetConverter->popFinishConversion() )
		{
			m_pFileSystem->rescan();
		}
#endif

		ResourceLoaderResult result = m_resourceLoader.loadResource( &request.m_pResource, request.m_fileNameCrc, request.m_resourceKey, request.m_resourceType, true );
#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		if( m_pAssetConverter != nullptr &&
			result == ResourceLoaderResult_FileNotFound )
		{
			if( !m_pAssetConverter->isConvertionRunning() )
			{
				m_pAssetConverter->queueAll();
			}
			return false;
		}
#endif

		const char* pFileName = (request.m_pResource != nullptr ? request.m_pResource->getFileName() : "");
		traceResourceLoadResult( result, pFileName, request.m_fileNameCrc, request.m_resourceType );

		request.m_isLoading = false;
		return true;
	}
}