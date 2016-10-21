#pragma once
#ifndef TIKI_RESOURCE_HPP
#define TIKI_RESOURCE_HPP

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/resource/resourcebase.hpp"

#define TIKI_DEFINE_RESOURCE( class_name, cc )											\
	public:																				\
	virtual fourcc getType() const TIKI_OVERRIDE_FINAL { return s_resourceType; } 	\
	static fourcc getResourceType() { return s_resourceType; }							\
	private:																			\
	static const fourcc s_resourceType = cc;											\
	friend class ResourceLoader;														\
	friend struct FactoryContextGenericBase< class_name >;								\
	friend void memory::deleteObjectAligned< class_name >( class_name * ptr )

namespace tiki
{
	class ResourceLoader;
	class ResourceManager;
	class ResourceStorage;

	class Resource
	{
		friend class ResourceLoader;
		friend class ResourceStorage;
		friend class ResourceManager;

	public:

		crc32				getKey() const { return m_id.key; }
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		const char*			getFileName() const { return m_id.fileName.cStr(); }
#else
		const char*			getFileName() const { return ""; }
#endif

		virtual fourcc		getType() const TIKI_PURE;

	protected:

							Resource();
		virtual				~Resource();

		virtual bool		createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext ) TIKI_PURE;
		virtual void		disposeInternal( const FactoryContext& factoryContext ) TIKI_PURE;

	private:

		ResourceId			m_id;
		ResourceSectionData	m_sectionData;

		mutable uint		m_referenceCount;

		bool				create( const ResourceId& id, const ResourceSectionData& sectorData, const ResourceInitData& initData, const FactoryContext& factoryContext );
		void				dispose( const FactoryContext& factoryContext );

		void				addReference() const;
		bool				releaseReference() const;

	};
}

#endif // TIKI_RESOURCE_HPP
