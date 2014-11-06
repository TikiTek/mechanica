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
			pColorHandle = nullptr;
			pDepthHandle = nullptr;
		}

		const D3D12_CPU_DESCRIPTOR_HANDLE*	pColorHandle;
		const D3D12_CPU_DESCRIPTOR_HANDLE*	pDepthHandle;
	};
}

#endif // TIKI_RENDERTARGET_D3D11_HPP__
