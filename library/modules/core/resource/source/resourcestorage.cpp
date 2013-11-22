
#include "tiki/resource/resourcestorage.hpp"

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

	bool ResourceStorage::findResource( const Resource** ppResource, crc32 resourceKey ) const
	{
		return m_resources.findValue( ppResource, resourceKey );
	}

	void ResourceStorage::allocateResource( const Resource* pResource, const ResourceId& resourceId, const ResourceSectorData& sectionData )
	{
		TIKI_ASSERT( pResource != nullptr );

		pResource->initialize( resourceId, sectionData );

		m_resources.set( resourceId.key, pResource );
	}
	
	void ResourceStorage::addReferenceToResource( const Resource* pResource )
	{
		TIKI_ASSERT( pResource != nullptr );

		pResource->addReference();
	}

	bool ResourceStorage::freeReferenceFromResource( const Resource* pResource )
	{
		TIKI_ASSERT( pResource != nullptr );

		return pResource->releaseReference();
	}
}