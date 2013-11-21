#pragma once
#ifndef __TIKI_RESOURCESTOREAGE_HPP_INCLUDED__
#define __TIKI_RESOURCESTOREAGE_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	class Resource;

	class ResourceStorage
	{
		TIKI_NONCOPYABLE_CLASS( ResourceStorage );

	public:

		ResourceStorage();
		~ResourceStorage();

		void			create( uint maxResourceCount );
		void			dispose();

		bool			allocateResource( Resource** ppResource, crc32 resourceKey );
		bool			freeResource( Resource* pResource );

	private:


	};
}

#endif // __TIKI_RESOURCESTOREAGE_HPP_INCLUDED__
