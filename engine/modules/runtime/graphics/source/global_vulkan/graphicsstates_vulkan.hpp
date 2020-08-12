#pragma once
#ifndef __TIKI_GRAPHICSSTATES_VULKAN_HPP_INCLUDED__
#define __TIKI_GRAPHICSSTATES_VULKAN_HPP_INCLUDED__

#include "tiki/graphics/graphicssystemlimits.hpp"

#include "graphicssystem_internal_vulkan.hpp"

namespace tiki
{
	struct BlendStatePlatformData
	{
		BlendStatePlatformData()
		{
		}
	};

	struct DepthStencilStatePlatformData
	{
		struct StencilPlatformData
		{
			StencilPlatformData()
			{
			}
		};

		DepthStencilStatePlatformData()
		{
		}
	};

	struct RasterizerStatePlatformData
	{
		RasterizerStatePlatformData()
		{
		}
	};

	struct SamplerStatePlatformData
	{
		SamplerStatePlatformData()
		{
		}
	};

	struct VertexInputBindingPlatformData
	{
		struct VertexAttribute
		{
		};

		VertexInputBindingPlatformData()
		{
		}
	};
}

#endif // __TIKI_GRAPHICSSTATES_VULKAN_HPP_INCLUDED__
