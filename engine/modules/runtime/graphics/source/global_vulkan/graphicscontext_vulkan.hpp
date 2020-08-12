#pragma once
#ifndef __TIKI_GRAPHICSCONTEXT_VULKAN_HPP_INCLUDED__
#define __TIKI_GRAPHICSCONTEXT_VULKAN_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#include "graphicstypes_vulkan.hpp"
#include "graphicssystem_internal_vulkan.hpp"

namespace tiki
{
	class VertexInputBinding;
	struct ShaderPlatformData;

	struct GraphicsContextPlatformData
	{
		GraphicsContextPlatformData()
		{
		}
	};
}

#endif // __TIKI_GRAPHICSCONTEXT_VULKAN_HPP_INCLUDED__
