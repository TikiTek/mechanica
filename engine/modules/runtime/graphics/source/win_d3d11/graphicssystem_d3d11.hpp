#pragma once
#ifndef TIKI_GRAPHICSSYSTEM_D3D11_HPP
#define TIKI_GRAPHICSSYSTEM_D3D11_HPP

#include "graphicstypes_d3d11.hpp"

namespace tiki
{
	class GraphicsSystem;

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

		ID3D11Device*			pDevice;
		ID3D11DeviceContext*	pContext;
		IDXGISwapChain*			pSwapChain;
		ID3D11RenderTargetView*	pBackBufferTargetView;

		ID3D11Texture2D*		pDepthStencilBuffer;
		ID3D11DepthStencilView*	pDepthStencilView;
	};

	class GraphicsSystemPlatform
	{
	public:

		static ID3D11Device*		getDevice( GraphicsSystem& graphicsSystem );
		static ID3D11DeviceContext*	getContext( GraphicsSystem& graphicsSystem );

	};
}

#endif // TIKI_GRAPHICSSYSTEM_D3D11_HPP
