#pragma once

#include "tiki/base/types.hpp"
#include "tiki/container/sorted_sized_map.hpp"

namespace tiki
{
	class Resource;
	struct ResourceId;
	struct ResourceSectorData;

	class ResourceStorage
	{
		TIKI_NONCOPYABLE_CLASS( ResourceStorage );

	public:

		ResourceStorage();
		~ResourceStorage();

		void	create( uint maxResourceCount );
		void	dispose();

		bool	findResource( Resource** ppResource, crc32 resourceKey ) const;

		void	allocateResource( Resource* pResource, const ResourceId& resourceId );
		void	addReferenceToResource( Resource* pResource );
		bool	freeReferenceFromResource( Resource* pResource );

	private:

		SortedSizedMap< crc32, Resource* >	m_resources;
	};
}
