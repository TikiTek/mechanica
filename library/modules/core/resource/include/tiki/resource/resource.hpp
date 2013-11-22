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
	class ResourceStorage;

	struct ResourceId
	{
		ResourceId()
		{
			key = InvalidCrc32;
		}

		crc32	key;
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		string	fileName;
#endif
	};

	struct ResourceSectorData
	{
		ResourceSectorData()
		{
			ppSectorPointers	= nullptr;
			sectorCount			= 0u;
		}

		void**	ppSectorPointers;
		uint	sectorCount;
	};

	class Resource
	{
		friend class ResourceLoader;
		friend class ResourceManager;

	public:

		crc32			getKey() const { return m_id.key; }
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		const string&	getFileName() const { return m_id.fileName; }
#endif

	protected:

						Resource();
		virtual			~Resource();

	private:

		ResourceId			m_id;
		ResourceSectorData	m_sectorData;

		mutable uint		m_referenceCount;

		void				initialize( const ResourceId& id, const ResourceSectorData& sectorData );

		void				addReference() const;
		bool				releaseReference() const;

	};
}

#endif // TIKI_RESOURCE_HPP
