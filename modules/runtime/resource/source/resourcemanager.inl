#pragma once
#ifndef TIKI_RESOURCEMANAGER_INL
#define TIKI_RESOURCEMANAGER_INL

#include "tiki/base/crc32.hpp"

namespace tiki
{
	template<typename T>
	TIKI_INLINE const T* ResourceManager::loadResource( const string& fileName )
	{
		ResourceId id;
		id.crcName		= crcString( fileName );
#if TIKI_BUILD_MASTER == 0
		id.fileName		= fileName;
#endif

		return (const T*)loadGenericResource( T::s_resourceType, id );
	}

	template<typename T>
	TIKI_INLINE void ResourceManager::unloadResource( const T* pResource )
	{
		unloadGenericResource( T::s_resourceType, pResource );
	}
}

#endif // TIKI_RESOURCEMANAGER_INL
