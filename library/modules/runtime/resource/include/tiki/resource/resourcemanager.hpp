#pragma once
#ifndef TIKI_RESOURCEMANAGER_HPP
#define TIKI_RESOURCEMANAGER_HPP

#include "tiki/base/inline.hpp"
#include "tiki/base/sizedarray.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class Resource;
	class FactoryBase;
	struct ResourceId;

	struct ResourceManagerParameters
	{
		cstring		pGamebuildPath;
	};

	class ResourceManager
	{
		TIKI_NONCOPYABLE_CLASS( ResourceManager );

	public:

		ResourceManager();
		~ResourceManager();

		bool						create( const ResourceManagerParameters& params );
		void						dispose();

		void						registerFactory( FactoryBase* pFactory );
		void						unregisterFactory( FactoryBase* pFactory );

		template<typename T>
		TIKI_INLINE const T*		loadResource( const string& fileName );

		template<typename T>
		TIKI_INLINE void			unloadResource( const T* pResource );

	private:

		SizedArray< FactoryBase* >	m_factories;
		SizedArray< Resource* >		m_resources;

		string						m_gamebuildPath;

		const Resource*				loadGenericResource( fourcc type, const ResourceId& resourceId );
		void						unloadGenericResource( fourcc type, const Resource* pResource );

	};
}

#include "resourcemanager.inl"

#endif // TIKI_RESOURCEMANAGER_HPP
