
#include "tiki/graphics/basebuffer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/string.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	static D3D12_HEAP_TYPE getD3dHeapType( GraphicsBufferType binding, bool dynamic )
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
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aD3dHeapType ) == GraphicsBufferType_Count );

		TIKI_ASSERT( binding < GraphicsBufferType_Count );
		return s_aD3dHeapType[ binding ];
	}

	static D3D12_RESOURCE_STATES getD3dResourceState( GraphicsBufferType binding, bool dynamic )
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
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aD3dResourceUsage ) == GraphicsBufferType_Count );

		TIKI_ASSERT( binding < GraphicsBufferType_Count );
		return s_aD3dResourceUsage[ binding ];
	}

	BaseBuffer::BaseBuffer()
	{
		for( uint i = 0u; i < m_buffers.getCount(); ++i )
		{
			m_buffers[ i ] = nullptr;
		}
		m_dynamic = false;
	}

	BaseBuffer::~BaseBuffer()
	{
#if TIKI_ENABLED( TIKI_USE_ASSERT )
		for( uint i = 0u; i < m_buffers.getCount(); ++i )
		{
			TIKI_ASSERT( m_buffers[ i ] == nullptr );
		}
#endif
	}

	bool BaseBuffer::create( GraphicsSystem& graphicsSystem, uint size, bool dynamic, GraphicsBufferType binding, const void* pInitData, const char* pDebugName )
	{
		TIKI_ASSERT( m_buffers[ 0u ] == nullptr );

		m_dynamic = dynamic;

		if( binding == GraphicsBufferType_ConstantBuffer )
		{
			size = alignValue< uint >( size, 256u );
		}

		ID3D12Device* pDevice = GraphicsSystemPlatform::getDevice( graphicsSystem );

		const D3D12_HEAP_PROPERTIES heapProperties	= CD3DX12_HEAP_PROPERTIES( getD3dHeapType( binding, dynamic ) );
		const D3D12_RESOURCE_DESC resourceDesc		= CD3DX12_RESOURCE_DESC::Buffer( size );
		const D3D12_RESOURCE_STATES resourceState	= getD3dResourceState( binding, dynamic );

		const uint bufferCount = m_dynamic ? m_buffers.getCount() : 1u;
		for( uint i = 0u; i < bufferCount; ++i )
		{
			HRESULT result = pDevice->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				resourceState,
				nullptr,
				IID_PPV_ARGS( &m_buffers[ i ] )
			);

			if( FAILED( result ) )
			{
				dispose( graphicsSystem );
				return false;
			}

			TIKI_SET_DX_OBJECT_NAME( m_buffers[ i ], pDebugName );
		}

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
				initData.pData		= pInitData;
				initData.RowPitch	= size;
				initData.SlicePitch	= size;

				ID3D12GraphicsCommandList* pCommandList = GraphicsSystemPlatform::lockResourceCommandList( graphicsSystem );
				for( uint i = 0u; i < bufferCount; ++i )
				{
					GraphicsSystemPlatform::setResourceBarrier( pCommandList, m_buffers[ i ], D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST );
					UpdateSubresources<1>( pCommandList, m_buffers[ i ], pUploadBuffer, 0, 0, 1, &initData );
					GraphicsSystemPlatform::setResourceBarrier( pCommandList, m_buffers[ i ], D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ );
				}
				GraphicsSystemPlatform::unlockResourceCommandList( graphicsSystem, pCommandList );

				pUploadBuffer->Release();
				pUploadBuffer = nullptr;
			}
			else
			{
				for( uint i = 0u; i < bufferCount; ++i )
				{
					void* pTargetData = nullptr;
					m_buffers[ i ]->Map( 0u, nullptr, &pTargetData );
					memory::copy( pTargetData, pInitData, size );
					m_buffers[ i ]->Unmap( 0u, nullptr );
				}
			}
		}

		//if ( binding == GraphicsBufferType_ConstantBuffer )
		//{
		//	DescriptorPoolD3d12& shaderResourcePool = GraphicsSystemPlatform::getShaderResourcePool( graphicsSystem );
		//	m_descriptorHandle = shaderResourcePool.allocateDescriptor();
		//	if( m_descriptorHandle == InvalidDescriptorHandle )
		//	{
		//		dispose( graphicsSystem );
		//		return false;
		//	}

		//	TIKI_DECLARE_STACKANDZERO( D3D12_CONSTANT_BUFFER_VIEW_DESC, viewDesc );
		//	viewDesc.SizeInBytes = (UINT)size;

		//	pDevice->CreateConstantBufferView( &viewDesc, shaderResourcePool.getCpuHandle( m_descriptorHandle ) );
		//}
		//else
		//{
		//	m_descriptorHandle = InvalidDescriptorHandle;
		//}

		return true;
	}

	void BaseBuffer::dispose( GraphicsSystem& graphicsSystem )
	{
		GraphicsSystemPlatform::waitForGpu( graphicsSystem );

		for( uint i = 0u; i < m_buffers.getCount(); ++i )
		{
			GraphicsSystemPlatform::safeRelease( &m_buffers[ i ] );
		}

		//if( m_descriptorHandle != InvalidDescriptorHandle )
		//{
		//	GraphicsSystemPlatform::getShaderResourcePool( graphicsSystem ).freeDescriptor( m_descriptorHandle );
		//	m_descriptorHandle = InvalidDescriptorHandle;
		//}
	}
}