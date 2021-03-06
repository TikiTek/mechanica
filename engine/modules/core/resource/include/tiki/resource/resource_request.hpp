#pragma once

#include "tiki/container/linked_list.hpp"
#include "tiki/container/sized_array.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class Resource;

	class ResourceRequest : private LinkedItem< ResourceRequest >
	{
		TIKI_NONCOPYABLE_CLASS( ResourceRequest );
		friend class ResourceManager;
		friend class LinkedList< ResourceRequest >;

	public:

		TIKI_FORCE_INLINE			ResourceRequest();
		TIKI_FORCE_INLINE			~ResourceRequest();

		TIKI_FORCE_INLINE bool		isLoading() const;
		TIKI_FORCE_INLINE bool		isSuccessful() const;

		template< class T >
		TIKI_FORCE_INLINE const T*	getResource() const;

	private:

		crc32						m_fileNameCrc;
		fourcc						m_resourceType;
		crc32						m_resourceKey;
		const Resource*				m_pResource;

		volatile bool				m_isLoading;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		const char*					m_pFileName;
#endif
	};
}

#include "../../../source/resource_request.inl"
