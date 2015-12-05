#pragma once
#ifndef TIKI_RESOURCEMANAGER_HPP
#define TIKI_RESOURCEMANAGER_HPP

#include "tiki/base/basicstring.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/sizedarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/pool.hpp"
#include "tiki/resource/resourceloader.hpp"
#include "tiki/resource/resourcestorage.hpp"
#include "tiki/threading/mutex.hpp"
#include "tiki/threading/thread.hpp"

#if TIKI_DISABLED( TIKI_BUILD_MASTER ) && TIKI_DISABLED( TIKI_BUILD_TOOLS )
#	define TIKI_ENABLE_ASSET_CONVERTER TIKI_ON
#else
#	define TIKI_ENABLE_ASSET_CONVERTER TIKI_OFF
#endif

namespace tiki
{
	class FactoryBase;
	class IAssetConverter;
	class Resource;
	class ResourceRequest;
	struct ResourceId;

	struct ResourceManagerParameters
	{
		ResourceManagerParameters()
		{
			maxResourceCount	= 1000u;
			maxRequestCount		= 128u;

			pFileSystem			= nullptr;
		}

		uint			maxResourceCount;
		uint			maxRequestCount;

		FileSystem*		pFileSystem;
	};

	class ResourceManager
	{
		TIKI_NONCOPYABLE_CLASS( ResourceManager );

	public:

													ResourceManager();
													~ResourceManager();

		bool										create( const ResourceManagerParameters& params );
		void										dispose();

		void										update();

		void										registerResourceType( fourcc type, const FactoryContext& factoryContext );
		void										unregisterResourceType( fourcc type );
		
		template<typename T>
		TIKI_FORCE_INLINE const T*					loadResource( const char* pFileName );
		void										unloadResource( const Resource* pResource );

		template<typename T>
		TIKI_FORCE_INLINE const ResourceRequest&	beginResourceLoading( const char* pFileName );
		void										endResourceLoading( const ResourceRequest& request );

	private:

		struct ResourceRequestData;

		ResourceLoader						m_resourceLoader;
		ResourceStorage						m_resourceStorage;

		Pool< ResourceRequestData >			m_resourceRequests;
		LinkedList< ResourceRequestData >	m_runningRequests;
		//SPSCQueue							m_runningRequests;

		Thread								m_loadingThread;
		Mutex								m_loadingMutex;

#if TIKI_ENABLED( TIKI_ENABLE_ASSET_CONVERTER )
		IAssetConverter*					m_pAssetConverter;
#endif

		ResourceRequestData&				getDataFromRequest( const ResourceRequest& request ) const;

		const Resource*						loadGenericResource( const char* pFileName, fourcc type, crc32 resourceKey );
		const ResourceRequest&				beginGenericResourceLoading( const char* pFileName, fourcc type, crc32 resourceKey );

		void								traceResourceLoadResult( ResourceLoaderResult result, const char* pFileName, crc32 resourceKey, fourcc resourceType );

		void								threadEntry( const Thread& thread );
		static int							staticThreadEntry( const Thread& thread );

	};
}

#include "../../../source/resourcemanager.inl"

#endif // TIKI_RESOURCEMANAGER_HPP
