
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
		static bool initWaitForGpu( GraphicsSystemPlatformData& data );

		static void resetDeviceState( const GraphicsSystemPlatformData& data );
	}

	GraphicsSystemPlatformData& graphics::getPlatformData( GraphicsSystem& graphicSystem )
	{
		return *(GraphicsSystemPlatformData*)addPtr( &graphicSystem, sizeof( uint ) );
	}

	ID3D12Device*				graphics::getDevice( GraphicsSystem& graphicsSystem )		{ return getPlatformData( graphicsSystem ).pDevice; }
	ID3D12GraphicsCommandList*	graphics::getCommandList( GraphicsSystem& graphicsSystem )	{ return getPlatformData( graphicsSystem ).pCommandList; }
	UploadHeapD3d12&			graphics::getUploadHeap( GraphicsSystem& graphicsSystem )	{ return getPlatformData( graphicsSystem ).uploadHeap; }
	
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

		if( !graphics::initObjects( m_platformData, params ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create D3D Objects.\n" );
			disposePlatform();
			return false;
		}

		if( !m_platformData.uploadHeap.create( m_platformData.pDevice, GraphicsSystemLimits_MaxUploadHeapSize ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create UploadHeap.\n" );
			disposePlatform();
			return false;
		}

		if( !graphics::initBackBuffer( m_platformData ) )
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

		// create back buffer target
		{
			m_backBufferTarget.m_width	= params.backBufferWidth;
			m_backBufferTarget.m_height	= params.backBufferHeight;

			m_backBufferTarget.m_colorBuffers[ 0u ].format		= PixelFormat_R8G8B8A8_Gamma;
			m_backBufferTarget.m_colorBuffers[ 0u ].pDataBuffer = nullptr;
			m_backBufferTarget.m_platformData.pColorHeap	= m_platformData.pBackBufferColorDescriptionHeap;
			m_backBufferTarget.m_colorBufferCount = 1u;

			m_backBufferTarget.m_depthBuffer.format			= PixelFormat_Depth24Stencil8;
			m_backBufferTarget.m_depthBuffer.pDataBuffer	= nullptr;
			m_backBufferTarget.m_platformData.pDepthHeap	= m_platformData.pBackBufferDepthDescriptionHeap;
		}

		if ( !m_commandBuffer.create( *this ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create CommandBuffer.\n" );
			disposePlatform();
			return false;
		}

		if ( !graphics::initWaitForGpu( m_platformData ) )
		{
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

			m_backBufferTarget.m_colorBuffers[ 0u ].clear();
			m_backBufferTarget.m_platformData.pColorHeap = nullptr;
			m_backBufferTarget.m_colorBufferCount = 0u;

			m_backBufferTarget.m_depthBuffer.clear();
			m_backBufferTarget.m_platformData.pDepthHeap = nullptr;
		}

		m_commandBuffer.dispose( *this );
		
		if ( m_platformData.pCommandQueue != nullptr )
		{
			// wait for the GPU to be done with all resources
			const UINT64 fenceToWait = m_platformData.currentFench;
			const UINT64 lastCompletedFence = m_platformData.pFence->GetCompletedValue();

			// Signal and increment the current fence
			m_platformData.pCommandQueue->Signal( m_platformData.pFence, m_platformData.currentFench);
			m_platformData.currentFench++;

			if ( lastCompletedFence < fenceToWait )
			{
				TIKI_VERIFY( m_platformData.pFence->SetEventOnCompletion( fenceToWait, m_platformData.waitEventHandle ) );
				WaitForSingleObject( m_platformData.waitEventHandle, INFINITE );
			}

			CloseHandle( m_platformData.waitEventHandle );
		}

		if( m_platformData.pSwapChain != nullptr )
		{
			m_platformData.pSwapChain->SetFullscreenState( false, nullptr );
		}

		m_platformData.uploadHeap.dispose();

		graphics::safeRelease( &m_platformData.pBackBufferDepth );
		graphics::safeRelease( &m_platformData.pBackBufferDepthDescriptionHeap );
		graphics::safeRelease( &m_platformData.pBackBufferColor );
		graphics::safeRelease( &m_platformData.pBackBufferColorDescriptionHeap );

		graphics::safeRelease( &m_platformData.pCommandList );
		graphics::safeRelease( &m_platformData.pRootSignature );
		graphics::safeRelease( &m_platformData.pCommandAllocator );
		graphics::safeRelease( &m_platformData.pCommandQueue );
		graphics::safeRelease( &m_platformData.pDevice );
		graphics::safeRelease( &m_platformData.pSwapChain );
	}

	bool GraphicsSystem::resize( uint width, uint height )
	{
		if ( width == 0u || height == 0u )
		{
			return false;
		}

		m_platformData.pCommandList->SetRenderTargets( nullptr, false, 0u, nullptr );

		graphics::safeRelease( &m_platformData.pBackBufferDepth );
		graphics::safeRelease( &m_platformData.pBackBufferColor );

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
		m_backBufferTarget.m_platformData.pColorHeap	= m_platformData.pBackBufferColorDescriptionHeap;
		m_backBufferTarget.m_platformData.pDepthHeap	= m_platformData.pBackBufferDepthDescriptionHeap;

		return true;
	}

	GraphicsContext& GraphicsSystem::beginFrame()
	{
		//const UINT64 lastCompletedFence = m_platformData.pFence->GetCompletedValue();
		//if (m_platformData.currentFench != 0u && m_platformData.currentFench > lastCompletedFence)
		//{
		//	TIKI_VERIFY( SUCCEEDED(m_platformData.pFence->SetEventOnCompletion( m_platformData.currentFench, m_platformData.waitEventHandle ) ) );
		//	WaitForSingleObject( m_platformData.waitEventHandle, INFINITE );
		//}

		m_frameNumber++;
		
		TIKI_VERIFY( SUCCEEDED( m_platformData.pCommandList->Close() ) );

		ID3D12CommandList* pCommandList = m_platformData.pCommandList;
		m_platformData.pCommandQueue->ExecuteCommandLists( 1u, &pCommandList );

		TIKI_VERIFY( SUCCEEDED( m_platformData.pCommandAllocator->Reset() ) );
		TIKI_VERIFY( SUCCEEDED( m_platformData.pCommandList->Reset( m_platformData.pCommandAllocator, nullptr ) ) );

		graphics::setResourceBarrier(
			m_platformData.pCommandList,
			m_platformData.pBackBufferColor,
			D3D12_RESOURCE_USAGE_PRESENT,
			D3D12_RESOURCE_USAGE_RENDER_TARGET
		);

		m_platformData.pCommandList->SetGraphicsRootSignature( m_platformData.pRootSignature );

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

		TIKI_VERIFY( SUCCEEDED( m_platformData.pCommandList->Close() ) );
		ID3D12CommandList* apCommandList[] = { m_platformData.pCommandList };
		m_platformData.pCommandQueue->ExecuteCommandLists( TIKI_COUNT( apCommandList ), apCommandList );
		
		TIKI_VERIFY( SUCCEEDED( m_platformData.pSwapChain->Present( 1, 0 ) ) );
		
		m_platformData.uploadHeap.finalizeFrame( (uint)m_platformData.currentFench );

		// signal and increment the current fence
		m_platformData.pCommandQueue->Signal( m_platformData.pFence, m_platformData.currentFench );
		m_platformData.currentFench++;

		m_platformData.currentSwapBufferIndex = (m_platformData.currentSwapBufferIndex + 1u) % m_platformData.swapBufferCount;

		m_platformData.pBackBufferColor->Release();
		m_platformData.pSwapChain->GetBuffer( (UINT)m_platformData.currentSwapBufferIndex, IID_PPV_ARGS( &m_platformData.pBackBufferColor ) );
		m_platformData.pDevice->CreateRenderTargetView( m_platformData.pBackBufferColor, nullptr, m_platformData.pBackBufferColorDescriptionHeap->GetCPUDescriptorHandleForHeapStart() );
		
		graphics::resetDeviceState( m_platformData );
	}

	static bool graphics::initSwapChain( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params, const uint2& backBufferSize )
	{
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
		
		HRESULT result = D3D12CreateDevice(
			nullptr,
			rendererType,
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
			D3D12_CREATE_DEVICE_DEBUG,
#else
			D3D12_CREATE_DEVICE_NONE,
#endif
			D3D_FEATURE_LEVEL_9_1,
			D3D12_SDK_VERSION,
			IID_PPV_ARGS( &data.pDevice)
		);
		
		if ( FAILED( result ) )
		{
			return false;
		}

		IDXGIFactory1* pDxgiFactory = nullptr;
		if ( FAILED( CreateDXGIFactory1( IID_PPV_ARGS( &pDxgiFactory ) ) ) )
		{
			return false;
		}

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
		//swapDesc.Flags							= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		data.swapBufferCount = swapDesc.BufferCount;

		result = pDxgiFactory->CreateSwapChain(
			data.pDevice,
			&swapDesc,
			&data.pSwapChain
		);

		safeRelease( &pDxgiFactory );

		return SUCCEEDED( result );
	}
	
	static bool graphics::initObjects( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params )
	{
		TIKI_DECLARE_STACKANDZERO( D3D12_COMMAND_QUEUE_DESC, queueDesc );
		queueDesc.Flags	= D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type	= D3D12_COMMAND_LIST_TYPE_DIRECT;
		if ( FAILED( data.pDevice->CreateCommandQueue( &queueDesc, IID_PPV_ARGS( &data.pCommandQueue ) ) ) )
		{
			return false;
		}

		if( FAILED( data.pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &data.pCommandAllocator ) ) ) )
		{
			return false;
		}

		if( FAILED( data.pDevice->CreateCommandList( 1u, D3D12_COMMAND_LIST_TYPE_DIRECT, data.pCommandAllocator, nullptr, IID_PPV_ARGS( &data.pCommandList ) ) ) )
		{
			return false;
		}

		D3D12_DESCRIPTOR_RANGE descRanges[ 3u ];
		descRanges[ 0u ].Init( D3D12_DESCRIPTOR_RANGE_TYPE_SRV, GraphicsSystemLimits_VertexShaderTextureSlots + GraphicsSystemLimits_PixelShaderTextureSlots, 0 );
		descRanges[ 1u ].Init( D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, GraphicsSystemLimits_VertexShaderTextureSlots + GraphicsSystemLimits_PixelShaderTextureSlots, 0 );
		descRanges[ 2u ].Init( D3D12_DESCRIPTOR_RANGE_TYPE_CBV, GraphicsSystemLimits_VertexShaderConstantSlots + GraphicsSystemLimits_PixelShaderConstantSlots, 0 );

		D3D12_ROOT_PARAMETER rootParameters[ 3u ];
		rootParameters[ 0u ].InitAsDescriptorTable( 1u, &descRanges[ 0u ], D3D12_SHADER_VISIBILITY_ALL );
		rootParameters[ 1u ].InitAsDescriptorTable( 1u, &descRanges[ 1u ], D3D12_SHADER_VISIBILITY_ALL );
		rootParameters[ 2u ].InitAsDescriptorTable( 1u, &descRanges[ 2u ], D3D12_SHADER_VISIBILITY_ALL );

		D3D12_ROOT_SIGNATURE_DESC descRootSignature;
		descRootSignature.Init( TIKI_COUNT( rootParameters ), rootParameters, 0u, nullptr, D3D12_ROOT_SIGNATURE_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT );

		ID3DBlob* pOutputBlob	= nullptr;
		ID3DBlob* pErrorBlob	= nullptr;
		if( FAILED( D3D12SerializeRootSignature( &descRootSignature, D3D_ROOT_SIGNATURE_V1, &pOutputBlob, &pErrorBlob ) ) )
		{
			TIKI_ASSERT( pOutputBlob == nullptr );

			if( pErrorBlob != nullptr )
			{
				TIKI_TRACE_ERROR( "Failed to serialize RootSignature. Error: %s", (const char*)pErrorBlob->GetBufferPointer() );
				pErrorBlob->Release();
				pErrorBlob = nullptr;
			}

			return false;
		}
		TIKI_ASSERT( pOutputBlob != nullptr );
		TIKI_ASSERT( pErrorBlob == nullptr );

		HRESULT result = data.pDevice->CreateRootSignature( 1u, pOutputBlob->GetBufferPointer(), pOutputBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&data.pRootSignature );

		pOutputBlob->Release();
		pOutputBlob = nullptr;

		if( FAILED( result ) )
		{
			return false;
		}

		result = data.pDevice->CreateFence( 0u, D3D12_FENCE_MISC_NONE, IID_PPV_ARGS( &data.pFence ) );
		if ( FAILED( result ) )
		{
			return false;
		}

		return true;
	}

	static bool graphics::initBackBuffer( GraphicsSystemPlatformData& data )
	{		
		TIKI_DECLARE_STACKANDZERO( D3D12_DESCRIPTOR_HEAP_DESC, descHeap );
		descHeap.NumDescriptors	= 1u;
		descHeap.Type			= D3D12_RTV_DESCRIPTOR_HEAP;

		if( FAILED( data.pDevice->CreateDescriptorHeap( &descHeap, __uuidof(ID3D12DescriptorHeap), (void**)&data.pBackBufferColorDescriptionHeap ) ) )
		{
			return false;
		}

		HRESULT result = data.pSwapChain->GetBuffer( 0, __uuidof(ID3D12Resource), (void**)&data.pBackBufferColor );
		if( FAILED( result ) )
		{
			return false;
		}
		data.pDevice->CreateRenderTargetView( data.pBackBufferColor, nullptr, data.pBackBufferColorDescriptionHeap->GetCPUDescriptorHandleForHeapStart( ) );

		return true;
	}

	static bool graphics::initDepthStencilBuffer( GraphicsSystemPlatformData& data, const uint2& backBufferSize )
	{
		TIKI_DECLARE_STACKANDZERO( D3D12_DEPTH_STENCIL_VIEW_DESC, depthStencilDesc );
		depthStencilDesc.Format			= DXGI_FORMAT_D32_FLOAT;
		depthStencilDesc.ViewDimension	= D3D12_DSV_DIMENSION_TEXTURE2D;
		depthStencilDesc.Flags			= D3D12_DSV_NONE;

		TIKI_DECLARE_STACKANDZERO( D3D12_CLEAR_VALUE, depthClearValue );
		depthClearValue.Format				= DXGI_FORMAT_D32_FLOAT;
		depthClearValue.DepthStencil.Depth	= 1.0f;

		HRESULT result = data.pDevice->CreateCommittedResource(
			&CD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_MISC_NONE,
			&CD3D12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, backBufferSize.x, backBufferSize.y, 1u, 0u, 1u, 0u, D3D12_RESOURCE_MISC_ALLOW_DEPTH_STENCIL),
			D3D12_RESOURCE_USAGE_INITIAL,
			&depthClearValue,
			IID_PPV_ARGS( &data.pBackBufferDepth )
		);
		
		//HRESULT result = data.pDevice->CreateDefaultResource(
		//	&CD3D11_RESOURCE_DESC( CD3D12_RESOURCE_DESC::Tex2D( DXGI_FORMAT_R24G8_TYPELESS, backBufferSize.x, backBufferSize.y, 1u, 0u, 1u, 0u, D3D12_RESOURCE_MISC_NO_STREAM_OUTPUT | D3D12_RESOURCE_MISC_NO_UNORDERED_ACCESS | D3D12_RESOURCE_MISC_DEPTH_STENCIL ) ),
		//	nullptr,
		//	IID_PPV_ARGS( &data.pBackBufferDepth )
		//);

		if( FAILED( result ) )
		{
			return false;
		}

		// create descriptor heap
		{
			TIKI_DECLARE_STACKANDZERO( D3D12_DESCRIPTOR_HEAP_DESC, heapDesc );
			heapDesc.NumDescriptors = 1u;
			heapDesc.Type			= D3D12_DSV_DESCRIPTOR_HEAP;

			result = data.pDevice->CreateDescriptorHeap( &heapDesc, IID_PPV_ARGS( &data.pBackBufferDepthDescriptionHeap ) );
			if( FAILED( result ) )
			{
				return false;
			}
		}

		TIKI_DECLARE_STACKANDZERO( D3D12_DEPTH_STENCIL_VIEW_DESC, depthStencilViewDesc );
		depthStencilViewDesc.Format				= DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.ViewDimension		= D3D12_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice	= 0u;

		data.pDevice->CreateDepthStencilView( data.pBackBufferDepth, &depthStencilViewDesc, data.pBackBufferDepthDescriptionHeap->GetCPUDescriptorHandleForHeapStart() );
		graphics::setResourceBarrier( data.pCommandList, data.pBackBufferDepth, D3D12_RESOURCE_USAGE_INITIAL, D3D12_RESOURCE_USAGE_DEPTH );

		return true;
	}
	
	bool graphics::initWaitForGpu( GraphicsSystemPlatformData& data )
	{
		data.waitEventHandle = CreateEventEx( nullptr, false, false, EVENT_ALL_ACCESS );

		const uint64 fenchToWait = data.currentFench;
		if ( FAILED( data.pCommandQueue->Signal( data.pFence, fenchToWait ) ) )
		{
			return false;
		}
		data.currentFench++;

		if ( FAILED( data.pFence->SetEventOnCompletion( fenchToWait, data.waitEventHandle ) ) )
		{
			return false;
		}
		WaitForSingleObject( data.waitEventHandle, INFINITE );

		return true;
	}

	void graphics::setResourceBarrier( ID3D12GraphicsCommandList* pCommandList, ID3D12Resource* pResource, UINT stateBefore, UINT stateAfter )
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