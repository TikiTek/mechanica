
#include "tiki/graphics/graphicssystem.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/vertexformat.hpp"

#include "graphicssystem_internal_d3d12.hpp"

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
#	include <dxgidebug.h>
#endif

namespace tiki
{
	namespace graphics
	{
		static bool initDevice( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params );
		static bool initSwapChain( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params, const uint2& backBufferSize );
		static bool initFrame( GraphicsSystemPlatformData& data, GraphicsSystemFrame& frame, uintreg frameIndex, DescriptorHandle backBufferColorHandle );
		static bool initObjects( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params );
	}

	bool GraphicsSystem::createPlatform( const GraphicsSystemParameters& params )
	{
		uint2 backBufferSize;
		backBufferSize.x = (uint32)params.backBufferWidth;
		backBufferSize.y = (uint32)params.backBufferHeight;

		m_frameNumber = 0;

		if( params.fullScreen )
		{
			RECT rect;
			HWND hDesktop = GetDesktopWindow();
			GetWindowRect( hDesktop, &rect );

			backBufferSize.x = (rect.left - rect.right);
			backBufferSize.y = (rect.top - rect.bottom);
		}

		if( !graphics::initDevice( m_platformData, params ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create Device.\n" );
			disposePlatform();
			return false;
		}

		for( uintreg i = 0u; i < TIKI_COUNT( m_platformData.frames ); ++i )
		{
			if( FAILED( m_platformData.pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &m_platformData.frames[ i ].pRenderCommandAllocator ) ) ) ||
				FAILED( m_platformData.pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &m_platformData.frames[ i ].pResourceCommandAllocator ) ) ) )
			{
				TIKI_TRACE_ERROR( "[graphics] Could not create CommandAllocator.\n" );
				disposePlatform();
				return false;
			}
			TIKI_SET_DX_OBJECT_NAME( m_platformData.frames[ i ].pRenderCommandAllocator, "RenderCommandAllocator" );
			TIKI_SET_DX_OBJECT_NAME( m_platformData.frames[ i ].pResourceCommandAllocator, "ResourceCommandAllocator" );
		}

		if( !graphics::initObjects( m_platformData, params ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create D3D Objects.\n" );
			disposePlatform();
			return false;
		}

		if( !graphics::initSwapChain( m_platformData, params, backBufferSize ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create SwapChain.\n" );
			disposePlatform();
			return false;
		}

		if( !m_platformData.uploadHeap.create( m_platformData.pDevice, GraphicsSystemLimits_MaxUploadHeapSize, 128u, "UploadHeap" ) ||
			!m_platformData.shaderResourcePool.create( m_platformData.pDevice, GraphicsSystemLimits_MaxShaderResourceViews, DescriptorType_ShaderResource, "ShaderResourcePool" ) ||
			!m_platformData.samplerPool.create( m_platformData.pDevice, GraphicsSystemLimits_MaxSamplers, DescriptorType_Sampler, "SamplerPool" ) ||
			!m_platformData.renderTargetPool.create( m_platformData.pDevice, GraphicsSystemLimits_MaxRenderTargetViews, DescriptorType_RenderTarget, "RenderTargetPool" ) ||
			!m_platformData.depthStencilPool.create( m_platformData.pDevice, GraphicsSystemLimits_MaxDepthStencilViews, DescriptorType_DepthStencil, "DepthStencilPool" ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create upload heap or descriptor pools.\n" );
			disposePlatform();
			return false;
		}

		// create frames
		for( uintreg i = 0u; i < TIKI_COUNT( m_platformData.frames ); ++i )
		{
			DescriptorHandle backBufferColorHandle = (i == 0u ? m_backBufferTarget.m_platformData.colorHandles[ 0u ] : InvalidDescriptorHandle);
			if( !graphics::initFrame( m_platformData, m_platformData.frames[ i ], i, backBufferColorHandle ) )
			{
				disposePlatform();
				return false;
			}
		}

		// create back buffer target
		{
			TextureDescription depthDescription;
			depthDescription.width		= backBufferSize.x;
			depthDescription.height		= backBufferSize.y;
			depthDescription.format		= PixelFormat_Depth24Stencil8;
			depthDescription.flags		= TextureFlags_DepthStencil;
			depthDescription.type		= TextureType_2d;

			if( !m_backBufferDepthData.create( *this, depthDescription, nullptr, "BackBufferDepth" ) )
			{
				TIKI_TRACE_ERROR( "[graphics] Could not create DepthStencilBuffer.\n" );
				disposePlatform();
				return false;
			}

			TextureDescription colorDescription;
			colorDescription.width		= backBufferSize.x;
			colorDescription.height		= backBufferSize.y;
			colorDescription.format		= PixelFormat_R8G8B8A8;
			colorDescription.flags		= TextureFlags_RenderTarget;
			colorDescription.type		= TextureType_2d;

			m_backBufferColorData.m_description				= colorDescription;
			m_backBufferColorData.m_platformData.pResource	= m_platformData.frames[ m_platformData.currentSwapBufferIndex ].pBackBufferColorResouce;

			RenderTargetBuffer colorBuffer( m_backBufferColorData );
			RenderTargetBuffer depthBuffer( m_backBufferDepthData );
			if( !m_backBufferTarget.create( *this, backBufferSize.x, backBufferSize.y, &colorBuffer, 1u, &depthBuffer ) )
			{
				TIKI_TRACE_ERROR( "[graphics] Could not create BackBuffer/DepthStencilBuffer.\n" );
				disposePlatform();
				return false;
			}
		}

		if ( !m_commandBuffer.create( *this ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create CommandBuffer.\n" );
			disposePlatform();
			return false;
		}

		m_platformData.waitEventHandle = CreateEvent( nullptr, false, false, nullptr );
		m_platformData.isInFrame = true;

		if ( !GraphicsSystemPlatform::waitForGpu( *this ) )
		{
			disposePlatform();
			return false;
		}

		m_platformData.isInFrame = false;

		return true;
	}

	void GraphicsSystem::disposePlatform()
	{
		GraphicsSystemPlatform::waitForGpu( *this );

		m_backBufferTarget.dispose( *this );
		m_backBufferDepthData.dispose( *this );
		GraphicsSystemPlatform::safeRelease( &m_backBufferColorData.m_platformData.pResource );

		m_commandBuffer.dispose( *this );
		
		if ( m_platformData.waitEventHandle != INVALID_HANDLE_VALUE )
		{
			CloseHandle( m_platformData.waitEventHandle );
			m_platformData.waitEventHandle = INVALID_HANDLE_VALUE;
		}

		if( m_platformData.pSwapChain != nullptr )
		{
			m_platformData.pSwapChain->SetFullscreenState( FALSE, nullptr );
		}

		m_platformData.uploadHeap.dispose();

		for( uintreg i = 0u; i < TIKI_COUNT( m_platformData.frames ); ++i )
		{
			GraphicsSystemFrame& frame = m_platformData.frames[ i ];

			DynamicBufferData* pBuffer = frame.pFirstDynamicBuffer;
			while( pBuffer != nullptr )
			{
				DynamicBufferData* pNext = pBuffer->pNext;
				GraphicsSystemPlatform::disposeDynamicBuffer( pBuffer );
				pBuffer = pNext;
			}
			frame.pFirstDynamicBuffer = nullptr;
			frame.pLastDynamicBuffer = nullptr;

			GraphicsSystemPlatform::safeRelease( &frame.pRenderCommandAllocator );
			GraphicsSystemPlatform::safeRelease( &frame.pResourceCommandAllocator );
			GraphicsSystemPlatform::safeRelease( &frame.pBackBufferColorResouce );

			if( i != 0u && frame.backBufferColorHandle != InvalidDescriptorHandle )
			{
				m_platformData.renderTargetPool.freeDescriptor( frame.backBufferColorHandle );
				frame.backBufferColorHandle = InvalidDescriptorHandle;
			}
		}

		m_platformData.shaderResourcePool.dispose();
		m_platformData.samplerPool.dispose();
		m_platformData.renderTargetPool.dispose();
		m_platformData.depthStencilPool.dispose();

		GraphicsSystemPlatform::safeRelease( &m_platformData.pFence );
		GraphicsSystemPlatform::safeRelease( &m_platformData.pRootSignature );
		GraphicsSystemPlatform::safeRelease( &m_platformData.pRenderCommandList );
		GraphicsSystemPlatform::safeRelease( &m_platformData.pResourceCommandList );
		GraphicsSystemPlatform::safeRelease( &m_platformData.pSwapChain );
		GraphicsSystemPlatform::safeRelease( &m_platformData.pCommandQueue );
		GraphicsSystemPlatform::safeRelease( &m_platformData.pDevice );
		GraphicsSystemPlatform::safeRelease( &m_platformData.pFactory );

		m_platformData.resourceCommandListMutex.dispose();

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		IDXGIDebug* pDebug = nullptr;
		if( SUCCEEDED( DXGIGetDebugInterface1( 0u, IID_PPV_ARGS( &pDebug ) ) ) )
		{
			pDebug->ReportLiveObjects( DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL );
			GraphicsSystemPlatform::safeReleaseBla( &pDebug );
		}
#endif
	}

	bool GraphicsSystem::resize( uintreg width, uintreg height )
	{
		if( m_backBufferTarget.getWidth() == width && m_backBufferTarget.getHeight() == height )
		{
			return true;
		}

		if ( width == 0u || height == 0u )
		{
			return false;
		}

		m_platformData.pRenderCommandList->ClearState( nullptr );
		GraphicsSystemPlatform::waitForGpu( *this );

		m_backBufferTarget.dispose( *this );
		m_backBufferDepthData.dispose( *this );
		GraphicsSystemPlatform::safeRelease( &m_backBufferColorData.m_platformData.pResource );

		HRESULT result = m_platformData.pSwapChain->ResizeBuffers( 0, UINT( width ), UINT( height ), DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH );
		if ( FAILED( result ) )
		{
			dispose();
			return false;
		}

		TextureDescription depthDescription;
		depthDescription.width		= (uint16)width;
		depthDescription.height		= (uint16)height;
		depthDescription.format		= PixelFormat_Depth24Stencil8;
		depthDescription.flags		= TextureFlags_DepthStencil;
		depthDescription.type		= TextureType_2d;

		if( !m_backBufferDepthData.create( *this, depthDescription ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create DepthStencilBuffer.\n" );
			return false;
		}

		result = m_platformData.pSwapChain->GetBuffer( 0, __uuidof(ID3D12Resource), (void**)&m_backBufferColorData.m_platformData.pResource );
		if( FAILED( result ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not get BackBuffer.\n" );
			return false;
		}

		RenderTargetBuffer colorBuffer( m_backBufferColorData );
		RenderTargetBuffer depthBuffer( m_backBufferDepthData );
		if( !m_backBufferTarget.create( *this, width, height, &colorBuffer, 1u, &depthBuffer ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not create BackBuffer/DepthStencilBuffer.\n" );
			disposePlatform();
			return false;
		}

		return true;
	}

	GraphicsContext& GraphicsSystem::beginFrame()
	{
		m_frameNumber++;
			
		GraphicsSystemFrame& frame = m_platformData.frames[ m_platformData.currentSwapBufferIndex ];
		TIKI_VERIFY( SUCCEEDED( frame.pRenderCommandAllocator->Reset() ) );
		TIKI_VERIFY( SUCCEEDED( m_platformData.pRenderCommandList->Reset( frame.pRenderCommandAllocator, nullptr ) ) );

		GraphicsSystemPlatform::setResourceBarrier(
			m_platformData.pRenderCommandList,
			frame.pBackBufferColorResouce,
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		);

		m_platformData.pRenderCommandList->SetGraphicsRootSignature( m_platformData.pRootSignature );

		ID3D12DescriptorHeap* apDescriptorHeaps[] =
		{
			m_platformData.shaderResourcePool.getHeap(),
			m_platformData.samplerPool.getHeap()
		};
		m_platformData.pRenderCommandList->SetDescriptorHeaps( TIKI_COUNT( apDescriptorHeaps ), apDescriptorHeaps );

		if( frame.pFirstDynamicBuffer != nullptr && (frame.requiredDynamicBufferSize > frame.pFirstDynamicBuffer->size || frame.requiredDynamicBufferSize < frame.pFirstDynamicBuffer->size / 3) )
		{
			DynamicBufferData* pBuffer = frame.pFirstDynamicBuffer;
			while( pBuffer != nullptr )
			{
				DynamicBufferData* pNext = pBuffer->pNext;
				GraphicsSystemPlatform::disposeDynamicBuffer( pBuffer );
				pBuffer = pNext;
			}

			frame.pFirstDynamicBuffer = GraphicsSystemPlatform::createDynamicBuffer( *this, frame.requiredDynamicBufferSize * 2 );
			frame.pLastDynamicBuffer = frame.pFirstDynamicBuffer;
		}
		else if ( frame.pFirstDynamicBuffer != nullptr )
		{
			DynamicBufferData* pBuffer = frame.pFirstDynamicBuffer;
			while( pBuffer != nullptr )
			{
				pBuffer->currentSize = 0u;
				pBuffer = pBuffer->pNext;
			}			
		}
		frame.requiredDynamicBufferSize = 0u;

		m_platformData.isInFrame = true;

		m_commandBuffer.beginFrame();

		return m_commandBuffer;
	}

	void GraphicsSystem::endFrame()
	{
		m_commandBuffer.endFrame();

		GraphicsSystemFrame& prevFrame = m_platformData.frames[ m_platformData.currentSwapBufferIndex ];

		GraphicsSystemPlatform::setResourceBarrier(
			m_platformData.pRenderCommandList,
			prevFrame.pBackBufferColorResouce,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		);

		// execute command list
		m_platformData.resourceCommandListMutex.lock();

		TIKI_VERIFY( SUCCEEDED( m_platformData.pRenderCommandList->Close() ) );
		TIKI_VERIFY( SUCCEEDED( m_platformData.pResourceCommandList->Close() ) );
		ID3D12CommandList* apCommandLists[] =
		{
			m_platformData.pResourceCommandList,
			m_platformData.pRenderCommandList,
		};
		m_platformData.pCommandQueue->ExecuteCommandLists( TIKI_COUNT( apCommandLists ), apCommandLists );

		TIKI_VERIFY( SUCCEEDED( m_platformData.pSwapChain->Present( 1, 0 ) ) );
		
		m_platformData.uploadHeap.finalizeFrame( (uintreg)prevFrame.currentFence );

		// swap frame
		m_platformData.currentSwapBufferIndex = m_platformData.pSwapChain->GetCurrentBackBufferIndex();
		GraphicsSystemFrame& nextFrame = m_platformData.frames[ m_platformData.currentSwapBufferIndex ];

		// signal and increment the current fence
		const uint64 currentFenceValue = prevFrame.currentFence;
		m_platformData.pCommandQueue->Signal( m_platformData.pFence, currentFenceValue );
	
		if( m_platformData.pFence->GetCompletedValue() < nextFrame.currentFence )
		{
			TIKI_VERIFY( SUCCEEDED( m_platformData.pFence->SetEventOnCompletion( nextFrame.currentFence, m_platformData.waitEventHandle ) ) );
			WaitForSingleObjectEx( m_platformData.waitEventHandle, INFINITE, FALSE );
		}
		nextFrame.currentFence = currentFenceValue + 1u;

		// start resource command list
		TIKI_VERIFY( SUCCEEDED( nextFrame.pResourceCommandAllocator->Reset() ) );
		TIKI_VERIFY( SUCCEEDED( m_platformData.pResourceCommandList->Reset( nextFrame.pResourceCommandAllocator, nullptr ) ) );
		m_platformData.resourceCommandListMutex.unlock();

		// set back buffer target
		m_backBufferColorData.m_platformData.pResource = nextFrame.pBackBufferColorResouce;
		m_backBufferTarget.m_platformData.colorHandles[ 0u ] = nextFrame.backBufferColorHandle;

		m_platformData.isInFrame = false;
	}

	static bool graphics::initDevice( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params )
	{
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		ID3D12Debug* pDebug = nullptr;
		if( SUCCEEDED( D3D12GetDebugInterface( IID_PPV_ARGS( &pDebug ) ) ) )
		{
			pDebug->EnableDebugLayer();
			GraphicsSystemPlatform::safeReleaseBla( &pDebug );
		}
#endif

		if( FAILED( CreateDXGIFactory1( IID_PPV_ARGS( &data.pFactory ) ) ) )
		{
			return false;
		}
		TIKI_SET_DX_OBJECT_NAME( data.pFactory, "Factory" );
		
		IDXGIAdapter1* pAdapter = nullptr;
		IDXGIAdapter1* pCurrentAdapter = nullptr;
		for( UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != data.pFactory->EnumAdapters1( adapterIndex, &pCurrentAdapter ); ++adapterIndex )
		{
			DXGI_ADAPTER_DESC1 desc;
			pCurrentAdapter->GetDesc1( &desc );

			if( desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE )
			{
				continue;
			}

			pAdapter = pCurrentAdapter;
		}

		HRESULT result = D3D12CreateDevice(
			pAdapter,
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS( &data.pDevice )
		);
		TIKI_SET_DX_OBJECT_NAME( data.pDevice, "Device" );

		GraphicsSystemPlatform::safeRelease( &pAdapter );

		if( FAILED( result ) )
		{
			return false;
		}
		
		return true;
	}

	static bool graphics::initSwapChain( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params, const uint2& backBufferSize )
	{
		TIKI_DECLARE_STACKANDZERO( DXGI_SWAP_CHAIN_DESC1, swapChainDesc );
		swapChainDesc.BufferCount		= 2u;
		swapChainDesc.Width				= backBufferSize.x;
		swapChainDesc.Height			= backBufferSize.y;
		swapChainDesc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect		= DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count	= 1;
		swapChainDesc.Flags				= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		TIKI_DECLARE_STACKANDZERO( DXGI_SWAP_CHAIN_FULLSCREEN_DESC, fullscreenDesc );
		fullscreenDesc.Windowed = !params.fullScreen;
		
		IDXGISwapChain1* pSwapChain = nullptr;
		HRESULT result = data.pFactory->CreateSwapChainForHwnd(
			data.pCommandQueue,
			(HWND)params.pWindowHandle,
			&swapChainDesc,
			nullptr,
			nullptr,
			&pSwapChain
		);

		if( FAILED( result ) )
		{
			return false;
		}
		TIKI_SET_DX_OBJECT_NAME( pSwapChain, "SwapChain" );

		result = pSwapChain->QueryInterface( IID_PPV_ARGS( &data.pSwapChain ) );
		GraphicsSystemPlatform::safeRelease( &pSwapChain );

		return SUCCEEDED( result );
	}

	bool graphics::initFrame( GraphicsSystemPlatformData& data, GraphicsSystemFrame& frame, uintreg frameIndex, DescriptorHandle backBufferColorHandle )
	{
		HRESULT result = data.pSwapChain->GetBuffer( (UINT)frameIndex, IID_PPV_ARGS( &frame.pBackBufferColorResouce ) );
		if( FAILED( result ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Could not get BackBuffer.\n" );
			return false;
		}
		TIKI_SET_DX_OBJECT_NAME( frame.pBackBufferColorResouce, "BackBuffer" );

		if( backBufferColorHandle != InvalidDescriptorHandle )
		{
			frame.backBufferColorHandle = backBufferColorHandle;
		}
		else
		{
			frame.backBufferColorHandle = data.renderTargetPool.allocateDescriptor();
			if( frame.backBufferColorHandle == InvalidDescriptorHandle )
			{
				return false;
			}

			data.pDevice->CreateRenderTargetView( frame.pBackBufferColorResouce, nullptr, data.renderTargetPool.getCpuHandle( frame.backBufferColorHandle ) );
		}
		
		return true;
	}
	
	static bool graphics::initObjects( GraphicsSystemPlatformData& data, const GraphicsSystemParameters& params )
	{
		GraphicsSystemFrame& frame = data.frames[ data.currentSwapBufferIndex ];

		TIKI_DECLARE_STACKANDZERO( D3D12_COMMAND_QUEUE_DESC, queueDesc );
		queueDesc.Flags	= D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type	= D3D12_COMMAND_LIST_TYPE_DIRECT;
		if ( FAILED( data.pDevice->CreateCommandQueue( &queueDesc, IID_PPV_ARGS( &data.pCommandQueue ) ) ) )
		{
			return false;
		}
		TIKI_SET_DX_OBJECT_NAME( data.pCommandQueue, "CommandQueue" );

		data.resourceCommandListMutex.create();

		if( FAILED( data.pDevice->CreateCommandList( 1u, D3D12_COMMAND_LIST_TYPE_DIRECT, frame.pRenderCommandAllocator, nullptr, IID_PPV_ARGS( &data.pRenderCommandList ) ) ) || 
			FAILED( data.pDevice->CreateCommandList( 1u, D3D12_COMMAND_LIST_TYPE_DIRECT, frame.pResourceCommandAllocator, nullptr, IID_PPV_ARGS( &data.pResourceCommandList ) ) ) )
		{
			return false;
		}
		TIKI_SET_DX_OBJECT_NAME( data.pRenderCommandList, "RenderCommandList" );
		TIKI_SET_DX_OBJECT_NAME( data.pResourceCommandList, "ResourceCommandList" );

		FixedSizedArray< CD3DX12_DESCRIPTOR_RANGE, GraphicsDiscriptorIndex_Count > descRanges;
		FixedArray< CD3DX12_ROOT_PARAMETER, GraphicsDiscriptorIndex_Count > rootParameters;

		uintreg i = 0u;
		for( ; i < GraphicsDiscriptorIndex_FirstVertexTexture; ++i )
		{
			rootParameters[ i ].InitAsConstantBufferView( UINT( i - GraphicsDiscriptorIndex_FirstVertexConstant ), 0u, D3D12_SHADER_VISIBILITY_VERTEX );
		}

		for( ; i < GraphicsDiscriptorIndex_FirstVertexSampler; ++i )
		{
			descRanges.push( CD3DX12_DESCRIPTOR_RANGE( D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1u, UINT( i - GraphicsDiscriptorIndex_FirstVertexTexture ) ) );
			rootParameters[ i ].InitAsDescriptorTable( 1u, &descRanges.getLast(), D3D12_SHADER_VISIBILITY_VERTEX );
		}

		for( ; i < GraphicsDiscriptorIndex_FirstPixelConstant; ++i )
		{
			descRanges.push( CD3DX12_DESCRIPTOR_RANGE( D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1u, UINT( i - GraphicsDiscriptorIndex_FirstVertexSampler ) ) );
			rootParameters[ i ].InitAsDescriptorTable( 1u, &descRanges.getLast(), D3D12_SHADER_VISIBILITY_VERTEX );
		}

		for( ; i < GraphicsDiscriptorIndex_FirstPixelTexture; ++i )
		{
			rootParameters[ i ].InitAsConstantBufferView( UINT( i - GraphicsDiscriptorIndex_FirstPixelConstant ), 0u, D3D12_SHADER_VISIBILITY_PIXEL );
		}

		for( ; i < GraphicsDiscriptorIndex_FirstPixelSampler; ++i )
		{
			descRanges.push( CD3DX12_DESCRIPTOR_RANGE( D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1u, UINT( i - GraphicsDiscriptorIndex_FirstPixelTexture ) ) );
			rootParameters[ i ].InitAsDescriptorTable( 1u, &descRanges.getLast(), D3D12_SHADER_VISIBILITY_PIXEL );
		}

		for( ; i < GraphicsDiscriptorIndex_Count; ++i )
		{
			descRanges.push( CD3DX12_DESCRIPTOR_RANGE( D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1u, UINT( i - GraphicsDiscriptorIndex_FirstPixelSampler ) ) );
			rootParameters[ i ].InitAsDescriptorTable( 1u, &descRanges.getLast(), D3D12_SHADER_VISIBILITY_PIXEL );
		}

		CD3DX12_ROOT_SIGNATURE_DESC descRootSignature;
		descRootSignature.Init( (UINT)i, rootParameters.getBegin(), 0u, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT );

		ID3DBlob* pOutputBlob	= nullptr;
		ID3DBlob* pErrorBlob	= nullptr;
		if( FAILED( D3D12SerializeRootSignature( &descRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &pOutputBlob, &pErrorBlob ) ) )
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
		TIKI_SET_DX_OBJECT_NAME( data.pRootSignature, "RootSignature" );

		result = data.pDevice->CreateFence( 0u, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS( &data.pFence ) );
		if ( FAILED( result ) )
		{
			return false;
		}
		TIKI_SET_DX_OBJECT_NAME( data.pFence, "Fence" );

		return true;
	}
}