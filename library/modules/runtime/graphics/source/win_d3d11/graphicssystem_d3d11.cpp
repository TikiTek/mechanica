
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
	void safeRelease( T** ppObject )
	{
		if ( *ppObject != nullptr )
		{
			(*ppObject)->Release();
			*ppObject = nullptr;
		}
	}

	GraphicsSystem::GraphicsSystem()
	{
	}

	GraphicsSystem::~GraphicsSystem()
	{
	}

	bool GraphicsSystem::create( const GraphicsSystemParameters& params )
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
			return false;
		}

		if( !graphics::initBackBuffer( m_platformData, params ) )
		{
			return false;
		}

		if( !graphics::initDepthStencilBuffer( m_platformData, params, backBufferSize ) )
		{
			return false;
		}

		graphics::initViewPort( m_platformData, backBufferSize );

		m_samplerStates.create( 32u );

		if ( !m_commandBuffer.create( *this ) )
		{
			return false;
		}

		return true;
	}

	void GraphicsSystem::dispose()
	{
		m_commandBuffer.dispose();

		m_samplerStates.dispose();
		
		if( m_platformData.pSwapChain )
		{
			m_platformData.pSwapChain->SetFullscreenState( false, NULL );
		}

		safeRelease( &m_platformData.pDepthStencilView );
		safeRelease( &m_platformData.pDepthStencilBuffer );

		safeRelease( &m_platformData.pBackBufferTargetView);
		safeRelease( &m_platformData.pContext );
		safeRelease( &m_platformData.pDevice );
		safeRelease( &m_platformData.pSwapChain );
	}

	GraphicsContext* GraphicsSystem::beginFrame()
	{	
		m_frameNumber++;

		return &m_commandBuffer;
	}

	void GraphicsSystem::endFrame()
	{
		m_platformData.pSwapChain->Present( 1, 0 );
	}

	const SamplerState* GraphicsSystem::createSamplerState( const SamplerStateParamters& creationParameters )
	{
		SamplerState* pSampler = m_samplerStates.findOrAllocate( crcT( &creationParameters ) );
		TIKI_ASSERT( pSampler != nullptr );

		if ( pSampler->icCreated() == false )
		{
			pSampler->create( *this, creationParameters );
		}

		return pSampler;
	}

	const SamplerState* GraphicsSystem::createSamplerState( AddressMode addressU, AddressMode addressV, AddressMode addressW, FilterMode magFilter, FilterMode mipFilter, size_t maxAnisotropy /*= 1*/, Color borderColor /*= TIKI_COLOR_BLACK */ )
	{
		SamplerStateParamters creationParameters;
		creationParameters.addressU			= addressU;
		creationParameters.addressV			= addressV;
		creationParameters.addressW			= addressW;
		creationParameters.magFilter		= magFilter;
		creationParameters.mipFilter		= mipFilter;
		creationParameters.maxAnisotropy	= maxAnisotropy;
		creationParameters.borderColor		= borderColor;

		return createSamplerState( creationParameters );
	}

	void GraphicsSystem::disposeSamplerState( const SamplerState* pSamplerState )
	{
		SamplerState* pNonConstState = const_cast< SamplerState* >( pSamplerState );

		if ( pNonConstState->releaseRef() == true )
		{
			pNonConstState->dispose();
		}
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

		r = data.pDevice->CreateRenderTargetView( pBackBufferPtr, NULL, &data.pBackBufferTargetView );
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

		HRESULT r = data.pDevice->CreateTexture2D( &depthDesc, NULL, &data.pDepthStencilBuffer );
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