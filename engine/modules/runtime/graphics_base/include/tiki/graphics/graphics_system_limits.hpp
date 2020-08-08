#pragma once

namespace tiki
{
	enum GraphicsSystemLimits
	{
		GraphicsSystemLimits_RenderTargetSlots			= 8u,
		GraphicsSystemLimits_RenderPassStackDepth		= 4u,
		GraphicsSystemLimits_MaxInputStreams			= 4u,
		GraphicsSystemLimits_VertexShaderTextureSlots	= 4u,
		GraphicsSystemLimits_PixelShaderTextureSlots	= 16u,
		GraphicsSystemLimits_VertexShaderConstantSlots	= 4u,
		GraphicsSystemLimits_PixelShaderConstantSlots	= 4u,
		GraphicsSystemLimits_MaxVertexAttributes		= 16u
	};
}
