
#include "tiki/resource/resourcemanager.hpp"

#include "tiki/base/file.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/iopath.hpp"
#include "tiki/resource/factorybase.hpp"
#include "tiki/resource/resource.hpp"
#include "tiki/toollibrarys/iassetconverter.hpp"

namespace tiki
{
	ResourceManager::ResourceManager()
	{
		m_pAssetConverter = nullptr;
	}

	ResourceManager::~ResourceManager()
	{
		TIKI_ASSERT( m_pAssetConverter == nullptr );
	}

	bool ResourceManager::create( const ResourceManagerParameters& params )
	{
		m_resourceStorage.create( params.maxResourceCount );
		m_resourceLoader.create( params.pFileSystem, &m_resourceStorage );

		AssetConverterParamter converterParameters;
		converterParameters.sourcePath	= "../../../../../content";
		converterParameters.outputPath	= "../../../../../gamebuild";

		m_pAssetConverter = createAssetConverter();
		m_pAssetConverter->create( converterParameters );
		m_pAssetConverter->startWatch();

		return true;
	}

	void ResourceManager::dispose()
	{
		if ( m_pAssetConverter != nullptr )
		{
			m_pAssetConverter->stopWatch();
			m_pAssetConverter->dispose();
			disposeAssetConverter( m_pAssetConverter );
			m_pAssetConverter = nullptr;
		}

		m_resourceLoader.dispose();
		m_resourceStorage.dispose();
	}

	void ResourceManager::update()
	{
		if ( m_pAssetConverter != nullptr )
		{
			Array< string > files;
			if ( m_pAssetConverter->getChangedFiles( files ) == true )
			{
				m_pAssetConverter->lockConversion();

				for (uint i = 0u; i < files.getCount(); ++i)
				{
					const string& file = files[ i ];
				} 

				m_pAssetConverter->unlockConversion();
				files.dispose();
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

	const Resource* ResourceManager::loadGenericResource( fourcc type, crc32 resourceKey, const char* pFileName )
	{
		TIKI_ASSERT( pFileName != nullptr );
		const crc32 crcFileName = crcString( pFileName );

		Resource* pResource = nullptr;
		ResourceLoaderResult result = m_resourceLoader.loadResource( &pResource, crcFileName, resourceKey, type );
		
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
		if ( pResource == nullptr )
		{
			return;
		}

		m_resourceLoader.unloadResource( pResource, type );
	}
}