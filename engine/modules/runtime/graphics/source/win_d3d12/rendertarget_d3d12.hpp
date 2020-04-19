#ifndef TIKI_RENDERTARGET_D3D11_HPP__
#define TIKI_RENDERTARGET_D3D11_HPP__

#include "tiki/base/types.hpp"
#include "tiki/graphics/graphicssystemlimits.hpp"

#include "graphicstypes_d3d12.hpp"

namespace tiki
{
	struct RenderTargetPlatformData
	{
		RenderTargetPlatformData()
		{
			for( uintreg i = 0u; i < TIKI_COUNT( colorHandles ); ++i )
			{
				colorHandles[ i ] = InvalidDescriptorHandle;
			}
			depthHandle = InvalidDescriptorHandle;
		}

		DescriptorHandle		colorHandles[ GraphicsSystemLimits_RenderTargetSlots ];
		DescriptorHandle		depthHandle;
	};
}

#endif // TIKI_RENDERTARGET_D3D11_HPP__
