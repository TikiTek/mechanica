#pragma once
#ifndef TIKI_RESOURCEMANAGER_HPP
#define TIKI_RESOURCEMANAGER_HPP

#include "tiki/base/dynamic_string.hpp"
#include "tiki/container/sizedarray.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/pool.hpp"
#include "tiki/resource/resourceloader.hpp"
#include "tiki/resource/resourcestorage.hpp"
#include "tiki/threading/mutex.hpp"
#include "tiki/threading/thread.hpp"

#if !defined( TIKI_RESOUCE_ENABLE_CONVERTER )
#	define TIKI_RESOUCE_ENABLE_CONVERTER TIKI_OFF
#endif

namespace tiki
{
	class FactoryBase;
	class Resource;
	class ResourceRequest;
	struct ResourceId;
#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
	class AssetConverterInterface;
	class Project;
#endif

	struct ResourceManagerParameters
	{
		ResourceManagerParameters()
		{
			maxResourceCount		= 1000u;
			maxRequestCount			= 128u;

			enableMultiThreading	= false;

			pFileSystem				= nullptr;
		}

		uint			maxResourceCount;
		uint			maxRequestCount;

		bool			enableMultiThreading;

		FileSystem*		pFileSystem;

#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		const Project*	pProject = nullptr;
#endif
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

#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		AssetConverterInterface*					getAssetConverter() const { return m_pAssetConverter; }
#endif

		template< typename T >
		TIKI_FORCE_INLINE const T*					loadResource( const char* pFileName );
		template< typename T >
		void										unloadResource( const T*& pResource );

		template< typename T >
		TIKI_FORCE_INLINE const ResourceRequest&	beginResourceLoading( const char* pFileName );
		void										endResourceLoading( const ResourceRequest& request );

	private:

		ResourceLoader						m_resourceLoader;
		ResourceStorage						m_resourceStorage;

		Pool< ResourceRequest >				m_resourceRequests;
		LinkedList< ResourceRequest >		m_runningRequests;

		Thread								m_loadingThread;
		Mutex								m_loadingMutex;

#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		AssetConverterInterface*			m_pAssetConverter;
#endif

		const Resource*						loadGenericResource( const char* pFileName, fourcc type, crc32 resourceKey );
		const ResourceRequest&				beginGenericResourceLoading( const char* pFileName, fourcc type, crc32 resourceKey );
		void								unloadGenericResource( const Resource** ppResource );

		void								traceResourceLoadResult( ResourceLoaderResult result, const char* pFileName, crc32 resourceKey, fourcc resourceType );

		void								threadEntry( const Thread& thread );
		static int							staticThreadEntry( const Thread& thread );

		void								updateResourceLoading( ResourceRequest* pData );

	};
}

#include "../../../source/resourcemanager.inl"

#endif // TIKI_RESOURCEMANAGER_HPP
