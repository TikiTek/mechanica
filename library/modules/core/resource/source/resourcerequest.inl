#pragma once
#ifndef TIKI_RESOURCEREQUEST_INL_INCLUDED__
#define TIKI_RESOURCEREQUEST_INL_INCLUDED__

namespace tiki
{
	TIKI_FORCE_INLINE ResourceRequest::ResourceRequest()
	{
		m_fileNameCrc	= TIKI_INVALID_CRC32;
		m_resourceType	= 0;
		m_resourceKey	= TIKI_INVALID_CRC32;
		m_pResource		= nullptr;
	}

	TIKI_FORCE_INLINE ResourceRequest::~ResourceRequest()
	{
		TIKI_ASSERT( !m_isLoading );
	}

	TIKI_FORCE_INLINE bool ResourceRequest::isLoading() const
	{
		return m_isLoading;
	}

	TIKI_FORCE_INLINE bool ResourceRequest::isSuccessful() const
	{
		return m_pResource != nullptr;
	}

	template<>
	TIKI_FORCE_INLINE const Resource* ResourceRequest::getResource() const
	{
		TIKI_ASSERT( m_pResource != nullptr );
		return m_pResource;
	}

	template< class T >
	TIKI_FORCE_INLINE const T* ResourceRequest::getResource() const
	{
		TIKI_ASSERT( m_pResource != nullptr );
		TIKI_ASSERT( m_pResource->getType() == T::getResourceType() );
		return static_cast< const T* >( m_pResource );
	}
}

#endif // TIKI_RESOURCEREQUEST_INL_INCLUDED__
