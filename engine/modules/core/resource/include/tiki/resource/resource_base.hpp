#pragma once

#include "tiki/resource/resource_definition.hpp"

namespace tiki
{
	class Resource;

	struct ResourceId
	{
		crc32				nameCrc;
		fourcc				type;
	};

	inline bool operator==( ResourceId lhs, ResourceId rhs )
	{
		return lhs.nameCrc == rhs.nameCrc && lhs.type == rhs.type;
	}

	inline bool operator<( ResourceId lhs, ResourceId rhs )
	{
		return (lhs.nameCrc ^ lhs.type) < (rhs.nameCrc ^ rhs.type);
	}

	struct ResourceName
	{
		ResourceName()
		{
			key = TIKI_INVALID_CRC32;
		}

		crc32				key;
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		DynamicString		fileName;
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
		FactoryContext( FlagMask8< ResourceDefinitionFeature > inResourceDefinitionFeatures )
			: resourceDefinitionFeatures( inResourceDefinitionFeatures )
		{
		}

		CreateResourceFunction					pCreateResource;
		DisposeResourceFunction					pDisposeResource;

		FlagMask8< ResourceDefinitionFeature >	resourceDefinitionFeatures;
	};

	template<class T>
	struct FactoryContextGenericBase : public FactoryContext
	{
		FactoryContextGenericBase( FlagMask8< ResourceDefinitionFeature > resourceDefinitionFeatures )
			: FactoryContext( resourceDefinitionFeatures )
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
