
#include "tiki/graphics/graphicssystem.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/vertexformat.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	TIKI_COMPILETIME_ASSERT( GL_VERSION_4_4 );

	namespace graphics
	{
		static bool initOpenGL( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params );
		static bool initBackBuffer( GraphicsSystemPlatformData& data, const uint2& backBufferSize );
		static bool initDepthStencilBuffer( GraphicsSystemPlatformData& data, const uint2& backBufferSize );

		//static void resetDeviceState( TGContext* pContext );

		static GraphicsSystemPlatformData& getPlatformData( GraphicsSystem& graphicSystem );
	}

	//static GraphicsSystemPlatformData& graphics::getPlatformData( GraphicsSystem& graphicSystem )
	//{
	//	return *(GraphicsSystemPlatformData*)addPtr( &graphicSystem, sizeof( uint ) );
	//}

	//TGDevice*	graphics::getDevice( GraphicsSystem& graphicsSystem )	{ return getPlatformData( graphicsSystem ).pDevice; }
	//TGContext*	graphics::getContext( GraphicsSystem& graphicsSystem )	{ return getPlatformData( graphicsSystem ).pContext; }

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

		if( !graphics::initOpenGL( m_platformData, params ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create OpenGL.\n" );
			return false;
		}

		if( !graphics::initDepthStencilBuffer( m_platformData, backBufferSize ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create DepthStencilBuffer.\n" );
			return false;
		}

		glViewport( 0, 0, backBufferSize.x, backBufferSize.y );

		// create back buffer target
		{
			m_backBufferTarget.m_width						= backBufferSize.x;
			m_backBufferTarget.m_height						= backBufferSize.y;

			m_backBufferTarget.m_colorBuffers[ 0u ].format		= PixelFormat_R8G8B8A8_Gamma;
			m_backBufferTarget.m_colorBuffers[ 0u ].pDataBuffer = nullptr;
			m_backBufferTarget.m_colorBufferCount = 1u;

			m_backBufferTarget.m_depthBuffer.format			= PixelFormat_Depth24Stencil8;
			m_backBufferTarget.m_depthBuffer.pDataBuffer	= nullptr;

			m_backBufferTarget.m_platformData.frameBufferId	= 0u;
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
		
		//if( m_platformData.pSwapChain != nullptr )
		//{
		//	m_platformData.pSwapChain->SetFullscreenState( false, nullptr );
		//}

		//if ( m_platformData.pContext != nullptr )
		//{
		//	m_platformData.pContext->ClearState();
		//	m_platformData.pContext->Flush();
		//}

		//safeRelease( &m_platformData.pDepthStencilView );
		//safeRelease( &m_platformData.pDepthStencilBuffer );
		//safeRelease( &m_platformData.pBackBufferTargetView);

		//safeRelease( &m_platformData.pContext );
		//safeRelease( &m_platformData.pSwapChain );
		//safeRelease( &m_platformData.pDevice );
	}

	bool GraphicsSystem::resize( uint width, uint height )
	{
		if ( width == 0u || height == 0u )
		{
			return false;
		}

		//m_platformData.pContext->OMSetRenderTargets( 0u, nullptr, nullptr );

		//safeRelease( &m_platformData.pDepthStencilView );
		//safeRelease( &m_platformData.pDepthStencilBuffer );
		//safeRelease( &m_platformData.pBackBufferTargetView);

		//HRESULT result = m_platformData.pSwapChain->ResizeBuffers( 0, UINT( width ), UINT( height ), DXGI_FORMAT_UNKNOWN, 0 );
		//if ( FAILED( result ) )
		//{
		//	dispose();
		//	return false;
		//}

		//uint2 backBufferSize = { uint32( width ), uint32( height ) };
		//graphics::initBackBuffer( m_platformData );
		//graphics::initDepthStencilBuffer( m_platformData, backBufferSize );

		//m_backBufferTarget.m_width	= width;
		//m_backBufferTarget.m_height	= height;
		//m_backBufferTarget.m_platformData.pColorViews[ 0u ]	= m_platformData.pBackBufferTargetView;
		//m_backBufferTarget.m_platformData.pDepthView		= m_platformData.pDepthStencilView;

		return true;
	}

	GraphicsContext& GraphicsSystem::beginFrame()
	{	
		m_frameNumber++;

		return m_commandBuffer;
	}

	void GraphicsSystem::endFrame()
	{
		//SwapBuffers( m_platformData.deviceContextHandle );
		
		//graphics::resetDeviceState( m_platformData.pContext );

		SwapBuffers( (HDC)m_platformData.deviceContextHandle );
	}

	static bool graphics::initOpenGL( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params )
	{
		data.deviceContextHandle = (DeviceContextHandle)GetDC( (HWND)params.pWindowHandle );
		if ( data.deviceContextHandle == nullptr )
		{
			return false;
		}

		TIKI_DECLARE_STACKANDZERO( PIXELFORMATDESCRIPTOR, pixelDesc );
		pixelDesc.nSize			= sizeof(pixelDesc);
		pixelDesc.nVersion		= 1u;
		pixelDesc.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pixelDesc.iPixelType	= PFD_TYPE_RGBA;
		pixelDesc.cColorBits	= 32u;
		pixelDesc.cDepthBits	= 24u;
		pixelDesc.cStencilBits	= 8u;
		pixelDesc.cAlphaBits	= 8u;
		pixelDesc.iLayerType	= PFD_MAIN_PLANE;
		data.pixelFormatHandle = ChoosePixelFormat( (HDC)data.deviceContextHandle, &pixelDesc );

		if ( !SetPixelFormat( (HDC)data.deviceContextHandle, data.pixelFormatHandle, &pixelDesc ) )
			return false;

		data.renderContextHandle = (RenderContextHandle)wglCreateContext( (HDC)data.deviceContextHandle );
		if ( data.renderContextHandle == nullptr )
		{
			return false;
		}

		if ( !wglMakeCurrent( (HDC)data.deviceContextHandle, (HGLRC)data.renderContextHandle ) )
		{
			return false;
		}

		// init Extensions
		if ( glewInit() != GLEW_OK )
		{
			return false;
		}

		//// Depth Buffer
		//glClearDepth(1.0f);
		//glDepthFunc(GL_LEQUAL);	
		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_ALPHA_TEST);

		// Back face culling
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		return true;
	}

	static bool graphics::initBackBuffer( GraphicsSystemPlatformData& data, const uint2& backBufferSize )
	{
		return true;
	}

	static bool graphics::initDepthStencilBuffer( GraphicsSystemPlatformData& data, const uint2& backBufferSize )
	{
		//TIKI_DECLARE_STACKANDZERO( D3D11_TEXTURE2D_DESC, depthDesc );
		//depthDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
		//depthDesc.Usage					= D3D11_USAGE_DEFAULT;
		//depthDesc.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
		//depthDesc.Width					= backBufferSize.x;
		//depthDesc.Height				= backBufferSize.y;
		//depthDesc.MipLevels				= 1;
		//depthDesc.ArraySize				= 1;
		//depthDesc.SampleDesc.Count		= 1;
		//depthDesc.SampleDesc.Quality	= 0;
		//depthDesc.CPUAccessFlags		= 0;
		//depthDesc.MiscFlags				= 0;

		//HRESULT r = data.pDevice->CreateTexture2D( &depthDesc, nullptr, &data.pDepthStencilBuffer );
		//if (FAILED(r)) { return false; }

		//TIKI_DECLARE_STACKANDZERO( D3D11_DEPTH_STENCIL_VIEW_DESC, depthStencilViewDesc );
		//depthStencilViewDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
		//depthStencilViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
		//depthStencilViewDesc.Texture2D.MipSlice	= 0;

		//r = data.pDevice->CreateDepthStencilView( data.pDepthStencilBuffer, &depthStencilViewDesc, &data.pDepthStencilView );
		//if (FAILED(r)) { return false; }

		//data.pContext->OMSetRenderTargets( 1, &data.pBackBufferTargetView, data.pDepthStencilView );

		return true;
	}

	//void graphics::resetDeviceState( TGContext* pContext )
	//{
	//	void* pNullPointers[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	//	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( pNullPointers ) >= GraphicsSystemLimits_VertexShaderConstantSlots );
	//	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( pNullPointers ) >= GraphicsSystemLimits_VertexShaderTextureSlots );
	//	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( pNullPointers ) >= GraphicsSystemLimits_PixelShaderConstantSlots );
	//	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( pNullPointers ) >= GraphicsSystemLimits_PixelShaderTextureSlots );
	//	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( pNullPointers ) >= GraphicsSystemLimits_MaxInputStreams );
	//	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( pNullPointers ) >= GraphicsSystemLimits_RenderTargetSlots );
	//	TIKI_COMPILETIME_ASSERT( sizeof( void* ) >= sizeof( UINT ) );

	//	pContext->VSSetConstantBuffers( 0u, GraphicsSystemLimits_VertexShaderConstantSlots, reinterpret_cast< ID3D11Buffer** >( pNullPointers ) );
	//	pContext->VSSetShaderResources( 0u, GraphicsSystemLimits_VertexShaderTextureSlots, reinterpret_cast< ID3D11ShaderResourceView** >( pNullPointers ) );
	//	pContext->PSSetConstantBuffers( 0u, GraphicsSystemLimits_PixelShaderConstantSlots, reinterpret_cast< ID3D11Buffer** >( pNullPointers ) );
	//	pContext->PSSetShaderResources( 0u, GraphicsSystemLimits_PixelShaderTextureSlots, reinterpret_cast< ID3D11ShaderResourceView** >( pNullPointers ) );
	//	pContext->IASetVertexBuffers( 0u, GraphicsSystemLimits_MaxInputStreams, reinterpret_cast< ID3D11Buffer** >( pNullPointers ), reinterpret_cast< UINT* >( pNullPointers ), reinterpret_cast< UINT* >( pNullPointers ) );
	//	pContext->OMSetRenderTargets( 0u, nullptr, nullptr );
	//}

	bool graphics::checkError()
	{
		const GLenum error = glGetError();
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		if ( error != GL_NO_ERROR )
		{
			TIKI_TRACE_ERROR( "[graphics] An error has occured. Error: %s\n", gluErrorString( error ) );
			return false;
		}
#endif
		return true;
	}
}