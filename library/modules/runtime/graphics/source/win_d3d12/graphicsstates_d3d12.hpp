#ifndef TIKI_GRAPHICSSTATES_D3D12_HPP__INCLUDED__
#define TIKI_GRAPHICSSTATES_D3D12_HPP__INCLUDED__

#include "tiki/base/memory.hpp"
#include "tiki/graphics/graphicssystemlimits.hpp"

#include <d3d12.h>

namespace tiki
{
	struct BlendStatePlatformData
	{
		BlendStatePlatformData()
		{
			isCreated = false;
		}

		bool				isCreated;
		D3D12_BLEND_DESC	blendDesc;
	};

	struct DepthStencilStatePlatformData
	{
		DepthStencilStatePlatformData()
		{
			isCreated	= false;
			stencilRef	= 0u;
		}

		bool						isCreated;
		uint						stencilRef;
		D3D12_DEPTH_STENCIL_DESC	depthStencilDesc;
	};

	struct RasterizerStatePlatformData
	{
		RasterizerStatePlatformData()
		{
			isCreated = false;
		}

		bool					isCreated;
		D3D12_RASTERIZER_DESC	rasterizerDesc;
	};

	struct SamplerStatePlatformData
	{
		SamplerStatePlatformData()
		{
			pDescriptorHeap = nullptr;
		}

		ID3D12DescriptorHeap*	pDescriptorHeap;
	};

	struct VertexInputBindingPlatformData
	{
		VertexInputBindingPlatformData()
		{
			isCreated = false;

			memory::zero( aInputElements, sizeof(aInputElements) );

			inputLayoutDesc.NumElements			= 0u;
			inputLayoutDesc.pInputElementDescs	= nullptr;
		}

		bool						isCreated;
		D3D12_INPUT_ELEMENT_DESC	aInputElements[ GraphicsSystemLimits_MaxVertexAttributes ];
		D3D12_INPUT_LAYOUT_DESC		inputLayoutDesc;
	};
}

#endif // TIKI_GRAPHICSSTATES_D3D12_HPP__INCLUDED__
