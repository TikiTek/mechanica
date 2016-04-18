#pragma once
#ifndef __TIKI_GRAPHICSSYSTEM_INTERNAL_VULKAN_HPP_INCLUDED__
#define __TIKI_GRAPHICSSYSTEM_INTERNAL_VULKAN_HPP_INCLUDED__

#if TIKI_ENABLED( TIKI_PLATFORM_WIN)
#	define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <vulkan/vulkan.h>

#include "graphicstypes_vulkan.hpp"

namespace tiki
{
    class GraphicsSystem;
	class ShaderLinker;
}

#endif // __TIKI_GRAPHICSSYSTEM_INTERNAL_VULKAN_HPP_INCLUDED__
