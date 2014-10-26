#ifndef TIKI_GRAPHICSSTATES_D3D12_HPP__INCLUDED__
#define TIKI_GRAPHICSSTATES_D3D12_HPP__INCLUDED__

#include "graphicstypes_d3d12.hpp"

namespace tiki
{
	struct BlendStatePlatformData
	{
		BlendStatePlatformData()
		{
			pBlendState = nullptr;
		}

		void*	pBlendState;
	};

	struct DepthStencilStatePlatformData
	{
		DepthStencilStatePlatformData()
		{
			stencilRef = 0u;
			pDepthStencilState = nullptr;
		}

		uint					stencilRef;
		TGDepthStencilState*	pDepthStencilState;
	};

	struct RasterizerStatePlatformData
	{
		RasterizerStatePlatformData()
		{
			pRasterizerState = nullptr;
		}

		TGRasterizerState*	pRasterizerState;
	};

	struct SamplerStatePlatformData
	{
		SamplerStatePlatformData()
		{
			pSamplerState = nullptr;
		}

		TGSamplerState* pSamplerState;
	};

	struct VertexInputBindingPlatformData
	{
		VertexInputBindingPlatformData()
		{
			pInputLayout = nullptr;
		}

		TGInputLayout*	pInputLayout;
	};
}

#endif // TIKI_GRAPHICSSTATES_D3D12_HPP__INCLUDED__
