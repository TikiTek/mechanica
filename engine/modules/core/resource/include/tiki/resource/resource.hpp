#pragma once

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/resource/resource_base.hpp"

#define TIKI_DEFINE_RESOURCE( class_name, cc )											\
	public:																				\
	virtual fourcc getType() const TIKI_OVERRIDE_FINAL { return s_resourceType; } 	\
	static fourcc getResourceType() { return s_resourceType; }							\
	private:																			\
	static const fourcc s_resourceType = cc;											\
	friend class ResourceLoader;														\
	friend struct FactoryContextGenericBase< class_name >;								\
	friend void memory::callDestructor< class_name >( class_name* pObject )

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

		crc32				getKey() const { return m_name.key; }
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		const char*			getFileName() const { return m_name.fileName.cStr(); }
#else
		const char*			getFileName() const { return ""; }
#endif

		virtual fourcc		getType() const = 0;

	protected:

							Resource();
		virtual				~Resource();

		virtual bool		createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext ) = 0;
		virtual void		disposeInternal( const FactoryContext& factoryContext ) = 0;

	private:

		ResourceName		m_name;
		ResourceSectionData	m_sectionData;

		mutable uint		m_referenceCount;

		bool				create( const ResourceName& name, const ResourceSectionData& sectorData, const ResourceInitData& initData, const FactoryContext& factoryContext );
		void				dispose( const FactoryContext& factoryContext );

		void				addReference() const;
		bool				releaseReference() const;
	};
}
