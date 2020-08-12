#ifndef TIKI_RENDERTARGET_D3D11_HPP__
#define TIKI_RENDERTARGET_D3D11_HPP__

#include "tiki/base/types.hpp"
#include "tiki/graphics/graphicssystemlimits.hpp"

#include "graphicstypes_d3d11.hpp"

namespace tiki
{
	struct RenderTargetPlatformData
	{
		RenderTargetPlatformData()
		{
			for (uint i = 0u; i < TIKI_COUNT( pColorViews ); ++i)
			{
				pColorViews[ i ] = nullptr;
			}

			pDepthView = nullptr;
		}

		ID3D11RenderTargetView*		pColorViews[ GraphicsSystemLimits_RenderTargetSlots ];
		ID3D11DepthStencilView*		pDepthView;
	};
}

#endif // TIKI_RENDERTARGET_D3D11_HPP__
