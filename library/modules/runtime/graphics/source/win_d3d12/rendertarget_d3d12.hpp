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
			pColorHeap = nullptr;
			pDepthHeap = nullptr;
		}

		ID3D12DescriptorHeap*		pColorHeap;
		ID3D12DescriptorHeap*		pDepthHeap;
	};
}

#endif // TIKI_RENDERTARGET_D3D11_HPP__
