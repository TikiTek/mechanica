#pragma once
#ifndef TIKI_RESOURCE_HPP
#define TIKI_RESOURCE_HPP

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"

#define TIKI_DEFINE_RESOURCE( factory, cc ) public: \
	friend class factory; \
	static const fourcc s_resourceType = cc

namespace tiki
{
	class ResourceLoader;
	class ResourceManager;

	struct ResourceId
	{
		ResourceId()
		{
			crcName	= InvalidCrc32;
		}

		crc32	crcName;
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		string	fileName;
#endif
	};

	class Resource
	{
		friend class ResourceLoader;
		friend class ResourceManager;

	public:

		crc32			getCrcName() const { return m_id.crcName; }
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		const string&	getFileName() const { return m_id.fileName; }
#endif

	protected:

						Resource();
		virtual			~Resource();

	private:

		ResourceId		m_id;

		mutable uint	m_references;

		void			initialize( ResourceId id );

		void			addReference() const;
		bool			releaseReference() const;

	};
}

#endif // TIKI_RESOURCE_HPP
