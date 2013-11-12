#pragma once
#ifndef __TIKI_RESOURCELOADER_HPP_INCLUDED__
#define __TIKI_RESOURCELOADER_HPP_INCLUDED__

#include "tiki/base/array.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class FactoryBase;
	class Resource;
	class Stream;

	enum ResourceLoadResult
	{
		ResourceLoadResult_Success,
		ResourceLoadResult_FileNotFound,
		ResourceLoadResult_CouldNotInitialize,
		ResourceLoadResult_UnknownError
	};

	class ResourceLoader
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ResourceLoader );

	public:

			void				create();
			void				dispose();

			ResourceLoadResult	loadResource( Resource* pResource, FactoryBase* pFactory, Stream* pFileStream );
			void				unloadResource( Resource* pResource );
			
	private:

		Array< uint8 >			m_initDataBuffer;

	};
}

#endif // __TIKI_RESOURCELOADER_HPP_INCLUDED__
