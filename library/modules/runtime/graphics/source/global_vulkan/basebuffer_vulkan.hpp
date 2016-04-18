#pragma once
#ifndef __TIKI_BASEBUFFER_VULKAN_HPP_INCLUDED__
#define __TIKI_BASEBUFFER_VULKAN_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#include "graphicstypes_vulkan.hpp"

namespace tiki
{
	class GraphicsSystem;

	class BaseBuffer
	{
		friend class GraphicsContext;

	protected:

								BaseBuffer();
								~BaseBuffer();

		bool					create( GraphicsSystem& graphicsSystem, size_t size, bool dynamic, BaseBufferTypes binding, const void* pInitData, const char* pDebugName );
		void					dispose( GraphicsSystem& graphicsSystem );
		
	private:

	};
}

#endif // __TIKI_BASEBUFFER_VULKAN_HPP_INCLUDED__
