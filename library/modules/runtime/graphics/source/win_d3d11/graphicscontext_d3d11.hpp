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
			pDevice						= nullptr;
			pContext					= nullptr;
		}

		TGDevice*				pDevice;
		TGContext*				pContext;
	};
}

#endif // TIKI_GRAPHICSCONTEXT_D3D11_HPP
