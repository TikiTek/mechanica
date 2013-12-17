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
			ppSectorPointers	= nullptr;
			sectorCount			= 0u;
		}

		void**	ppSectorPointers;
		uint	sectorCount;
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
}

#endif // __TIKI_RESOURCEBASE_HPP_INCLUDED__
