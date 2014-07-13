#pragma once
#ifndef TIKI_GRAPHICSSYSTEM_D3D11_HPP
#define TIKI_GRAPHICSSYSTEM_D3D11_HPP

#include "graphicstypes_d3d11.hpp"

namespace tiki
{
	struct GraphicsSystemPlatformData
	{
		GraphicsSystemPlatformData()
		{
			pDevice						= nullptr;
			pContext					= nullptr;
			pSwapChain					= nullptr;
			pBackBufferTargetView		= nullptr;

			pDepthStencilBuffer			= nullptr;
			pDepthStencilView			= nullptr;
		}

		TGDevice*				pDevice;
		TGContext*				pContext;
		TGSwapChain*			pSwapChain;
		TGRenderTargetView*		pBackBufferTargetView;

		TGTexture2D*			pDepthStencilBuffer;
		TGDepthStencilView*		pDepthStencilView;
	};
}

#endif // TIKI_GRAPHICSSYSTEM_D3D11_HPP
