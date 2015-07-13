#pragma once
#ifndef TIKI_GRAPHICSCONTEXT_D3D11_HPP
#define TIKI_GRAPHICSCONTEXT_D3D11_HPP

#include "graphicstypes_d3d11.hpp"

namespace tiki
{
	struct GraphicsContextPlatformData
	{
		GraphicsContextPlatformData()
		{
			pDevice		= nullptr;
			pContext	= nullptr;
		}

		ID3D11Device*			pDevice;
		ID3D11DeviceContext*	pContext;
	};
}

#endif // TIKI_GRAPHICSCONTEXT_D3D11_HPP
