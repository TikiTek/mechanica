#pragma once
#ifndef TIKI_RESOURCEMANAGER_INL
#define TIKI_RESOURCEMANAGER_INL

#include "tiki/base/crc32.hpp"
#include "tiki/resource/resource.hpp"

namespace tiki
{
	template<typename T>
	TIKI_INLINE const T* ResourceManager::loadResource( const char* pFileName )
	{
		const Resource* pResource = loadGenericResource( pFileName, T::getResourceType(), crcString( pFileName ) );
		TIKI_ASSERT( pResource == nullptr || pResource->getType() == T::getResourceType() );

		return (const T*)pResource;
	}

	template<typename T>
	TIKI_FORCE_INLINE const ResourceRequest& tiki::ResourceManager::beginResourceLoading( const char* pFileName )
	{
		return beginGenericResourceLoading( pFileName, T::getResourceType(), crcString( pFileName ) );
	}
	
	template< typename T >
	void tiki::ResourceManager::unloadResource( const T*& pResource )
	{
		unloadGenericResource( (const Resource**)&pResource );
	}
}

#endif // TIKI_RESOURCEMANAGER_INL
