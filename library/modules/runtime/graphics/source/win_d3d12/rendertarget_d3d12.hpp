#ifndef TIKI_RENDERTARGET_D3D11_HPP__
#define TIKI_RENDERTARGET_D3D11_HPP__

#include "tiki/base/types.hpp"
#include "tiki/graphics/graphissystemlimits.hpp"

#include "graphicstypes_d3d12.hpp"

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

		ID3D12Resource*		pColorViews[ GraphicsSystemLimits_RenderTargetSlots ];
		ID3D12Resource*		pDepthView;
	};
}

#endif // TIKI_RENDERTARGET_D3D11_HPP__
