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
			key = InvalidCrc32;
		}

		crc32	key;
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		string	fileName;
#endif
	};

	struct ResourceSectorData
	{
		ResourceSectorData()
		{
			ppLinkedResources	= nullptr;
			ppStringPointers	= nullptr;
			ppSectorPointers	= nullptr;
			sectorCount			= 0u;
			stringCount			= 0u;
			linkCount			= 0u;
		}

		Resource**	ppLinkedResources;
		char**		ppStringPointers;
		void**		ppSectorPointers;
		uint		sectorCount;
		uint		stringCount;
		uint		linkCount;
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
			return TIKI_NEW T();
		}

		static void factoryContextGenericDisposeResourceFunction( Resource* pResource )
		{
			TIKI_DEL static_cast< T* >( pResource );
		}

	};
}

#endif // __TIKI_RESOURCEBASE_HPP_INCLUDED__
