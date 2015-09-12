#pragma once
#ifndef __TIKI_RESOURCELOADER_HPP_INCLUDED__
#define __TIKI_RESOURCELOADER_HPP_INCLUDED__

#include "tiki/base/sortedsizedmap.hpp"
#include "tiki/base/types.hpp"
#include "tiki/base/zoneallocator.hpp"
#include "tiki/resource/resourcedefinition.hpp"
#include "tiki/resource/resourcenamemapper.hpp"

namespace tiki
{
	class DataStream;
	class FileSystem;
	class Resource;
	class ResourceStorage;
	struct FactoryContext;
	struct ResourceHeader;
	struct ResourceInitData;
	struct ResourceLoaderContext;
	struct ResourceSectionData;

	enum ResourceLoaderResult
	{
		ResourceLoaderResult_Success,

		ResourceLoaderResult_CouldNotAccessFile,
		ResourceLoaderResult_CouldNotCreateResource,
		ResourceLoaderResult_CouldNotInitialize,
		ResourceLoaderResult_FileNotFound,
		ResourceLoaderResult_OutOfMemory,
		ResourceLoaderResult_ResourceNotFound,
		ResourceLoaderResult_UnknownError,
		ResourceLoaderResult_WrongFileFormat,
		ResourceLoaderResult_WrongResourceType
	};

	class ResourceLoader
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ResourceLoader );

	public:

			void					create( FileSystem* pFileSystem, ResourceStorage* pStorage );
			void					dispose();

			void					registerResourceType( fourcc type, const FactoryContext& factoryContext );
			void					unregisterResourceType( fourcc type );

			ResourceLoaderResult	loadResource( Resource** ppTargetResource, crc32 crcFileName, crc32 resourceKey, fourcc resourceType );
			void					unloadResource( const Resource* pResource, fourcc resourceType );

			ResourceLoaderResult	reloadResource( Resource* pResource, crc32 crcFileName, crc32 resourceKey, fourcc resourceType );
			void					reloadResourceMapping();
			
	private:

		enum
		{
			MaxFactoryCount					= 32u,
			InitializationDataBufferSize	= 1u * 1024u * 1024u
		};


		typedef SortedSizedMap< fourcc, const FactoryContext* > FactoryMap;

		FileSystem*				m_pFileSystem;
		ResourceStorage*		m_pStorage;
		ResourceNameMapper		m_nameMapper;
		FactoryMap				m_factories;

		ResourceDefinition		m_definition;

		ZoneAllocator			m_bufferAllocator;
		
		const FactoryContext*	findFactory( fourcc resourceType ) const;

		ResourceLoaderResult	initializeLoaderContext( ResourceLoaderContext& context, crc32 crcFileName, crc32 resourceKey, fourcc resourceType );
		ResourceLoaderResult	createResource( ResourceLoaderContext& context );
		ResourceLoaderResult	loadResourceData( ResourceLoaderContext& context );
		ResourceLoaderResult	initializeResource( ResourceLoaderContext& context );
		void					cancelOperation( ResourceLoaderContext& context );

		void					disposeResource( Resource* pResource, fourcc resourceType, bool freeResourceObject );

	};
}

#endif // __TIKI_RESOURCELOADER_HPP_INCLUDED__
