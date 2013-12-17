#pragma once
#ifndef TIKI_RESOURCE_HPP
#define TIKI_RESOURCE_HPP

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/resource/resourcebase.hpp"

#define TIKI_DEFINE_RESOURCE( class_name, cc )				\
	public:													\
	friend class ResourceLoader;							\
	static fourcc getType() { return s_resourceType; }		\
	private:												\
	static const fourcc s_resourceType = cc;				\
	friend struct FactoryContextGenericBase< class_name >

//#define TIKI_DEFAULT_RESOURCE_CODE( class_name )					\
//	Resource* class_name :: createResource()						\
//	{																\
//		return TIKI_NEW class_name ;								\
//	}																\
//	void class_name :: disposeResource( Resource* pRes )			\
//	{																\
//		TIKI_DEL static_cast< class_name * >( pRes );				\
//	}																\
//	const FactoryContext& class_name :: getFactoryContext()			\
//	{																\
//		static TIKI_CONCAT( class_name, FactoryContext ) context;	\
//		context.pCreateResource		= &createResource;				\
//		context.pDisposeResource	= &disposeResource;				\
//		return context;												\
//	}																\
//	typedef class_name * TIKI_CONCAT( class_name, Handle )

namespace tiki
{
	class ResourceLoader;
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

	protected:

							Resource();
		virtual				~Resource();

		virtual bool		createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext ) = 0u;
		virtual void		disposeInternal( const FactoryContext& factoryContext ) = 0u;

	private:

		ResourceId			m_id;
		ResourceSectorData	m_sectorData;

		mutable uint		m_referenceCount;

		bool				create( const ResourceId& id, const ResourceSectorData& sectorData, const ResourceInitData& initData, const FactoryContext& factoryContext );
		void				dispose( const FactoryContext& factoryContext );

		void				addReference() const;
		bool				releaseReference() const;

	};
}

#endif // TIKI_RESOURCE_HPP
