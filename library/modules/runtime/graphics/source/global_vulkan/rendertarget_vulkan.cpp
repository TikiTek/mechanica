
#include "tiki/graphics/rendertarget.hpp"

#include "tiki/graphics/texturedata.hpp"

#include "graphicssystem_internal_vulkan.hpp"

namespace tiki
{
	RenderTarget::RenderTarget()
		: m_width( 0u ), m_height( 0u ), m_colorBufferCount( 0u )
	{
	}

	RenderTarget::~RenderTarget()
	{
	}

	bool RenderTarget::create( GraphicsSystem& graphicsSystem, size_t width, size_t height, const RenderTargetBuffer* pColorBuffers, size_t colorBufferCount, const RenderTargetBuffer* pDepthBuffer )
	{
		return false;
	}

	void RenderTarget::dispose( GraphicsSystem& graphicsSystem )
	{
	}
}
