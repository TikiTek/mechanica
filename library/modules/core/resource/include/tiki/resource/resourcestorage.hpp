#pragma once
#ifndef __TIKI_RESOURCESTOREAGE_HPP_INCLUDED__
#define __TIKI_RESOURCESTOREAGE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/container/sortedsizedmap.hpp"

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

#endif // __TIKI_RESOURCESTOREAGE_HPP_INCLUDED__
