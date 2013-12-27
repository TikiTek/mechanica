
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3d11.lib")

#include "tiki/graphics/graphicssystem.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/graphicsbase/color.hpp"
#include "tiki/graphicsresources/material.hpp"
#include "tiki/graphics/vertexformat.hpp"

#include "graphicssystem_internal_d3d11.hpp"

namespace tiki
{
	namespace graphics
	{
		static bool initSwapChain( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params, const uint2& backBufferSize );
		static bool initBackBuffer( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params );
		static bool initDepthStencilBuffer( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params, const uint2& backBufferSize );
		static void initViewPort( GraphicsSystemPlatformData& data, const uint2& backBufferSize );

		static GraphicsSystemPlatformData& getPlatformData( GraphicsSystem& graphicSystem );
	}

	static GraphicsSystemPlatformData& graphics::getPlatformData( GraphicsSystem& graphicSystem )
	{
		return *(GraphicsSystemPlatformData*)addPtr( &graphicSystem, 4u );
	}

	TGDevice*	graphics::getDevice( GraphicsSystem& graphicsSystem )	{ return getPlatformData( graphicsSystem ).pDevice; }
	TGContext*	graphics::getContext( GraphicsSystem& graphicsSystem )	{ return getPlatformData( graphicsSystem ).pContext; }

	template<class T>
	TIKI_FORCE_INLINE void safeRelease( T** ppObject )
	{
		if ( *ppObject != nullptr )
		{
			(*ppObject)->Release();
			*ppObject = nullptr;
		}
	}

	bool GraphicsSystem::createPlatform( const GraphicsSystemParameters& params )
	{
		uint2 backBufferSize;
		backBufferSize.x = params.backBufferWidth;
		backBufferSize.y = params.backBufferHeight;

		if ( params.fullScreen )
		{
			RECT rect;
			HWND hDesktop = GetDesktopWindow();
			GetWindowRect(hDesktop, &rect);
			
			backBufferSize.x = (rect.left - rect.right);
			backBufferSize.y = (rect.top - rect.bottom);
		}

		if( !graphics::initSwapChain( m_platformData, params, backBufferSize ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create SwapChain.\n" );
			return false;
		}

		if( !graphics::initBackBuffer( m_platformData, params ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create BackBuffer.\n" );
			return false;
		}

		if( !graphics::initDepthStencilBuffer( m_platformData, params, backBufferSize ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create DepthStencilBuffer.\n" );
			return false;
		}

		graphics::initViewPort( m_platformData, backBufferSize );

		// create back buffer
		{
			m_backBufferTarget.m_width	= params.backBufferWidth;
			m_backBufferTarget.m_height	= params.backBufferHeight;

			m_backBufferTarget.m_colorBuffers[ 0u ].format		= PixelFormat_R8G8B8A8_Gamma;
			m_backBufferTarget.m_colorBuffers[ 0u ].pDataBuffer = nullptr;
			m_backBufferTarget.m_platformData.pColorViews[ 0u ]	= m_platformData.pBackBufferTargetView;
			m_backBufferTarget.m_colorBufferCount = 1u;

			m_backBufferTarget.m_depthBuffer.format			= PixelFormat_Depth24Stencil8;
			m_backBufferTarget.m_depthBuffer.pDataBuffer	= nullptr;
			m_backBufferTarget.m_platformData.pDepthView	= m_platformData.pDepthStencilView;
		}

		if ( !m_commandBuffer.create( *this ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create CommandBuffer.\n" );
			return false;
		}

		return true;
	}

	void GraphicsSystem::disposePlatform()
	{
		m_commandBuffer.dispose( *this );
		
		if( m_platformData.pSwapChain )
		{
			m_platformData.pSwapChain->SetFullscreenState( false, nullptr );
		}

		safeRelease( &m_platformData.pDepthStencilView );
		safeRelease( &m_platformData.pDepthStencilBuffer );

		safeRelease( &m_platformData.pBackBufferTargetView);
		safeRelease( &m_platformData.pContext );
		safeRelease( &m_platformData.pDevice );
		safeRelease( &m_platformData.pSwapChain );
	}

	GraphicsContext& GraphicsSystem::beginFrame()
	{	
		m_frameNumber++;

		return m_commandBuffer;
	}

	void GraphicsSystem::endFrame()
	{
		m_platformData.pSwapChain->Present( 1, 0 );
	}

	static bool graphics::initSwapChain( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params, const uint2& backBufferSize )
	{
		TIKI_DECLARE_STACKANDZERO( DXGI_SWAP_CHAIN_DESC, swapDesc );
		swapDesc.BufferCount						= 2;
		swapDesc.BufferDesc.Format					= DXGI_FORMAT_B8G8R8A8_UNORM;
		swapDesc.BufferDesc.Width					= backBufferSize.x;
		swapDesc.BufferDesc.Height					= backBufferSize.y;
		swapDesc.BufferDesc.RefreshRate.Denominator	= 1;
		swapDesc.BufferDesc.RefreshRate.Numerator	= 60;
		swapDesc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow						= static_cast< HWND >( params.pWindowHandle );
		swapDesc.SampleDesc.Count					= 1;
		swapDesc.SampleDesc.Quality					= 0;
		swapDesc.Windowed							= !params.fullScreen;
		swapDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
		swapDesc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		D3D_FEATURE_LEVEL level;
		D3D_FEATURE_LEVEL levels = D3D_FEATURE_LEVEL_11_0;

		D3D_DRIVER_TYPE rendererType;

		switch ( params.rendererMode )
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
			break;
		}

		HRESULT r = D3D11CreateDeviceAndSwapChain(
			nullptr,
			rendererType,
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			&levels,
			1,
			D3D11_SDK_VERSION,
			&swapDesc,
			&data.pSwapChain,
			&data.pDevice,
			&level,
			&data.pContext
		);

		return SUCCEEDED( r );
	}

	static bool graphics::initBackBuffer( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params )
	{
		ID3D11Texture2D* pBackBufferPtr;
		HRESULT r = data.pSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferPtr );

		if (FAILED(r)) { return false; }

		r = data.pDevice->CreateRenderTargetView( pBackBufferPtr, nullptr, &data.pBackBufferTargetView );
		pBackBufferPtr->Release();

		if (FAILED(r)) { return false; }

		return true;
	}

	static bool graphics::initDepthStencilBuffer( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params, const uint2& backBufferSize )
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

		HRESULT r = data.pDevice->CreateTexture2D( &depthDesc, nullptr, &data.pDepthStencilBuffer );
		if (FAILED(r)) { return false; }

		TIKI_DECLARE_STACKANDZERO( D3D11_DEPTH_STENCIL_VIEW_DESC, depthStencilViewDesc );
		depthStencilViewDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice	= 0;

		r = data.pDevice->CreateDepthStencilView( data.pDepthStencilBuffer, &depthStencilViewDesc, &data.pDepthStencilView );
		if (FAILED(r)) { return false; }

		data.pContext->OMSetRenderTargets( 1, &data.pBackBufferTargetView, data.pDepthStencilView );

		return true;
	}

	static void graphics::initViewPort( GraphicsSystemPlatformData& data, const uint2& backBufferSize )
	{
		TIKI_DECLARE_STACKANDZERO( D3D11_VIEWPORT, viewPort );
		viewPort.MinDepth	= 0.0f;
		viewPort.MaxDepth	= 1.0f;
		viewPort.TopLeftX	= 0;
		viewPort.TopLeftY	= 0;
		viewPort.Width		= (float)backBufferSize.x;
		viewPort.Height		= (float)backBufferSize.y;
		data.pContext->RSSetViewports( 1, &viewPort );
	}
}