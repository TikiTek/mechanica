#include "graphicssystem_internal_d3d12.hpp"

#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/indexbuffer.hpp"

namespace tiki
{
	/*static*/ ID3D12Device* GraphicsSystemPlatform::getDevice( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.pDevice;
	}

	/*static*/ IDXGISwapChain1* GraphicsSystemPlatform::getSwapChain( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.pSwapChain;
	}

	/*static*/ ID3D12GraphicsCommandList* GraphicsSystemPlatform::getRenderCommandList( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.pRenderCommandList;
	}

	/*static*/ ID3D12RootSignature* GraphicsSystemPlatform::getRootSignature( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.pRootSignature;
	}

	/*static*/ UploadHeapD3d12& GraphicsSystemPlatform::getUploadHeap( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.uploadHeap;
	}

	/*static*/ DescriptorPoolD3d12& GraphicsSystemPlatform::getShaderResourcePool( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.shaderResourcePool;
	}

	/*static*/ DescriptorPoolD3d12& GraphicsSystemPlatform::getSamplerPool( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.samplerPool;
	}

	/*static*/ DescriptorPoolD3d12& GraphicsSystemPlatform::getRenderTargetPool( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.renderTargetPool;
	}

	/*static*/ DescriptorPoolD3d12& GraphicsSystemPlatform::getDepthStencilPool( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.depthStencilPool;
	}

	/*static*/ DXGI_FORMAT GraphicsSystemPlatform::getD3dPixelFormat( PixelFormat pixelFormat, bool useTypelessFormat )
	{
		TIKI_ASSERT( pixelFormat < PixelFormat_Count || pixelFormat == PixelFormat_Invalid );

		static DXGI_FORMAT s_formatLookup[] =
		{
			DXGI_FORMAT_R8_UNORM,				// PixelFormat_R8,
			DXGI_FORMAT_R8G8B8A8_UNORM,			// PixelFormat_R8G8B8A8
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,	// PixelFormat_R8G8B8A8_Gamma
			DXGI_FORMAT_R16G16B16A16_FLOAT,		// PixelFormat_R16G16B16A16_Float
			DXGI_FORMAT_R32_FLOAT,				// PixelFormat_R32_Float
			DXGI_FORMAT_R32G32B32_FLOAT,		// PixelFormat_R32G32B32_Float
			DXGI_FORMAT_R32G32B32A32_FLOAT,		// PixelFormat_R32G32B32A32_Float
			DXGI_FORMAT_D24_UNORM_S8_UINT		// PixelFormat_Depth24Stencil8
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_formatLookup ) == PixelFormat_Count );

		if( useTypelessFormat && pixelFormat == PixelFormat_Depth24Stencil8 )
		{
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		}
		else if( pixelFormat == PixelFormat_Invalid )
		{
			return DXGI_FORMAT_UNKNOWN;
		}

		return s_formatLookup[ pixelFormat ];
	}

	/*static*/ DXGI_FORMAT GraphicsSystemPlatform::getD3dIndexFormat( IndexType type )
	{
		if( type == IndexType_UInt16 )
		{
			return DXGI_FORMAT_R16_UINT;
		}
		else
		{
			return DXGI_FORMAT_R32_UINT;
		}
	}

	/*static*/ ID3D12GraphicsCommandList* GraphicsSystemPlatform::lockResourceCommandList( GraphicsSystem& graphicsSystem )
	{
		graphicsSystem.m_platformData.resourceCommandListMutex.lock();
		return graphicsSystem.m_platformData.pResourceCommandList;
	}

	/*static*/ void GraphicsSystemPlatform::unlockResourceCommandList( GraphicsSystem& graphicsSystem, ID3D12GraphicsCommandList*& pCommandList )
	{
		TIKI_ASSERT( pCommandList == graphicsSystem.m_platformData.pResourceCommandList );
		pCommandList = nullptr;

		graphicsSystem.m_platformData.resourceCommandListMutex.unlock();
	}

	/*static*/ void GraphicsSystemPlatform::setResourceBarrier( ID3D12GraphicsCommandList* pCommandList, ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter )
	{
		TIKI_DECLARE_STACKANDZERO( D3D12_RESOURCE_BARRIER, descBarrier );
		descBarrier.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		descBarrier.Transition.pResource	= pResource;
		descBarrier.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		descBarrier.Transition.StateBefore	= stateBefore;
		descBarrier.Transition.StateAfter	= stateAfter;

		pCommandList->ResourceBarrier( 1, &descBarrier );
	}

	/*static*/ DynamicBufferData* GraphicsSystemPlatform::createDynamicBuffer( GraphicsSystem& graphicsSystem, uint size )
	{
		GraphicsSystemPlatformData& platformData = graphicsSystem.m_platformData;

		DynamicBufferData* pBuffer = TIKI_MEMORY_NEW_OBJECT( DynamicBufferData );
		if( pBuffer == nullptr )
		{
			return nullptr;
		}

		HRESULT result = platformData.pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer( size ),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS( &pBuffer->pResource )
		);

		if( FAILED( result ) )
		{
			disposeDynamicBuffer( pBuffer );
			return nullptr;
		}
		
		result = pBuffer->pResource->Map( 0u, nullptr, (void**)&pBuffer->pMappedData );
		if( FAILED( result ) )
		{
			disposeDynamicBuffer( pBuffer );
			return nullptr;
		}

		pBuffer->size = size;
		pBuffer->currentSize = 0u;

		return pBuffer;		
	}

	/*static*/ void GraphicsSystemPlatform::disposeDynamicBuffer( DynamicBufferData* pBuffer )
	{
		if( pBuffer->pMappedData != nullptr )
		{
			TIKI_ASSERT( pBuffer->pResource != nullptr );

			pBuffer->pMappedData = nullptr;
			pBuffer->pResource->Unmap( 0u, nullptr );
		}

		if( pBuffer->pResource != nullptr )
		{
			pBuffer->pResource->Release();
			pBuffer->pResource = nullptr;
		}

		TIKI_MEMORY_DELETE_OBJECT( pBuffer );
	}

	/*static*/ DynamicBuffer GraphicsSystemPlatform::allocateDynamicBuffer( GraphicsSystem& graphicsSystem, DynamicBufferTypes type, uint size, uint alignment )
	{
		TIKI_ASSERT( graphicsSystem.m_platformData.isInFrame );

		GraphicsSystemFrame& frame = graphicsSystem.m_platformData.frames[ graphicsSystem.m_platformData.currentSwapBufferIndex ];

		if( type == DynamicBufferTypes_ConstantBuffer )
		{
			alignment = TIKI_MAX( alignment, 256u );
		}

		DynamicBufferData* pBuffer = frame.pLastDynamicBuffer;
		if( pBuffer == nullptr || (alignValue( pBuffer->currentSize, alignment ) + size) > pBuffer->size )
		{
			DynamicBufferData* pNewBuffer = GraphicsSystemPlatform::createDynamicBuffer( graphicsSystem, TIKI_MAX( size, GraphicsSystemDefaults_DynamicBufferSize ) );

			if( pBuffer != nullptr )
			{
				pBuffer->pNext = pNewBuffer;		
				frame.pLastDynamicBuffer = pNewBuffer;
			}
			else
			{
				frame.pFirstDynamicBuffer = pNewBuffer;
				frame.pLastDynamicBuffer = pNewBuffer;
			}

			pBuffer = pNewBuffer;
		}

		pBuffer->currentSize = alignValue( pBuffer->currentSize, alignment );

		DynamicBuffer dynamicBuffer;
		if( pBuffer != nullptr )
		{
			dynamicBuffer.type = type;
			dynamicBuffer.pResource = pBuffer->pResource;
			dynamicBuffer.bufferOffset = pBuffer->currentSize;
			dynamicBuffer.pMappedData = pBuffer->pMappedData + pBuffer->currentSize;
			dynamicBuffer.dataSize = size;

			pBuffer->currentSize += size;
			frame.requiredDynamicBufferSize += size;
		}

		return dynamicBuffer;
	}

	/*static*/ bool GraphicsSystemPlatform::waitForGpu( GraphicsSystem& graphicsSystem )
	{
		GraphicsSystemPlatformData& platformData = graphicsSystem.m_platformData;
		GraphicsSystemFrame& frame = platformData.frames[ platformData.currentSwapBufferIndex ];

		// execute command list
		MutexStackLock stackLock( platformData.resourceCommandListMutex );

		if( platformData.isInFrame && FAILED( platformData.pRenderCommandList->Close() ) )
		{
			return false;
		}
		
		if( FAILED( platformData.pResourceCommandList->Close() ) )
		{
			return false;
		}

		ID3D12CommandList* apCommandLists[] =
		{
			platformData.pResourceCommandList,
			platformData.pRenderCommandList,
		};
		platformData.pCommandQueue->ExecuteCommandLists( platformData.isInFrame ? 2u : 1u, apCommandLists );

		if( FAILED( platformData.pCommandQueue->Signal( platformData.pFence, frame.currentFence ) ) )
		{
			return false;
		}

		if( FAILED( platformData.pFence->SetEventOnCompletion( frame.currentFence, platformData.waitEventHandle ) ) )
		{
			return false;
		}

		WaitForSingleObjectEx( platformData.waitEventHandle, INFINITE, FALSE );
		frame.currentFence++;

		// start resource command list
		if( FAILED( frame.pResourceCommandAllocator->Reset() ) ||
			FAILED( platformData.pResourceCommandList->Reset( frame.pResourceCommandAllocator, nullptr ) ) )
		{
			return false;
		}

		return true;
	}
}
