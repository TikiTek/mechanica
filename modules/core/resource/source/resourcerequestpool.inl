#pragma once
#ifndef TIKI_RESOURCEREQUESTPOOL_INL_INCLUDED__
#define TIKI_RESOURCEREQUESTPOOL_INL_INCLUDED__

namespace tiki
{
	template< typename T >
	void ResourceRequestPool::beginLoadResource( const T** ppTargetResource, const char* pFileName )
	{
		TIKI_ASSERT( m_pResourceManager != nullptr );

		ResourceRequestData& data = m_requests.push();
		data.ppTargetResource	= (const Resource**)ppTargetResource;
		data.pRequest			= &m_pResourceManager->beginResourceLoading< T >( pFileName );
	}
	
	template< typename T >
	void tiki::ResourceRequestPool::unloadResource( const T*& pResource )
	{
		TIKI_ASSERT( m_pResourceManager != nullptr );

		m_pResourceManager->unloadResource< T >( pResource );
	}
}

#endif // TIKI_RESOURCEREQUESTPOOL_INL_INCLUDED__
