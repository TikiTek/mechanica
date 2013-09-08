#pragma once
#ifndef TIKI_RESOURCE_HPP
#define TIKI_RESOURCE_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/string.hpp"

#define TIKI_DEFINE_RESOURCE( factory, cc ) public: \
	friend class factory; \
	static const fourcc s_resourceType = cc

namespace tiki
{
	class ResourceManager;

	struct ResourceId
	{
		crc32	crcName;
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		string	fileName;
#endif
	};

	class Resource
	{
		friend class ResourceManager;

	public:

		crc32			getCrcName() const { return m_id.crcName; }
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		const string&	getFileName() const { return m_id.fileName; }
#endif

		size_t			addReference() const;
		size_t			releaseReference() const;

	protected:

						Resource();
		virtual			~Resource();

	private:

		ResourceId		m_id;

		mutable size_t	m_references;

		void			initialize( ResourceId id );

	};
}

#endif // TIKI_RESOURCE_HPP
