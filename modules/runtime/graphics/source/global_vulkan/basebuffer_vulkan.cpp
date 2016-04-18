
#include "tiki/graphics/basebuffer.hpp"

#include "graphicssystem_internal_vulkan.hpp"

namespace tiki
{
	BaseBuffer::BaseBuffer()
	{
	}

	BaseBuffer::~BaseBuffer()
	{
	}

	bool BaseBuffer::create( GraphicsSystem& graphicsSystem, size_t size, bool dynamic, BaseBufferTypes binding, const void* pInitData, const char* pDebugName )
	{
		return false;
	}

	void BaseBuffer::dispose( GraphicsSystem& graphicsSystem )
	{
	}
}