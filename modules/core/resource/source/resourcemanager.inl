#pragma once
#ifndef TIKI_RESOURCEMANAGER_INL
#define TIKI_RESOURCEMANAGER_INL

#include "tiki/base/crc32.hpp"
#include "tiki/resource/resource.hpp"

namespace tiki
{
	template<typename T>
	TIKI_INLINE const T* ResourceManager::loadResource( const string& fileName )
	{
		return (const T*)loadGenericResource( T::s_resourceType, crcString( fileName ), fileName.cStr() );
	}

	template<typename T>
	TIKI_INLINE void ResourceManager::unloadResource( const T* pResource )
	{
		unloadGenericResource( T::s_resourceType, pResource );
	}
}

#endif // TIKI_RESOURCEMANAGER_INL
