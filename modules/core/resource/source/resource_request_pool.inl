#pragma once

#include  "tiki/resource/resource_manager.hpp"

namespace tiki
{
	template< typename T >
	void tiki::ResourceRequestPool::beginLoadResource( T** ppTargetResource, const char* pFileName )
	{
		return beginLoadResource( (const T**)ppTargetResource, pFileName );
	}

	template< typename T >
	void ResourceRequestPool::beginLoadResource( const T** ppTargetResource, const char* pFileName )
	{
		TIKI_ASSERT( m_pResourceManager != nullptr );

		ResourceRequestData& data = m_requests.push();
		data.ppTargetResource	= (const Resource**)ppTargetResource;
		data.pRequest			= &m_pResourceManager->beginResourceLoading< T >( pFileName );
	}

	template< typename T >
	void ResourceRequestPool::unloadResource( const T*& pResource )
	{
		TIKI_ASSERT( m_pResourceManager != nullptr );

		m_pResourceManager->unloadResource< T >( pResource );
	}
}
