
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

	bool RenderTarget::create( GraphicsSystem& graphicsSystem, uintreg width, uintreg height, const RenderTargetBuffer* pColorBuffers, uintreg colorBufferCount, const RenderTargetBuffer* pDepthBuffer )
	{
		return false;
	}

	void RenderTarget::dispose( GraphicsSystem& graphicsSystem )
	{
	}
}
