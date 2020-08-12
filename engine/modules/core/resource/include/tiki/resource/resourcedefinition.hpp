#pragma once
#ifndef TIKI_RESOURCEDEFINITION_HPP_INCLUDED__
#define TIKI_RESOURCEDEFINITION_HPP_INCLUDED__

#include "tiki/base/platform.hpp"

namespace tiki
{
	enum ResourceDefinitionMask
	{
		ResourceDefinitionMask_Platform_FirstBit	= 0,
		ResourceDefinitionMask_Platform_BitCount	= 2,
		ResourceDefinitionMask_Platform_BitMask		= 0x3 << ResourceDefinitionMask_Platform_FirstBit,
		ResourceDefinitionMask_GraphicsApi_FirstBit	= 3,
		ResourceDefinitionMask_GraphicsApi_BitCount	= 2,	
		ResourceDefinitionMask_GraphicsApi_BitMask	= 0x3 << ResourceDefinitionMask_GraphicsApi_FirstBit,
	};
	TIKI_COMPILETIME_ASSERT( 1u << ResourceDefinitionMask_Platform_BitCount >= PlatformType_Count );
	TIKI_COMPILETIME_ASSERT( 1u << ResourceDefinitionMask_GraphicsApi_BitCount >= GraphicsApi_Count );

	struct ResourceDefinition
	{
		uint32 definitionMask;

		ResourceDefinition()
		{
			definitionMask = 0;
		}

		TIKI_INLINE void applyHostValues()
		{
			setPlatformType( platform::getHostPlatform() );
			setGraphicsApi( platform::getHostGraphicsApi() );
		}

		TIKI_INLINE PlatformType getPlatformType() const
		{
			return (PlatformType)((definitionMask & ResourceDefinitionMask_Platform_BitMask) >> ResourceDefinitionMask_Platform_FirstBit);
		}

		TIKI_INLINE void setPlatformType(PlatformType val)
		{
			definitionMask = definitionMask & ~ResourceDefinitionMask_Platform_BitMask;
			definitionMask |= val << ResourceDefinitionMask_Platform_FirstBit;
		}

		TIKI_INLINE GraphicsApi getGraphicsApi() const
		{
			return (GraphicsApi)((definitionMask & ResourceDefinitionMask_GraphicsApi_BitMask) >> ResourceDefinitionMask_GraphicsApi_FirstBit);
		}

		TIKI_INLINE void setGraphicsApi(GraphicsApi val)
		{
			definitionMask = definitionMask & ~ResourceDefinitionMask_GraphicsApi_BitMask;
			definitionMask |= val << ResourceDefinitionMask_GraphicsApi_FirstBit;
		}
	};
}

#endif // TIKI_RESOURCEDEFINITION_HPP_INCLUDED__
