#pragma once

#include "tiki/base/types.hpp"
#include "tiki/graphics/graphics_system_limits.hpp"

#include "graphics_types_d3d11.hpp"

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
