#include "tiki/graphics/graphics_system.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/vertex_format.hpp"

#include <d3d11.h>
#include <dxgi.h>

namespace tiki
{
	namespace graphics
	{
		static bool initSwapChain( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params, const uint2& backBufferSize );
		static bool initBackBuffer( GraphicsSystemPlatformData& data, PixelFormat format );
		static bool initDepthStencilBuffer( GraphicsSystemPlatformData& data, const uint2& backBufferSize );
		static void initViewPort( GraphicsSystemPlatformData& data, const uint2& backBufferSize );

		static void resetDeviceState( ID3D11DeviceContext* pContext );

		static GraphicsSystemPlatformData& getPlatformData( GraphicsSystem& graphicSystem );
	}

	ID3D11Device*			GraphicsSystemPlatform::getDevice( GraphicsSystem& graphicsSystem )		{ return graphicsSystem.m_platformData.pDevice; }
	ID3D11DeviceContext*	GraphicsSystemPlatform::getContext( GraphicsSystem& graphicsSystem )	{ return graphicsSystem.m_platformData.pContext; }

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
		backBufferSize.x = (uint32)params.backBufferWidth;
		backBufferSize.y = (uint32)params.backBufferHeight;

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
			disposePlatform();
			return false;
		}

		if( !graphics::initBackBuffer( m_platformData, params.backBufferFormat ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create BackBuffer.\n" );
			disposePlatform();
			return false;
		}

		if( !graphics::initDepthStencilBuffer( m_platformData, backBufferSize ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create DepthStencilBuffer.\n" );
			disposePlatform();
			return false;
		}

		graphics::initViewPort( m_platformData, backBufferSize );

		// create back buffer target
		{
			m_backBufferColorData.m_description.width			= backBufferSize.x;
			m_backBufferColorData.m_description.height			= backBufferSize.y;
			m_backBufferColorData.m_description.flags			= TextureFlags_RenderTarget;
			m_backBufferColorData.m_description.type			= TextureType_2d;
			m_backBufferColorData.m_description.format			= params.backBufferFormat;

			m_backBufferDepthData.m_description.width			= backBufferSize.x;
			m_backBufferDepthData.m_description.height			= backBufferSize.y;
			m_backBufferDepthData.m_description.flags			= TextureFlags_DepthStencil;
			m_backBufferDepthData.m_description.type			= TextureType_2d;
			m_backBufferDepthData.m_description.format			= PixelFormat_Depth24Stencil8;

			m_backBufferTarget.m_width							= params.backBufferWidth;
			m_backBufferTarget.m_height							= params.backBufferHeight;

			m_backBufferTarget.m_colorBuffers.push( &m_backBufferColorData );
			m_backBufferTarget.m_platformData.pColorViews[ 0u ]	= m_platformData.pBackBufferTargetView;

			m_backBufferTarget.m_pDepthBuffer					= &m_backBufferDepthData;
			m_backBufferTarget.m_platformData.pDepthView		= m_platformData.pDepthStencilView;
		}

		if ( !m_commandBuffer.create( *this ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create CommandBuffer.\n" );
			disposePlatform();
			return false;
		}

		return true;
	}

	void GraphicsSystem::disposePlatform()
	{
		// dispose back buffer target
		{
			m_backBufferTarget.m_width	= 0u;
			m_backBufferTarget.m_height	= 0u;

			m_backBufferTarget.m_colorBuffers.clear();
			m_backBufferTarget.m_platformData.pColorViews[ 0u ]	= nullptr;

			m_backBufferTarget.m_pDepthBuffer = nullptr;
			m_backBufferTarget.m_platformData.pDepthView = nullptr;
		}

		m_commandBuffer.dispose( *this );

		if( m_platformData.pSwapChain != nullptr )
		{
			m_platformData.pSwapChain->SetFullscreenState( false, nullptr );
		}

		if ( m_platformData.pContext != nullptr )
		{
			m_platformData.pContext->ClearState();
			m_platformData.pContext->Flush();
		}

		safeRelease( &m_platformData.pDepthStencilView );
		safeRelease( &m_platformData.pDepthStencilBuffer );
		safeRelease( &m_platformData.pBackBufferTargetView);

		safeRelease( &m_platformData.pContext );
		safeRelease( &m_platformData.pSwapChain );
		safeRelease( &m_platformData.pDevice );
	}

	bool GraphicsSystem::resize( uint16 width, uint16 height )
	{
		if( width == m_backBufferTarget.m_width && height == m_backBufferTarget.m_height )
		{
			return true;
		}
		else if ( width == 0u || height == 0u )
		{
			return false;
		}

		m_platformData.pContext->OMSetRenderTargets( 0u, nullptr, nullptr );

		safeRelease( &m_platformData.pDepthStencilView );
		safeRelease( &m_platformData.pDepthStencilBuffer );
		safeRelease( &m_platformData.pBackBufferTargetView);

		HRESULT result = m_platformData.pSwapChain->ResizeBuffers( 0, UINT( width ), UINT( height ), DXGI_FORMAT_UNKNOWN, 0 );
		if ( FAILED( result ) )
		{
			dispose();
			return false;
		}

		m_backBufferColorData.m_description.width	= width;
		m_backBufferColorData.m_description.height	= height;
		m_backBufferDepthData.m_description.width	= width;
		m_backBufferDepthData.m_description.height	= height;
		m_backBufferTarget.m_width					= width;
		m_backBufferTarget.m_height					= height;

		const uint2 backBufferSize = { uint32( width ), uint32( height ) };
		if( !graphics::initBackBuffer( m_platformData, m_backBufferColorData.m_description.format ) ||
			!graphics::initDepthStencilBuffer( m_platformData, backBufferSize ) )
		{
			return false;
		}

		m_backBufferTarget.m_platformData.pColorViews[ 0u ]	= m_platformData.pBackBufferTargetView;
		m_backBufferTarget.m_platformData.pDepthView		= m_platformData.pDepthStencilView;

		return true;
	}

	GraphicsContext& GraphicsSystem::beginFrame()
	{
		m_frameNumber++;

		return m_commandBuffer;
	}

	void GraphicsSystem::endFrame()
	{
		m_platformData.pSwapChain->Present( 1, 0 );

		graphics::resetDeviceState( m_platformData.pContext );
	}

	static bool graphics::initSwapChain( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params, const uint2& backBufferSize )
	{
		TIKI_DECLARE_STACKANDZERO( DXGI_SWAP_CHAIN_DESC, swapDesc );
		swapDesc.BufferCount						= 2;
		swapDesc.BufferDesc.Format					= getD3dFormat( params.backBufferFormat, TextureFlags_RenderTarget, false );
		swapDesc.BufferDesc.Width					= backBufferSize.x;
		swapDesc.BufferDesc.Height					= backBufferSize.y;
		swapDesc.BufferDesc.RefreshRate.Denominator	= 1;
		swapDesc.BufferDesc.RefreshRate.Numerator	= 60;
		swapDesc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow						= static_cast< HWND >( params.pWindowHandle );
		swapDesc.SampleDesc.Count					= 1;
		swapDesc.SampleDesc.Quality					= 0;
		swapDesc.Windowed							= !params.fullScreen;
		swapDesc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapDesc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		D3D_FEATURE_LEVEL level;
		D3D_FEATURE_LEVEL aLevels[] =
		{
			//D3D_FEATURE_LEVEL_10_0,
			//D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_11_0
		};

		HRESULT r = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
			D3D11_CREATE_DEVICE_DEBUG,
#else
			D3D11_CREATE_DEVICE_SINGLETHREADED,
#endif
			aLevels,
			TIKI_COUNT( aLevels ),
			D3D11_SDK_VERSION,
			&swapDesc,
			&data.pSwapChain,
			&data.pDevice,
			&level,
			&data.pContext
		);

		return SUCCEEDED( r );
	}

	static bool graphics::initBackBuffer( GraphicsSystemPlatformData& data, PixelFormat format )
	{
		ID3D11Texture2D* pBackBufferPtr;
		HRESULT r = data.pSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferPtr );
		if( FAILED( r ) )
		{
			return false;
		}

		D3D11_RENDER_TARGET_VIEW_DESC backBufferViewDesc;
		backBufferViewDesc.Format				= graphics::getD3dFormat( format, TextureFlags_RenderTarget, false );
		backBufferViewDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
		backBufferViewDesc.Texture2D.MipSlice	= 0;

		r = data.pDevice->CreateRenderTargetView( pBackBufferPtr, &backBufferViewDesc, &data.pBackBufferTargetView );
		pBackBufferPtr->Release();
		if( FAILED( r ) )
		{
			return false;
		}

		return true;
	}

	static bool graphics::initDepthStencilBuffer( GraphicsSystemPlatformData& data, const uint2& backBufferSize )
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

	void graphics::resetDeviceState( ID3D11DeviceContext* pContext )
	{
		void* pNullPointers[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( pNullPointers ) >= GraphicsSystemLimits_VertexShaderConstantSlots );
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( pNullPointers ) >= GraphicsSystemLimits_VertexShaderTextureSlots );
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( pNullPointers ) >= GraphicsSystemLimits_PixelShaderConstantSlots );
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( pNullPointers ) >= GraphicsSystemLimits_PixelShaderTextureSlots );
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( pNullPointers ) >= GraphicsSystemLimits_MaxInputStreams );
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( pNullPointers ) >= GraphicsSystemLimits_RenderTargetSlots );
		TIKI_COMPILETIME_ASSERT( sizeof( void* ) >= sizeof( UINT ) );

		pContext->VSSetConstantBuffers( 0u, GraphicsSystemLimits_VertexShaderConstantSlots, reinterpret_cast< ID3D11Buffer** >( pNullPointers ) );
		pContext->VSSetShaderResources( 0u, GraphicsSystemLimits_VertexShaderTextureSlots, reinterpret_cast< ID3D11ShaderResourceView** >( pNullPointers ) );
		pContext->PSSetConstantBuffers( 0u, GraphicsSystemLimits_PixelShaderConstantSlots, reinterpret_cast< ID3D11Buffer** >( pNullPointers ) );
		pContext->PSSetShaderResources( 0u, GraphicsSystemLimits_PixelShaderTextureSlots, reinterpret_cast< ID3D11ShaderResourceView** >( pNullPointers ) );
		pContext->IASetVertexBuffers( 0u, GraphicsSystemLimits_MaxInputStreams, reinterpret_cast< ID3D11Buffer** >( pNullPointers ), reinterpret_cast< UINT* >( pNullPointers ), reinterpret_cast< UINT* >( pNullPointers ) );
		pContext->OMSetRenderTargets( 0u, nullptr, nullptr );
	}
}