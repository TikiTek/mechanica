
#include "tiki/graphics/graphicssystem.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/graphics/buffer.hpp"
#include "tiki/graphicsbase/color.hpp"
#include "tiki/graphicsresources/material.hpp"

#include "graphicshandles.hpp"

namespace tiki
{
	template<class T>
	void safeRelease( T** ppObject )
	{
		if ( *ppObject != nullptr )
		{
			(*ppObject)->Release();
			*ppObject = nullptr;
		}
	}

	GraphicsHandles* getHandles( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_pHandles;
	}

	bool initSwapChain( GraphicsHandles& handles, const GraphicsSystemParameters& params, const uint2& backBufferSize )
	{
		TIKI_DECLARE_STACKANDZERO( DXGI_SWAP_CHAIN_DESC, swapDesc );
		swapDesc.BufferCount						= 2;
		swapDesc.BufferDesc.Format					= DXGI_FORMAT_B8G8R8A8_UNORM;
		swapDesc.BufferDesc.Width					= backBufferSize.x;
		swapDesc.BufferDesc.Height					= backBufferSize.y;
		swapDesc.BufferDesc.RefreshRate.Denominator	= 1;
		swapDesc.BufferDesc.RefreshRate.Numerator	= 60;
		swapDesc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow						= (HWND)params.pWindowHandle;
		swapDesc.SampleDesc.Count					= 1;
		swapDesc.SampleDesc.Quality					= 0;
		swapDesc.Windowed							= !params.fullScreen;
		swapDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
		swapDesc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		D3D_FEATURE_LEVEL level;
		D3D_FEATURE_LEVEL levels = D3D_FEATURE_LEVEL_11_0;

		D3D_DRIVER_TYPE rendererType;

		switch (params.rendererMode)
		{
		case GraphicsRendererMode_Hardware:
			rendererType = D3D_DRIVER_TYPE_HARDWARE;
			break;
		case GraphicsRendererMode_Software:
			rendererType = D3D_DRIVER_TYPE_SOFTWARE;
			break;
		case GraphicsRendererMode_Wrapper:
			rendererType = D3D_DRIVER_TYPE_WARP;
			break;
		default:
			rendererType = D3D_DRIVER_TYPE_HARDWARE;
		}

		HRESULT r = D3D11CreateDeviceAndSwapChain(
			NULL,
			rendererType,
			NULL,
			0, // DEBUG FLAG
			&levels,
			1,
			D3D11_SDK_VERSION,
			&swapDesc,
			&handles.pSwapChain,
			&handles.pDevice,
			&level,
			&handles.pContext
		);

		return SUCCEEDED( r );
	}

	bool initBackBuffer( GraphicsHandles& handles, const GraphicsSystemParameters& params )
	{
		ID3D11Texture2D* pBackBufferPtr;
		HRESULT r = handles.pSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferPtr );

		if (FAILED(r)) { return false; }

		r = handles.pDevice->CreateRenderTargetView( pBackBufferPtr, NULL, &handles.pBackBufferTargetView );
		pBackBufferPtr->Release();

		if (FAILED(r)) { return false; }

		return true;
	}

	bool initDepthStencilBuffer( GraphicsHandles& handles, const GraphicsSystemParameters& params, const uint2& backBufferSize )
	{
		TIKI_DECLARE_STACKANDZERO( D3D11_TEXTURE2D_DESC, depthDesc );
		depthDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.Usage					= D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
		depthDesc.Width					= backBufferSize.x;
		depthDesc.Height				= backBufferSize.y;
		depthDesc.MipLevels				= 1;
		depthDesc.ArraySize				= 1;
		depthDesc.SampleDesc.Count		= 1;
		depthDesc.SampleDesc.Quality	= 0;
		depthDesc.CPUAccessFlags		= 0;
		depthDesc.MiscFlags				= 0;

		HRESULT r = handles.pDevice->CreateTexture2D( &depthDesc, NULL, &handles.pDepthStencilBuffer );
		if (FAILED(r)) { return false; }

		TIKI_DECLARE_STACKANDZERO( D3D11_DEPTH_STENCIL_VIEW_DESC, depthStencilViewDesc );
		depthStencilViewDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice	= 0;

		r = handles.pDevice->CreateDepthStencilView( handles.pDepthStencilBuffer, &depthStencilViewDesc, &handles.pDepthStencilView );
		if (FAILED(r)) { return false; }

		handles.pContext->OMSetRenderTargets( 1, &handles.pBackBufferTargetView, handles.pDepthStencilView );

		return true;
	}

	bool initStates( GraphicsHandles& handles )
	{
		// Create and set DepthStencilState - Normal
		TIKI_DECLARE_STACKANDZERO( D3D11_DEPTH_STENCIL_DESC, depthStencilDesc );
		depthStencilDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc						= D3D11_COMPARISON_LESS;
		depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.DepthEnable					= true;
		depthStencilDesc.StencilEnable					= false;
		depthStencilDesc.StencilReadMask				= 0x00;
		depthStencilDesc.StencilWriteMask				= 0x00;

		HRESULT r = handles.pDevice->CreateDepthStencilState( &depthStencilDesc, &handles.pDepthStencilState );
		if ( FAILED( r ) ) return false;

		handles.pContext->OMSetDepthStencilState( handles.pDepthStencilState, 1 );

		// Create DepthStencilState - No Depthtest
		memory::zero( depthStencilDesc );
		depthStencilDesc.DepthEnable = false;
		depthStencilDesc.StencilEnable = false;			

		r = handles.pDevice->CreateDepthStencilState( &depthStencilDesc, &handles.pDepthStencilStateNoDepth );
		if ( FAILED( r ) ) return false;

		// RasterizerState
		TIKI_DECLARE_STACKANDZERO( D3D11_RASTERIZER_DESC, rasterDesc );
		rasterDesc.CullMode					= D3D11_CULL_BACK;
		rasterDesc.FillMode					= D3D11_FILL_SOLID;
		rasterDesc.AntialiasedLineEnable	= false;
		rasterDesc.DepthBias				= 0;
		rasterDesc.DepthBiasClamp			= 0.0f;
		rasterDesc.DepthClipEnable			= true;
		rasterDesc.FrontCounterClockwise	= false;
		rasterDesc.MultisampleEnable		= false;
		rasterDesc.ScissorEnable			= false;
		rasterDesc.SlopeScaledDepthBias		= 0.0f;

		r = handles.pDevice->CreateRasterizerState( &rasterDesc, &handles.pRasterState );
		if(FAILED( r )) return false;

		handles.pContext->RSSetState( handles.pRasterState );

		return true;
	}

	bool initBlendStates( GraphicsHandles& handles )
	{
		D3D11_BLEND_DESC blendStateDesc;
		ZeroMemory( &blendStateDesc, sizeof(blendStateDesc) );

		blendStateDesc.RenderTarget[0].BlendEnable				= TRUE;
		blendStateDesc.RenderTarget[0].SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask	= 0x0f;

		HRESULT result = handles.pDevice->CreateBlendState( &blendStateDesc, &handles.pBlendStateAlphaOn );
		if( FAILED( result ) )return false;

		//ZeroMemory(&blendStateDesc, sizeof(blendStateDesc));
		//blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
		//blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;//  D3D11_BLEND_ONE;
		//blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		//blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		//blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		//blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		//blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		//blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		blendStateDesc.RenderTarget[0].BlendEnable = FALSE;

		result = handles.pDevice->CreateBlendState( &blendStateDesc, &handles.pBlendStateAlphaOff );

		if ( FAILED(result) ) return false; 

		return true;
	}

	void initViewPort( GraphicsHandles& handles, const Vector2& backBufferSize )
	{
		TIKI_DECLARE_STACKANDZERO( D3D11_VIEWPORT, viewPort );
		viewPort.Width		= (float)backBufferSize.x;
		viewPort.Height		= (float)backBufferSize.y;

		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;

		//this->viewPort = ViewPort(
		//	(Int32)viewPort.TopLeftX,
		//	(Int32)viewPort.TopLeftY,
		//	(Int32)viewPort.Width,
		//	(Int32)viewPort.Height,
		//	viewPort.MinDepth,
		//	viewPort.MaxDepth
		//	);
		//desc.Graphics.ViewPort = this->viewPort;

		handles.pContext->RSSetViewports( 1, &viewPort );
	}

	GraphicsSystem::GraphicsSystem()
		: m_pHandles( nullptr )
	{
		m_backBufferSizeFloat.x = 0.0f;
		m_backBufferSizeFloat.y = 0.0f;

		m_backBufferSize.x = 0u;
		m_backBufferSize.y = 0u;
	}

	GraphicsSystem::~GraphicsSystem()
	{
		TIKI_ASSERT( m_pHandles == nullptr );
	}

	bool GraphicsSystem::create( const GraphicsSystemParameters& params )
	{
		TIKI_ASSERT( m_pHandles == nullptr );

		m_pHandles			= TIKI_NEW GraphicsHandles();

		m_backBufferSize.x	= params.backBufferWidth;
		m_backBufferSize.y	= params.backBufferHeight;

		if (params.fullScreen)
		{
			RECT rect;
			HWND hDesktop = GetDesktopWindow();
			GetWindowRect(hDesktop, &rect);
			
			m_backBufferSize.x = (rect.left - rect.right);
			m_backBufferSize.y = (rect.top - rect.bottom);
		}

		GraphicsHandles& handles = *m_pHandles;
		if( !initSwapChain( handles, params, m_backBufferSize ) )
		{
			return false;
		}

		if( !initBackBuffer( handles, params ) )
		{
			return false;
		}

		if( !initDepthStencilBuffer( handles, params, m_backBufferSize ) )
		{
			return false;
		}

		if( !initStates( handles ) )
		{
			return false;
		}

		if( !initBlendStates( handles ) )
		{
			return false;
		}

		m_backBufferSizeFloat.x = (float)m_backBufferSize.x;
		m_backBufferSizeFloat.y = (float)m_backBufferSize.y;
		
		initViewPort( handles, m_backBufferSizeFloat );

		VertexFormat::initializeSystem();

		return true;
	}

	bool GraphicsSystem::createContext()
	{
		return m_commandBuffer.create( *this );
	}

	void GraphicsSystem::disposeContext()
	{
		m_commandBuffer.dispose();
	}

	void GraphicsSystem::dispose()
	{
		VertexFormat::disposeSystem();
		
		GraphicsHandles& handles = *m_pHandles;
		if( handles.pSwapChain )
		{
			handles.pSwapChain->SetFullscreenState( false, NULL );
		}

		safeRelease( &handles.pDepthStencilState );
		safeRelease( &handles.pDepthStencilStateNoDepth );
		safeRelease( &handles.pRasterState );

		safeRelease( &handles.pDepthStencilView );
		safeRelease( &handles.pDepthStencilBuffer );

		safeRelease( &handles.pBackBufferTargetView);
		safeRelease( &handles.pContext );
		safeRelease( &handles.pDevice );
		safeRelease( &handles.pSwapChain );
		safeDelete( &m_pHandles );
	}

	GraphicsContext* GraphicsSystem::beginFrame()
	{	
		m_frameCount++;

		return &m_commandBuffer;
	}

	void GraphicsSystem::endFrame()
	{
		m_pHandles->pSwapChain->Present( 1, 0 );
	}

}