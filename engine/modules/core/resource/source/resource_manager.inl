#pragma once

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
	TIKI_FORCE_INLINE const ResourceRequest& ResourceManager::beginResourceLoading( const char* pFileName )
	{
		return beginGenericResourceLoading( pFileName, T::getResourceType(), crcString( pFileName ) );
	}

	template< typename T >
	void ResourceManager::unloadResource( const T*& pResource )
	{
		unloadGenericResource( (const Resource**)&pResource );
	}
}
