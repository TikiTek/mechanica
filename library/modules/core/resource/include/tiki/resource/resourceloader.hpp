#pragma once
#ifndef __TIKI_RESOURCELOADER_HPP_INCLUDED__
#define __TIKI_RESOURCELOADER_HPP_INCLUDED__

#include "tiki/base/sortedsizedmap.hpp"
#include "tiki/base/types.hpp"
#include "tiki/base/zoneallocator.hpp"

namespace tiki
{
	class DataStream;
	class FactoryBase;
	class FileSystem;
	class Resource;
	class ResourceStorage;
	struct ResourceLoaderContext;

	enum ResourceLoaderResult
	{
		ResourceLoaderResult_Success,

		ResourceLoaderResult_CouldNotAccessFile,
		ResourceLoaderResult_CouldNotCreateResource,
		ResourceLoaderResult_CouldNotInitialize,
		ResourceLoaderResult_FileNotFound,
		ResourceLoaderResult_OutOfMemory,
		ResourceLoaderResult_UnknownError,
		ResourceLoaderResult_WrongFileFormat
	};

	class ResourceLoader
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ResourceLoader );

	public:

			void					create( FileSystem* pFileSystem, ResourceStorage* pStorage );
			void					dispose();

			void					registerFactory( FactoryBase& factory );
			void					unregisterFactory( FactoryBase& factory );

			ResourceLoaderResult	loadResource( const Resource** ppTargetResource, const char* pFileName, crc32 resourceKey );
			void					unloadResource( const Resource* pResource );
			
	private:

		enum
		{
			MaxFactoryCount					= 32u,
			InitializationDataBufferSize	= 1u * 1024u * 1024u
		};


		typedef SortedSizedMap< fourcc, FactoryBase* > FactoryMap;

		FileSystem*			m_pFileSystem;
		ResourceStorage*	m_pStorage;

		FactoryMap			m_factories;
		ZoneAllocator		m_bufferAllocator;
		
		FactoryBase*			findFactory( fourcc resourceType ) const;

		ResourceLoaderResult	loadResource( ResourceLoaderContext& context, uint resourceIndex );
		void					cancelOperation( ResourceLoaderContext& context );


	};
}

#endif // __TIKI_RESOURCELOADER_HPP_INCLUDED__
