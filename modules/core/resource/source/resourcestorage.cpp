
#include "tiki/resource/resourcestorage.hpp"

#include "tiki/resource/resource.hpp"

namespace tiki
{
	ResourceStorage::ResourceStorage()
	{
	}

	ResourceStorage::~ResourceStorage()
	{
	}

	void ResourceStorage::create( uint maxResourceCount )
	{
		m_resources.create( maxResourceCount );
	}

	void ResourceStorage::dispose()
	{
		m_resources.dispose();
	}

	bool ResourceStorage::findResource( Resource** ppResource, crc32 resourceKey ) const
	{
		return m_resources.findValue( ppResource, resourceKey );
	}

	void ResourceStorage::allocateResource( Resource* pResource, const ResourceId& resourceId, const ResourceSectorData& sectionData )
	{
		TIKI_ASSERT( pResource != nullptr );

		pResource->initialize( resourceId, sectionData );

		m_resources.set( resourceId.key, pResource );
	}
	
	void ResourceStorage::addReferenceToResource( Resource* pResource )
	{
		TIKI_ASSERT( pResource != nullptr );

		pResource->addReference();
	}

	bool ResourceStorage::freeReferenceFromResource( Resource* pResource )
	{
		TIKI_ASSERT( pResource != nullptr );

		if ( pResource->releaseReference() )
		{
			return m_resources.remove( pResource->getKey() );

		}

		return false;
	}
}