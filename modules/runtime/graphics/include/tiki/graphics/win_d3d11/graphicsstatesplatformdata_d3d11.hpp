#pragma once
#ifndef __TIKI_GRAPHICSSTATESPLATFORMDATA_D3D11_HPP_INCLUDED__
#define __TIKI_GRAPHICSSTATESPLATFORMDATA_D3D11_HPP_INCLUDED__

#include "../../../../source/win_d3d11/graphicstypes_d3d11.hpp"

namespace tiki
{
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

#endif // __TIKI_GRAPHICSSTATESPLATFORMDATA_D3D11_HPP_INCLUDED__
