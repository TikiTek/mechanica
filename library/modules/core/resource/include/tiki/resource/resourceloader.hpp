#pragma once
#ifndef __TIKI_RESOURCELOADER_HPP_INCLUDED__
#define __TIKI_RESOURCELOADER_HPP_INCLUDED__

#include "tiki/base/zoneallocator.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class DataStream;
	class FactoryBase;
	class FileSystem;
	class Resource;
	struct ResourceHeader;

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

			void					create( FileSystem* pFileSystem );
			void					dispose();

			ResourceLoaderResult	loadResource( const Resource** ppTargetResource, uint targetCapacity, const char* pFileName, crc32 resourceKey, const FactoryBase* pFactory );
			void					unloadResource( const Resource* pResource, const FactoryBase* pFactory );
			
	private:

		enum
		{
			InitializationDataBufferSize = 1u * 1024u * 1024u
		};

		struct Context
		{
			DataStream*		pStream;
			FactoryBase*	pFactory;

			Resource*		pResource;

			uint			resourceCount;
			ResourceHeader*	pResourceHeader;
		};

		FileSystem*				m_pFileSystem;

		ZoneAllocator			m_bufferAllocator;
		
		void					cancelOperation( Context& context ) const;


	};
}

#endif // __TIKI_RESOURCELOADER_HPP_INCLUDED__
