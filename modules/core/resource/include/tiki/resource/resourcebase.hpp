#pragma once
#ifndef __TIKI_RESOURCEBASE_HPP_INCLUDED__
#define __TIKI_RESOURCEBASE_HPP_INCLUDED__

namespace tiki
{
	class Resource;

	struct ResourceId
	{
		ResourceId()
		{
			key = TIKI_INVALID_CRC32;
		}

		crc32	key;
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		string	fileName;
#endif
	};

	struct ResourceSectionData
	{
		ResourceSectionData()
		{
			ppLinkedResources	= nullptr;
			ppStringPointers	= nullptr;
			ppSectorPointers	= nullptr;
			sectorCount			= 0u;
			stringCount			= 0u;
			linkCount			= 0u;
		}

		const Resource**	ppLinkedResources;
		char**				ppStringPointers;
		void**				ppSectorPointers;
		uint				sectorCount;
		uint				stringCount;
		uint				linkCount;
	};

	struct ResourceInitData
	{
		const void*	pData;
		uint		size;
	};

	typedef Resource*(*CreateResourceFunction)();
	typedef void(*DisposeResourceFunction)( Resource* );

	struct FactoryContext
	{
		CreateResourceFunction		pCreateResource;
		DisposeResourceFunction		pDisposeResource;
	};

	template<class T>
	struct FactoryContextGenericBase : public FactoryContext
	{
		FactoryContextGenericBase()
		{
			pCreateResource		= factoryContextGenericCreateResourceFunction;
			pDisposeResource	= factoryContextGenericDisposeResourceFunction;
		}

	private:

		static Resource* factoryContextGenericCreateResourceFunction()
		{
			return TIKI_NEW( T );
		}

		static void factoryContextGenericDisposeResourceFunction( Resource* pResource )
		{
			TIKI_DELETE( static_cast< T* >( pResource ) );
		}
	};
}

#endif // __TIKI_RESOURCEBASE_HPP_INCLUDED__
