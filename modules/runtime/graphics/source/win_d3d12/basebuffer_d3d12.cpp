
#include "tiki/graphics/basebuffer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/string.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	static D3D12_HEAP_TYPE getD3dHeapType( BaseBufferTypes binding, bool dynamic )
	{
		if( dynamic )
		{
			return D3D12_HEAP_TYPE_UPLOAD;
		}

		static const D3D12_HEAP_TYPE s_aD3dHeapType[ ] =
		{
			D3D12_HEAP_TYPE_UPLOAD,
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_HEAP_TYPE_DEFAULT
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aD3dHeapType ) == BaseBufferTypes_Count );

		TIKI_ASSERT( binding < BaseBufferTypes_Count );
		return s_aD3dHeapType[ binding ];
	}

	static D3D12_RESOURCE_STATES getD3dResourceState( BaseBufferTypes binding, bool dynamic )
	{
		if( dynamic )
		{
			return D3D12_RESOURCE_STATE_GENERIC_READ;
		}

		static const D3D12_RESOURCE_STATES s_aD3dResourceUsage[] =
		{
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COMMON
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aD3dResourceUsage ) == BaseBufferTypes_Count );

		TIKI_ASSERT( binding < BaseBufferTypes_Count );
		return s_aD3dResourceUsage[ binding ];
	}

	static DynamicBufferTypes getDynamicBufferType( BaseBufferTypes binding )
	{
		static const DynamicBufferTypes s_aDynamicType[] =
		{
			DynamicBufferTypes_ConstantBuffer,
			DynamicBufferTypes_IndexBuffer,
			DynamicBufferTypes_VertexBuffer
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aDynamicType ) == BaseBufferTypes_Count );

		TIKI_ASSERT( binding < BaseBufferTypes_Count );
		return s_aDynamicType[ binding ];
	}

	BaseBuffer::BaseBuffer()
	{
		m_pBuffer = nullptr;
		m_isDynamic = false;

		m_dynamicBufferType = DynamicBufferTypes_Invalid;
		m_dynamicBufferSize = 0u;
	}

	BaseBuffer::~BaseBuffer()
	{
		TIKI_ASSERT( m_pBuffer == nullptr );
	}

	bool BaseBuffer::create( GraphicsSystem& graphicsSystem, uint size, bool dynamic, BaseBufferTypes binding, const void* pInitData, const char* pDebugName )
	{
		TIKI_ASSERT( m_pBuffer == nullptr );

		m_isDynamic = dynamic;

		if( m_isDynamic )
		{
			TIKI_ASSERT( pInitData == nullptr );

			m_dynamicBufferType = getDynamicBufferType( binding );
			m_dynamicBufferSize = size;
		}
		else
		{
			ID3D12Device* pDevice = GraphicsSystemPlatform::getDevice( graphicsSystem );

			const D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES( getD3dHeapType( binding, dynamic ) );
			const D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer( size );
			const D3D12_RESOURCE_STATES resourceState = getD3dResourceState( binding, dynamic );

			HRESULT result = pDevice->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				resourceState,
				nullptr,
				IID_PPV_ARGS( &m_pBuffer )
				);

			if( FAILED( result ) )
			{
				dispose( graphicsSystem );
				return false;
			}

			TIKI_SET_DX_OBJECT_NAME( m_pBuffer, pDebugName );

			if( pInitData != nullptr )
			{
				if( heapProperties.Type != D3D12_HEAP_TYPE_UPLOAD )
				{
					ID3D12Resource* pUploadBuffer = nullptr;
					HRESULT result = pDevice->CreateCommittedResource(
						&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ),
						D3D12_HEAP_FLAG_NONE,
						&CD3DX12_RESOURCE_DESC::Buffer( size ),
						D3D12_RESOURCE_STATE_GENERIC_READ,
						nullptr,
						IID_PPV_ARGS( &pUploadBuffer )
						);

					if( FAILED( result ) )
					{
						dispose( graphicsSystem );
						return false;
					}

					TIKI_DECLARE_STACKANDZERO( D3D12_SUBRESOURCE_DATA, initData );
					initData.pData = pInitData;
					initData.RowPitch = size;
					initData.SlicePitch = size;

					ID3D12GraphicsCommandList* pCommandList = GraphicsSystemPlatform::lockResourceCommandList( graphicsSystem );
					GraphicsSystemPlatform::setResourceBarrier( pCommandList, m_pBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST );
					UpdateSubresources<1>( pCommandList, m_pBuffer, pUploadBuffer, 0, 0, 1, &initData );
					GraphicsSystemPlatform::setResourceBarrier( pCommandList, m_pBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ );
					GraphicsSystemPlatform::unlockResourceCommandList( graphicsSystem, pCommandList );

					pUploadBuffer->Release();
					pUploadBuffer = nullptr;
				}
				else
				{
					void* pTargetData = nullptr;
					m_pBuffer->Map( 0u, nullptr, &pTargetData );
					memory::copy( pTargetData, pInitData, size );
					m_pBuffer->Unmap( 0u, nullptr );
				}
			}
		}

		return true;
	}

	void BaseBuffer::dispose( GraphicsSystem& graphicsSystem )
	{
		if( !m_isDynamic )
		{
			GraphicsSystemPlatform::waitForGpu( graphicsSystem );

			GraphicsSystemPlatform::safeRelease( &m_pBuffer );
		}
		else
		{
			m_dynamicBuffer = DynamicBuffer();
			m_dynamicBufferType = DynamicBufferTypes_Invalid;
			m_dynamicBufferSize = 0u;
		}
	}
}