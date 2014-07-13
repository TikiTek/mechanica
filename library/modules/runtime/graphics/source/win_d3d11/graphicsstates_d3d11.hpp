#pragma once
#ifndef __TIKI_GRAPHICSSTATES_D3D11_HPP_INCLUDED__
#define __TIKI_GRAPHICSSTATES_D3D11_HPP_INCLUDED__

#include "graphicstypes_d3d11.hpp"

namespace tiki
{
	struct BlendStatePlatformData
	{
		BlendStatePlatformData()
		{
			pBlendState = nullptr;
		}

		TGBlendState*	pBlendState;
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

#endif // __TIKI_GRAPHICSSTATES_D3D11_HPP_INCLUDED__
