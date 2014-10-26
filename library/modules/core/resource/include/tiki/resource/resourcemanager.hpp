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
	class FactoryBase;
	class IAssetConverter;
	class Resource;
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

		void						update();

		void						registerResourceType( fourcc type, const FactoryContext& factoryContext );
		void						unregisterResourceType( fourcc type );
		
		template<typename T>
		TIKI_FORCE_INLINE const T*	loadResource( const string& fileName );

		template<typename T>
		TIKI_FORCE_INLINE void		unloadResource( const T* pResource );

	private:

		ResourceLoader				m_resourceLoader;
		ResourceStorage				m_resourceStorage;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		IAssetConverter*			m_pAssetConverter;
#endif

		const Resource*				loadGenericResource( fourcc type, crc32 resourceKey, const char* pFileName );
		void						unloadGenericResource( fourcc type, const Resource* pResource );

		void						traceResourceLoadResult( ResourceLoaderResult result, const char* pFileName, crc32 resourceKey, fourcc resourceType );

	};
}

#include "../../../source/resourcemanager.inl"

#endif // TIKI_RESOURCEMANAGER_HPP