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

		ID3D11BlendState* pBlendState;
	};

	struct DepthStencilStatePlatformData
	{
		DepthStencilStatePlatformData()
		{
			stencilRef			= 0u;
			pDepthStencilState	= nullptr;
		}

		uint						stencilRef;
		ID3D11DepthStencilState*	pDepthStencilState;
	};

	struct RasterizerStatePlatformData
	{
		RasterizerStatePlatformData()
		{
			pRasterizerState = nullptr;
		}

		ID3D11RasterizerState* pRasterizerState;
	};

	struct SamplerStatePlatformData
	{
		SamplerStatePlatformData()
		{
			pSamplerState = nullptr;
		}

		ID3D11SamplerState* pSamplerState;
	};

	struct VertexInputBindingPlatformData
	{
		VertexInputBindingPlatformData()
		{
			pInputLayout = nullptr;
		}

		ID3D11InputLayout* pInputLayout;
	};
}

#endif // __TIKI_GRAPHICSSTATES_D3D11_HPP_INCLUDED__
