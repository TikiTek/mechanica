#ifndef TIKI_RENDERTARGET_D3D11_HPP__
#define TIKI_RENDERTARGET_D3D11_HPP__


#include "tiki/graphicsbase/graphissystemlimits.hpp"

#include "../../../../source/win_d3d11/graphicstypes_d3d11.hpp"

namespace tiki
{
	struct RenderTargetPlatformData
	{
		TGRenderTargetView*		m_pColorViews[ GraphicsSystemLimits_RenderTargetSlots ];
		TGDepthStencilView*		m_pDepthView;
	};
}

#endif // TIKI_RENDERTARGET_D3D11_HPP__
