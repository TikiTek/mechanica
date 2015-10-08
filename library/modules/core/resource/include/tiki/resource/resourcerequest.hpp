#pragma once
#ifndef TIKI_RESOURCEREQUEST_HPP_INCLUDED__
#define TIKI_RESOURCEREQUEST_HPP_INCLUDED__

#include "tiki/base/linkedlist.hpp"
#include "tiki/base/sizedarray.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class Resource;

	class ResourceRequest : private LinkedItem< ResourceRequest >
	{
		TIKI_NONCOPYABLE_CLASS( ResourceRequest );
		friend class ResourceManager;

	public:

									ResourceRequest();
									~ResourceRequest();
		
		void						create(uint resourceCount);
		void						dispose();

		TIKI_FORCE_INLINE bool		isLoading() const;
		TIKI_FORCE_INLINE bool		isFinish() const;
		TIKI_FORCE_INLINE bool		isSuccessful() const;

		template< class T >
		TIKI_FORCE_INLINE const T*	getResource() const;

	private: // friend

		void		create( const char* pFileName, fourcc resourceType );

	private:

		struct ResourceDesc
		{
			crc32			fileNameCrc;
			fourcc			resourceType;
			const Resource*	pResource;
		};

		SizedArray< ResourceDesc >	m_resources;

		volatile bool				m_isLoading;
		volatile bool				m_isFinish;
		volatile bool				m_isSuccessful;

		

	};
}

#include "../../../source/resourcerequest.inl"

#endif // TIKI_RESOURCEREQUEST_HPP_INCLUDED__
