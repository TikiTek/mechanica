
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
		m_gamebuildPath = params.pGamebuildPath;

		m_resources.create( 1000u );
		m_factories.create( 10u );

		return true;
	}

	void ResourceManager::dispose()
	{
		TIKI_ASSERT( m_factories.getCount() == 0u );
		TIKI_ASSERT( m_resources.getCount() == 0u );

		m_gamebuildPath = "";

		m_resources.dispose();
		m_factories.dispose();
	}

	void ResourceManager::registerFactory( FactoryBase* pFactory )
	{
		m_factories.push( pFactory );
	}

	void ResourceManager::unregisterFactory( FactoryBase* pFactory )
	{
		m_factories.remove( pFactory );
	}

	const Resource* ResourceManager::loadGenericResource( fourcc type, const ResourceId& resourceId )
	{
		for (size_t i = 0u; i < m_resources.getCount(); ++i)
		{
			if ( m_resources[ i ]->getKey() == resourceId.key )
			{
				m_resources[ i ]->addReference();
				return m_resources[ i ];
			}
		}

		FactoryBase* pFactory = nullptr;

		for (size_t i = 0u; i < m_factories.getCount(); ++i)
		{
			if ( m_factories[ i ]->getType() == type )
			{
				pFactory = m_factories[ i ];
				break;
			}
		}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		if ( pFactory == nullptr )
		{
			TIKI_TRACE( "Factory not registed for Resource '%s'\n", resourceId.fileName.cStr() );
			return nullptr;
		}
#endif

		Array< uint8 > fileContent;
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		string fullPath = path::combine( m_gamebuildPath, resourceId.fileName );

		if ( !file::exists( fullPath ) )
		{
			TIKI_TRACE( "File not found: %s\n", fullPath.cStr() );
			return nullptr;
		}

		if ( !file::readAllBytes( fullPath, fileContent ) )
		{
			TIKI_TRACE( "Can't access to File: %s\n", fullPath.cStr() );
			return nullptr;
		}

		const uint32* pInfo = (const uint32*)fileContent.getData();

		if ( (fourcc)pInfo[ 0u ] != TIKI_FOURCC( 'T', 'I', 'K', 'I' ) )
		{
			TIKI_TRACE( "File is not a valid Resource: %s\n", fullPath.cStr() );
			fileContent.dispose();
			return nullptr;
		}

		if ( (fourcc)pInfo[ 1u ] != type )
		{
			TIKI_TRACE( "Resource has a wrong type: %s\n", fullPath.cStr() );
			fileContent.dispose();
			return nullptr;
		}
#else
#	error not implemented
#endif

		Resource* pResource = pFactory->createResource();
		
		if ( pResource )
		{
			//pResource->initialize( resourceId );

			const void* pData = (const void*)(fileContent.getData() + 16u);

			if ( pFactory->initializeResource( pResource, pData ) )
			{
				pResource->addReference();
				m_resources.push( pResource );
			}
			else
			{
				pFactory->destroyResource( pResource );
				pResource = nullptr;
			}
		}

		fileContent.dispose();

		return pResource;	
	}

	void ResourceManager::unloadGenericResource( fourcc type, const Resource* pResource )
	{
		TIKI_ASSERT( pResource );

		Resource* pNonConstResource = (Resource*)pResource;

		if ( pNonConstResource->releaseReference() )
		{
			return;
		}

		FactoryBase* pFactory = nullptr;

		for (size_t i = 0u; i < m_factories.getCount(); ++i)
		{
			if ( m_factories[ i ]->getType() == type )
			{
				pFactory = m_factories[ i ];
				break;
			}
		}
		TIKI_ASSERT( pFactory );

		pFactory->destroyResource( pNonConstResource );
		m_resources.remove( pNonConstResource );
	}
}