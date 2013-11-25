#pragma once
#ifndef TIKI_RESOURCEMANAGER_HPP
#define TIKI_RESOURCEMANAGER_HPP

#include "tiki/base/inline.hpp"
#include "tiki/base/sizedarray.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/resource/resourceloader.hpp"
#include "tiki/resource/resourcestorage.hpp"

namespace tiki
{
	class Resource;
	class FactoryBase;
	struct ResourceId;

	struct ResourceManagerParameters
	{
		ResourceManagerParameters()
		{
			maxResourceCount	= 1000u;

			pFileSystem			= nullptr;
		}

		uint			maxResourceCount;

		FileSystem*		pFileSystem;
	};

	class ResourceManager
	{
		TIKI_NONCOPYABLE_CLASS( ResourceManager );

	public:

		ResourceManager();
		~ResourceManager();

		bool						create( const ResourceManagerParameters& params );
		void						dispose();

		void						registerFactory( FactoryBase& factory );
		void						unregisterFactory( FactoryBase& factory );

		template<typename T>
		TIKI_INLINE const T*		loadResource( const string& fileName );

		template<typename T>
		TIKI_INLINE void			unloadResource( const T* pResource );

	private:

		ResourceLoader				m_resoureLoader;
		ResourceStorage				m_resourceStorage;

		const Resource*				loadGenericResource( fourcc type, crc32 resourceKey, const char* pFileName );
		void						unloadGenericResource( fourcc type, const Resource* pResource );

	};
}

#include "../../../source/resourcemanager.inl"

#endif // TIKI_RESOURCEMANAGER_HPP
