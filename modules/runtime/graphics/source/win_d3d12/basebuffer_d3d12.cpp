
#include "tiki/graphics/basebuffer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	D3D12_HEAP_TYPE getD3dHeapType( GraphicsBufferType binding, bool dynamic )
	{
		if( dynamic )
		{
			return D3D12_HEAP_TYPE_UPLOAD;
		}

		switch ( binding )
		{
		case GraphicsBufferType_ConstantBuffer:
			return D3D12_HEAP_TYPE_UPLOAD;

		case GraphicsBufferType_IndexBuffer:
		case GraphicsBufferType_VertexBuffer:
			return D3D12_HEAP_TYPE_DEFAULT;
			return D3D12_HEAP_TYPE_DEFAULT;

		default:
			break;
		}

		TIKI_BREAK( "[graphics] wrong GraphicsBufferType.\n" );
		return (D3D12_HEAP_TYPE)0;
	}

	D3D12_RESOURCE_USAGE getD3dResourceUsage( GraphicsBufferType binding, bool dynamic )
	{
		if( dynamic )
		{
			return D3D12_RESOURCE_USAGE_GENERIC_READ;
		}

		switch( binding )
		{
		case GraphicsBufferType_ConstantBuffer:
			return D3D12_RESOURCE_USAGE_GENERIC_READ;

		case GraphicsBufferType_IndexBuffer:
		case GraphicsBufferType_VertexBuffer:
			return D3D12_RESOURCE_USAGE_INITIAL;
			return D3D12_RESOURCE_USAGE_INITIAL;

		default:
			break;
		}

		TIKI_BREAK( "[graphics] wrong GraphicsBufferType.\n" );
		return (D3D12_RESOURCE_USAGE)0;
	}

	BaseBuffer::BaseBuffer()
	{
		m_pBuffer = nullptr;
	}

	BaseBuffer::~BaseBuffer()
	{
		TIKI_ASSERT( m_pBuffer == nullptr );
	}

	bool BaseBuffer::create( GraphicsSystem& graphicsSystem, size_t size, bool dynamic, GraphicsBufferType binding, const void* pInitData /*= nullptr*/ )
	{
		TIKI_ASSERT( m_pBuffer == nullptr );

		m_dynamic = dynamic;

		ID3D12Device* pDevice = graphics::getDevice( graphicsSystem );

		const D3D12_HEAP_TYPE heapType				= getD3dHeapType( binding, dynamic );
		const D3D12_RESOURCE_USAGE resourceUsage	= getD3dResourceUsage( binding, dynamic );

		HRESULT result = pDevice->CreateCommittedResource(
			&CD3D12_HEAP_PROPERTIES( heapType ),
			D3D12_HEAP_MISC_NONE,
			&CD3D12_RESOURCE_DESC::Buffer( size ),
			resourceUsage,
			IID_PPV_ARGS( &m_pBuffer )
		);

		if( FAILED( result ) )
		{
			return false;
		}

		if( pInitData != nullptr )
		{
			if( heapType != D3D12_HEAP_TYPE_UPLOAD )
			{
				ID3D12Resource* pUploadBuffer = nullptr;
				result = pDevice->CreateCommittedResource(
					&CD3D12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ),
					D3D12_HEAP_MISC_NONE,
					&CD3D12_RESOURCE_DESC::Buffer( size ),
					D3D12_RESOURCE_USAGE_GENERIC_READ,
					IID_PPV_ARGS( &pUploadBuffer )
				);

				TIKI_DECLARE_STACKANDZERO( D3D12_SUBRESOURCE_DATA, initData );
				initData.pData		= pInitData;
				initData.RowPitch	= size;
				initData.SlicePitch	= size;

				ID3D12CommandList* pCommandList = graphics::getCommandList( graphicsSystem );
				graphics::setResourceBarrier( pCommandList, m_pBuffer, D3D12_RESOURCE_USAGE_INITIAL, D3D12_RESOURCE_USAGE_COPY_DEST );
				UpdateSubresources<1>( pCommandList, m_pBuffer, pUploadBuffer, 0, 0, 1, &initData );
				graphics::setResourceBarrier( pCommandList, m_pBuffer, D3D12_RESOURCE_USAGE_COPY_DEST, D3D12_RESOURCE_USAGE_GENERIC_READ );

				pUploadBuffer->Release();
				pUploadBuffer = nullptr;
			}
			else
			{
				void* pTargetData = nullptr;
				m_pBuffer->Map( nullptr, &pTargetData );
				memory::copy( pTargetData, pInitData, size );
				m_pBuffer->Unmap( nullptr );
			}
		}

		return true;
	}

	void BaseBuffer::dispose( GraphicsSystem& graphicsSystem )
	{
		if ( m_pBuffer != nullptr )
		{
			m_pBuffer->Release();
			m_pBuffer = nullptr;
		}
	}
}