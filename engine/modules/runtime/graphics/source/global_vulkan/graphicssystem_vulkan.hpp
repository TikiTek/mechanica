#pragma once
#ifndef __TIKI_GRAPHICSSYSTEM_VULKAN_HPP_INCLUDED__
#define __TIKI_GRAPHICSSYSTEM_VULKAN_HPP_INCLUDED__

#include "tiki/base/platform.hpp"
#include "tiki/graphics/texturedata.hpp"

#include "graphicstypes_vulkan.hpp"

namespace tiki
{
	enum
	{
		GraphicsSystemLimits_MaxProgramCount = 1024u
	};

	struct GraphicsSystemPlatformData
	{
		GraphicsSystemPlatformData()
		{
			instance		= nullptr;
			device			= nullptr;

			windowHandle	= InvalidWindowHandle;
		}

		VkInstance			instance;
		VkDevice			device;

		WindowHandle		windowHandle;
	};
}

#endif // __TIKI_GRAPHICSSYSTEM_VULKAN_HPP_INCLUDED__
