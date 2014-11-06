
#include "tiki/graphics/graphicssystem.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/vertexformat.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	namespace graphics
	{
		static bool initSwapChain( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params, const uint2& backBufferSize );
		static bool initObjects( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params );
		static bool initBackBuffer( GraphicsSystemPlatformData& data );
		static bool initDepthStencilBuffer( GraphicsSystemPlatformData& data, const uint2& backBufferSize );

		static void resetDeviceState( const GraphicsSystemPlatformData& data );
	}

	GraphicsSystemPlatformData& graphics::getPlatformData( GraphicsSystem& graphicSystem )
	{
		return *(GraphicsSystemPlatformData*)addPtr( &graphicSystem, sizeof( uint ) );
	}

	ID3D12Device*		graphics::getDevice( GraphicsSystem& graphicsSystem )		{ return getPlatformData( graphicsSystem ).pDevice; }
	ID3D12CommandList*	graphics::getCommandList( GraphicsSystem& graphicsSystem )	{ return getPlatformData( graphicsSystem ).pCommandList; }

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
			return false;
		}

		if( !graphics::initBackBuffer( m_platformData ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create BackBuffer.\n" );
			return false;
		}

		if( !graphics::initDepthStencilBuffer( m_platformData, backBufferSize ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create DepthStencilBuffer.\n" );
			return false;
		}

		// create back buffer target
		{
			m_backBufferTarget.m_width	= params.backBufferWidth;
			m_backBufferTarget.m_height	= params.backBufferHeight;

			m_backBufferTarget.m_colorBuffers[ 0u ].format		= PixelFormat_R8G8B8A8_Gamma;
			m_backBufferTarget.m_colorBuffers[ 0u ].pDataBuffer = nullptr;
			m_backBufferTarget.m_platformData.pColorViews[ 0u ]	= m_platformData.pBackBufferColor;
			m_backBufferTarget.m_colorBufferCount = 1u;

			m_backBufferTarget.m_depthBuffer.format			= PixelFormat_Depth24Stencil8;
			m_backBufferTarget.m_depthBuffer.pDataBuffer	= nullptr;
			m_backBufferTarget.m_platformData.pDepthView	= m_platformData.pBackBufferDepth;
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
		// dispose back buffer target
		{
			m_backBufferTarget.m_width	= 0u;
			m_backBufferTarget.m_height	= 0u;

			m_backBufferTarget.m_colorBuffers[ 0u ].clear();
			m_backBufferTarget.m_platformData.pColorViews[ 0u ]	= nullptr;
			m_backBufferTarget.m_colorBufferCount = 0u;

			m_backBufferTarget.m_depthBuffer.clear();
			m_backBufferTarget.m_platformData.pDepthView = nullptr;
		}

		m_commandBuffer.dispose( *this );
		
		if ( m_platformData.pCommandQueue != nullptr )
		{
			HANDLE eventHandle = CreateEventEx( nullptr, FALSE, FALSE, EVENT_ALL_ACCESS );

			const UINT64 fence = m_platformData.pCommandQueue->AdvanceFence();
			if( m_platformData.pCommandQueue->GetLastCompletedFence( ) < fence )
			{
				TIKI_VERIFY( SUCCEEDED( m_platformData.pCommandQueue->SetEventOnFenceCompletion( fence, eventHandle ) ) );
				WaitForSingleObject( eventHandle, INFINITE );
			}

			CloseHandle( eventHandle );
		}

		if( m_platformData.pSwapChain != nullptr )
		{
			m_platformData.pSwapChain->SetFullscreenState( false, nullptr );
		}

		safeRelease( &m_platformData.pBackBufferDepth );
		safeRelease( &m_platformData.pBackBufferColor );

		safeRelease( &m_platformData.pPipelineState );
		safeRelease( &m_platformData.pCommandList );
		safeRelease( &m_platformData.pDescriptionHeap );
		safeRelease( &m_platformData.pRootSignature );
		safeRelease( &m_platformData.pCommandAllocator );
		safeRelease( &m_platformData.pCommandQueue );
		safeRelease( &m_platformData.pDevice );
		safeRelease( &m_platformData.pSwapChain );
	}

	bool GraphicsSystem::resize( uint width, uint height )
	{
		if ( width == 0u || height == 0u )
		{
			return false;
		}

		m_platformData.pCommandList->SetRenderTargets( nullptr, false, 0u, nullptr );

		safeRelease( &m_platformData.pBackBufferDepth );
		safeRelease( &m_platformData.pBackBufferColor );

		HRESULT result = m_platformData.pSwapChain->ResizeBuffers( 0, UINT( width ), UINT( height ), DXGI_FORMAT_UNKNOWN, 0 );
		if ( FAILED( result ) )
		{
			dispose();
			return false;
		}

		uint2 backBufferSize = { uint32( width ), uint32( height ) };
		graphics::initBackBuffer( m_platformData );
		graphics::initDepthStencilBuffer( m_platformData, backBufferSize );

		m_backBufferTarget.m_width	= width;
		m_backBufferTarget.m_height	= height;
		m_backBufferTarget.m_platformData.pColorViews[ 0u ]	= m_platformData.pBackBufferColor;
		m_backBufferTarget.m_platformData.pDepthView		= m_platformData.pBackBufferDepth;

		return true;
	}

	GraphicsContext& GraphicsSystem::beginFrame()
	{	
		m_frameNumber++;

		m_platformData.pCommandAllocator->Reset();
		m_platformData.pCommandList->Reset( m_platformData.pCommandAllocator, m_platformData.pPipelineState );

		graphics::setResourceBarrier(
			m_platformData.pCommandList,
			m_platformData.pBackBufferColor,
			D3D12_RESOURCE_USAGE_PRESENT,
			D3D12_RESOURCE_USAGE_RENDER_TARGET
		);

		return m_commandBuffer;
	}

	void GraphicsSystem::endFrame()
	{
		graphics::setResourceBarrier(
			m_platformData.pCommandList,
			m_platformData.pBackBufferColor,
			D3D12_RESOURCE_USAGE_RENDER_TARGET,
			D3D12_RESOURCE_USAGE_PRESENT
		);

		m_platformData.pCommandList->Close();
		m_platformData.pCommandQueue->ExecuteCommandList( m_platformData.pCommandList );
		
		TIKI_VERIFY( SUCCEEDED( m_platformData.pSwapChain->Present( 1, 0 ) ) );

		m_platformData.currentSwapBufferIndex = (m_platformData.currentSwapBufferIndex + 1u) % m_platformData.swapBufferCount;

		m_platformData.pBackBufferColor->Release();
		m_platformData.pSwapChain->GetBuffer( m_platformData.currentSwapBufferIndex, IID_PPV_ARGS( &m_platformData.pBackBufferColor ) );
		m_platformData.pDevice->CreateRenderTargetView( m_platformData.pBackBufferColor, nullptr, m_platformData.pDescriptionHeap->GetCPUDescriptorHandleForHeapStart() );
		
		graphics::resetDeviceState( m_platformData );
	}

	static bool graphics::initSwapChain( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params, const uint2& backBufferSize )
	{
		TIKI_DECLARE_STACKANDZERO( DXGI_SWAP_CHAIN_DESC, swapDesc );
		swapDesc.BufferCount						= 2;
		swapDesc.BufferDesc.Format					= DXGI_FORMAT_R16G16B16A16_FLOAT;
		swapDesc.BufferDesc.Width					= backBufferSize.x;
		swapDesc.BufferDesc.Height					= backBufferSize.y;
		swapDesc.BufferDesc.RefreshRate.Denominator	= 1;
		swapDesc.BufferDesc.RefreshRate.Numerator	= 60;
		swapDesc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow						= static_cast< HWND >( params.pWindowHandle );
		swapDesc.SampleDesc.Count					= 1;
		swapDesc.Windowed							= !params.fullScreen;
		swapDesc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		//swapDesc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		data.swapBufferCount = swapDesc.BufferCount;

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
			TIKI_BREAK( "[graphics] renderer type not supported.\n" );
			break;
		}

		HRESULT result = D3D12CreateDeviceAndSwapChain(
			nullptr,
			rendererType,
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
			D3D12_CREATE_DEVICE_DEBUG,
#else
			D3D12_CREATE_DEVICE_NONE,
#endif
			D3D_FEATURE_LEVEL_9_1,
			D3D12_SDK_VERSION,
			&swapDesc,
			IID_PPV_ARGS( &data.pSwapChain ),
			IID_PPV_ARGS( &data.pDevice )
		);

		return SUCCEEDED( result );
	}
	
	static bool graphics::initObjects( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params )
	{
		if( !data.pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, &data.pCommandAllocator ) )
		{
			return false;
		}

		data.pDevice->GetDefaultCommandQueue( &data.pCommandQueue );
		if( data.pCommandQueue == nullptr )
		{
			return false;
		}

		TIKI_DECLARE_STACKANDZERO( D3D12_DESCRIPTOR_HEAP_DESC, descHeap );
		descHeap.NumDescriptors	= 1;
		descHeap.Type			= D3D12_RTV_DESCRIPTOR_HEAP;

		if( FAILED( data.pDevice->CreateDescriptorHeap( &descHeap, __uuidof(ID3D12DescriptorHeap), (void**)&data.pDescriptionHeap ) ) )
		{
			return false;
		}

		if( FAILED( data.pDevice->CreateCommandList( D3D12_COMMAND_LIST_TYPE_DIRECT, data.pCommandAllocator, nullptr, &data.pCommandList ) ) )
		{
			return false;
		}

		D3D12_DESCRIPTOR_RANGE descRange;
		D3D12_ROOT_PARAMETER slotRootParameter;
		slotRootParameter.InitAsDescriptorTable( 1, &descRange, D3D12_SHADER_VISIBILITY_ALL );
		descRange.Init( D3D12_DESCRIPTOR_RANGE_CBV, 1, 0 );

		D3D12_ROOT_SIGNATURE descRootSignature;
		descRootSignature.pParameters	= &slotRootParameter;
		descRootSignature.Flags			= 0;
		descRootSignature.NumParameters	= 1;

		ID3DBlob* pOutputBlob	= nullptr;
		ID3DBlob* pErrorBlob	= nullptr;
		if( FAILED( D3D12SerializeRootSignature( &descRootSignature, D3D_ROOT_SIGNATURE_V1, &pOutputBlob, &pErrorBlob ) ) )
		{
			TIKI_ASSERT( pOutputBlob == nullptr );

			if( pErrorBlob != nullptr )
			{
				TIKI_TRACE_ERROR( "Failed to serialize RootSignature. Error: %s", (const char*)pErrorBlob->GetBufferPointer() );
				pErrorBlob->Release();
				pErrorBlob == nullptr;
			}

			return false;
		}
		TIKI_ASSERT( pOutputBlob != nullptr );
		TIKI_ASSERT( pErrorBlob == nullptr );

		HRESULT result = data.pDevice->CreateRootSignature( pOutputBlob->GetBufferPointer(), pOutputBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&data.pRootSignature );

		pOutputBlob->Release();
		pOutputBlob = nullptr;

		if( FAILED( result ) )
		{
			return false;
		}

		return true;
	}

	static bool graphics::initBackBuffer( GraphicsSystemPlatformData& data )
	{		
		HRESULT result = data.pSwapChain->GetBuffer( 0, __uuidof(ID3D12Resource), (void**)&data.pBackBufferColor );
		if( FAILED( result ) )
		{
			return false;
		}
		data.pDevice->CreateRenderTargetView( data.pBackBufferColor, nullptr, data.pDescriptionHeap->GetCPUDescriptorHandleForHeapStart() );

		result = data.pCommandList->Close();
		if( FAILED( result ) )
		{
			return false;
		}
		
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
	
	void graphics::setResourceBarrier( ID3D12CommandList* pCommandList, ID3D12Resource* pResource, UINT stateBefore, UINT stateAfter )
	{
		TIKI_DECLARE_STACKANDZERO( D3D12_RESOURCE_BARRIER_DESC, descBarrier );
		descBarrier.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		descBarrier.Transition.pResource	= pResource;
		descBarrier.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		descBarrier.Transition.StateBefore	= stateBefore;
		descBarrier.Transition.StateAfter	= stateAfter;

		pCommandList->ResourceBarrier( 1, &descBarrier );
	}

	void graphics::resetDeviceState( const GraphicsSystemPlatformData& data )
	{
		//static void* s_pNullPointers[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		//TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_pNullPointers ) >= GraphicsSystemLimits_VertexShaderConstantSlots );
		//TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_pNullPointers ) >= GraphicsSystemLimits_VertexShaderTextureSlots );
		//TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_pNullPointers ) >= GraphicsSystemLimits_PixelShaderConstantSlots );
		//TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_pNullPointers ) >= GraphicsSystemLimits_PixelShaderTextureSlots );
		//TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_pNullPointers ) >= GraphicsSystemLimits_MaxInputStreams );
		//TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_pNullPointers ) >= GraphicsSystemLimits_RenderTargetSlots );
		//TIKI_COMPILETIME_ASSERT( sizeof( void* ) >= sizeof( UINT ) );
		//
		//data.pCommandList->Reset( data.pCommandAllocator, )

		//pContext->VSSetConstantBuffers( 0u, GraphicsSystemLimits_VertexShaderConstantSlots, reinterpret_cast<ID3D11Buffer**>(s_pNullPointers) );
		//pContext->VSSetShaderResources( 0u, GraphicsSystemLimits_VertexShaderTextureSlots, reinterpret_cast<ID3D11ShaderResourceView**>(s_pNullPointers) );
		//pContext->PSSetConstantBuffers( 0u, GraphicsSystemLimits_PixelShaderConstantSlots, reinterpret_cast<ID3D11Buffer**>(s_pNullPointers) );
		//pContext->PSSetShaderResources( 0u, GraphicsSystemLimits_PixelShaderTextureSlots, reinterpret_cast<ID3D11ShaderResourceView**>(s_pNullPointers) );
		//pContext->IASetVertexBuffers( 0u, GraphicsSystemLimits_MaxInputStreams, reinterpret_cast<ID3D11Buffer**>(s_pNullPointers), reinterpret_cast<UINT*>(s_pNullPointers), reinterpret_cast<UINT*>(s_pNullPointers) );
		//pContext->OMSetRenderTargets( 0u, nullptr, nullptr );

		TIKI_NOT_IMPLEMENTED;
	}
}