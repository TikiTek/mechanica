#ifndef TIKI_GRAPHICSHANDLES_HPP
#define TIKI_GRAPHICSHANDLES_HPP

#include "tiki/base/types.hpp"

#include <dxgi.h>
#include <d3d11.h>

namespace tiki
{
	class GraphicsSystem;

	struct GraphicsHandles 
	{
		GraphicsHandles()
		{
			pDevice						= nullptr;
			pContext					= nullptr;
			pSwapChain					= nullptr;
			pBackBufferTargetView		= nullptr;

			pDepthStencilBuffer			= nullptr;
			pDepthStencilView			= nullptr;
			pDepthStencilState			= nullptr;
			pDepthStencilStateNoDepth	= nullptr;

			pBlendStateAlphaOn			= nullptr;
			pBlendStateAlphaOff			= nullptr;

			pRasterState				= nullptr;
		}

		ID3D11Device*				pDevice;
		ID3D11DeviceContext*		pContext;
		IDXGISwapChain*				pSwapChain;
		ID3D11RenderTargetView*		pBackBufferTargetView;

		ID3D11Texture2D*			pDepthStencilBuffer;
		ID3D11DepthStencilView*		pDepthStencilView;
		ID3D11DepthStencilState*	pDepthStencilState;
		ID3D11DepthStencilState*	pDepthStencilStateNoDepth;

		ID3D11BlendState*			pBlendStateAlphaOn;
		ID3D11BlendState*			pBlendStateAlphaOff;

		ID3D11RasterizerState*		pRasterState;
	};

	GraphicsHandles* getHandles( GraphicsSystem& graphicsSystem );
}

#endif // TIKI_GRAPHICSHANDLES_HPP