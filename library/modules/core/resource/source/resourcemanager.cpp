
#include "tiki/resource/resourcemanager.hpp"

#include "tiki/base/debugprop.hpp"
#include "tiki/base/file.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/iopath.hpp"
#include "tiki/resource/factorybase.hpp"
#include "tiki/resource/resource.hpp"
#include "tiki/toollibraries/iassetconverter.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_BOOL( s_enableAssetConverterWatch, "EnableAssetConverterWatch", true );

	ResourceManager::ResourceManager()
	{
#if TIKI_ENABLED( TIKI_ENABLE_ASSET_CONVERTER )
		m_pAssetConverter = nullptr;
#endif
	}

	ResourceManager::~ResourceManager()
	{
	}

	bool ResourceManager::create( const ResourceManagerParameters& params )
	{
		m_resourceStorage.create( params.maxResourceCount );
		m_resourceLoader.create( params.pFileSystem, &m_resourceStorage );

#if TIKI_ENABLED( TIKI_ENABLE_ASSET_CONVERTER )
		AssetConverterParamter converterParameters;
		converterParameters.sourcePath	= "../../../../../content";
		converterParameters.outputPath	= "../../../../../gamebuild";

		m_pAssetConverter = createAssetConverter();
		m_pAssetConverter->create( converterParameters );
		//m_pAssetConverter->convertAll();

		m_resourceLoader.reloadResourceMapping();

		if ( s_enableAssetConverterWatch )
		{
			m_pAssetConverter->startWatch();
		}
#endif

		return true;
	}

	void ResourceManager::dispose()
	{
#if TIKI_ENABLED( TIKI_ENABLE_ASSET_CONVERTER )
		if ( m_pAssetConverter != nullptr )
		{
			if ( s_enableAssetConverterWatch )
			{
				m_pAssetConverter->stopWatch();
			}

			m_pAssetConverter->dispose();
			disposeAssetConverter( m_pAssetConverter );
			m_pAssetConverter = nullptr;
		}
#endif

		m_resourceLoader.dispose();
		m_resourceStorage.dispose();
	}

	void ResourceManager::update()
	{
#if TIKI_ENABLED( TIKI_ENABLE_ASSET_CONVERTER )
		if ( m_pAssetConverter != nullptr && s_enableAssetConverterWatch )
		{
			Array< string > files;
			if ( m_pAssetConverter->getChangedFiles( files ) )
			{
				m_pAssetConverter->lockConversion();

				for (uint i = 0u; i < files.getCount(); ++i)
				{
					const string& file = files[ i ];
					const string fileName = path::getFilename( file );
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

				m_pAssetConverter->unlockConversion();
				files.dispose();
			}
		}
#endif
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
#if TIKI_ENABLED( TIKI_ENABLE_ASSET_CONVERTER )
		if ( s_enableAssetConverterWatch )
		{
			m_pAssetConverter->lockConversion();
		}
#endif

		TIKI_ASSERT( pFileName != nullptr );
		const crc32 crcFileName = crcString( pFileName );

		Resource* pResource = nullptr;
		const ResourceLoaderResult result = m_resourceLoader.loadResource( &pResource, crcFileName, resourceKey, type );
		traceResourceLoadResult( result, pFileName, crcFileName, type );

#if TIKI_ENABLED( TIKI_ENABLE_ASSET_CONVERTER )
		if ( s_enableAssetConverterWatch )
		{
			m_pAssetConverter->unlockConversion();
		}
#endif

		return pResource;	
	}

	void ResourceManager::unloadGenericResource( fourcc type, const Resource* pResource )
	{
		if ( pResource == nullptr )
		{
			return;
		}

		m_resourceLoader.unloadResource( pResource, type );
	}

	void ResourceManager::traceResourceLoadResult( ResourceLoaderResult result, const char* pFileName, crc32 resourceKey, fourcc resourceType )
	{
		switch ( result )
		{
		case ResourceLoaderResult_Success:
			break;

		case ResourceLoaderResult_CouldNotAccessFile:
			TIKI_TRACE_ERROR( "[resourcemanager] Could not access File: %s\n", pFileName );
			break;

		case ResourceLoaderResult_CouldNotCreateResource:
			TIKI_TRACE_ERROR( "[resourcemanager] Could not create Resource.\n" );
			break;

		case ResourceLoaderResult_CouldNotInitialize:
			TIKI_TRACE_ERROR( "[resourcemanager] Could not initialize Resource.\n" );
			break;

		case ResourceLoaderResult_FileNotFound:
			TIKI_TRACE_ERROR( "[resourcemanager] File not found: %s\n", pFileName );
			break;

		case ResourceLoaderResult_OutOfMemory:
			TIKI_TRACE_ERROR( "[resourcemanager] Out of Memory.\n" );
			break;

		case ResourceLoaderResult_ResourceNotFound:
			TIKI_TRACE_ERROR( "[resourcemanager] Resource not found: %u\n", resourceKey );
			break;

		case ResourceLoaderResult_UnknownError:
			TIKI_TRACE_ERROR( "[resourcemanager] Unknown error.\n" );
			break;

		case ResourceLoaderResult_WrongFileFormat:
			TIKI_TRACE_ERROR( "[resourcemanager] Wrong File format.\n" );
			break;

		case ResourceLoaderResult_WrongResourceType:
			TIKI_TRACE_ERROR( "[resourcemanager] Wrong Resource type: %u\n", resourceType );
			break;

		default:
			TIKI_BREAK( "Case not handle.\n" );
		}
	}

}