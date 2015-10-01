#pragma once
#ifndef TIKI_RESOURCEREQUEST_HPP_INCLUDED__
#define TIKI_RESOURCEREQUEST_HPP_INCLUDED__

#include "tiki/base/linkedlist.hpp"

namespace tiki
{
	class ResourceRequest : public LinkedItem< ResourceRequest > TIKI_FINAL
	{
		TIKI_NONCOPYABLE_CLASS( ResourceRequest );
		friend class ResourceManager;

	public:

		TIKI_FORCE_INLINE			ResourceRequest();
		TIKI_FORCE_INLINE			~ResourceRequest();
				
		TIKI_FORCE_INLINE bool		isLoading() const;
		TIKI_FORCE_INLINE bool		isFinish() const;
		TIKI_FORCE_INLINE bool		isSuccessful() const;

		template< class T >
		TIKI_FORCE_INLINE const T*	getResource() const;

	private: // friend

		TIKI_FORCE_INLINE void		create( const char* pFileName, fourcc resourceType );

	private:

		crc32			m_fileNameCrc;
		fourcc			m_resourceType;

		volatile bool	m_isLoading;
		volatile bool	m_isFinish;

		const Resource*	m_pResource;

	};
}

#include "../../../source/resourcerequest.inl"

#endif // TIKI_RESOURCEREQUEST_HPP_INCLUDED__
