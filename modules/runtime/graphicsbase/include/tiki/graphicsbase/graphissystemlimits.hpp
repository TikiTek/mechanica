#pragma once
#ifndef TIKI_GRAPHISSYSTEMLIMITS_HPP__INCLUDED
#define TIKI_GRAPHISSYSTEMLIMITS_HPP__INCLUDED

namespace tiki
{
	enum GraphicsSystemLimits
	{
		GraphicsSystemLimits_RenderTargetSlots			= 8u,
		GraphicsSystemLimits_MaxInputStreams			= 4u,
		GraphicsSystemLimits_VertexShaderTextureSlots	= 16u,	
		GraphicsSystemLimits_PixelShaderTextureSlots	= 16u,	
		GraphicsSystemLimits_VertexShaderConstantSlots	= 4u,	
		GraphicsSystemLimits_PixelShaderConstantSlots	= 4u,	
	};
}

#endif // TIKI_GRAPHISSYSTEMLIMITS_HPP__INCLUDED
