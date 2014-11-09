
#include "tiki/graphics/basebuffer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/graphics/indexbuffer.hpp"
#include "tiki/graphics/vertexbuffer.hpp"

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

	static D3D12_RESOURCE_USAGE getD3dResourceUsage( GraphicsBufferType binding, bool dynamic )
	{
		if( dynamic )
		{
			return D3D12_RESOURCE_USAGE_GENERIC_READ;
		}

		static const D3D12_RESOURCE_USAGE s_aD3dResourceUsage[] =
		{
			D3D12_RESOURCE_USAGE_GENERIC_READ,
			D3D12_RESOURCE_USAGE_INITIAL,
			D3D12_RESOURCE_USAGE_INITIAL
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aD3dResourceUsage ) == GraphicsBufferType_Count );

		TIKI_ASSERT( binding < GraphicsBufferType_Count );
		return s_aD3dResourceUsage[ binding ];
	}

	static D3D12_DESCRIPTOR_HEAP_TYPE getD3dDescriptorHeapType( GraphicsBufferType binding  )
	{
		static const D3D12_DESCRIPTOR_HEAP_TYPE s_aD3dDescriptorHeapType[ ] =
		{
			D3D12_CBV_SRV_UAV_DESCRIPTOR_HEAP,
			D3D12_IBV_DESCRIPTOR_HEAP,
			D3D12_VBV_DESCRIPTOR_HEAP
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aD3dDescriptorHeapType ) == GraphicsBufferType_Count );

		TIKI_ASSERT( binding < GraphicsBufferType_Count );
		return s_aD3dDescriptorHeapType[ binding ];
	}

	static DXGI_FORMAT getD3dIndexFormat( IndexType type )
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

	BaseBuffer::BaseBuffer()
	{
		m_pBuffer			= nullptr;
		m_pDescriptorHeap	= nullptr;
	}

	BaseBuffer::~BaseBuffer()
	{
		TIKI_ASSERT( m_pBuffer == nullptr );
		TIKI_ASSERT( m_pDescriptorHeap == nullptr );
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
			dispose( graphicsSystem );
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

				if( FAILED( result ) )
				{
					dispose( graphicsSystem );
					return false;
				}

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

		// create descriptor heap
		{
			TIKI_DECLARE_STACKANDZERO( D3D12_DESCRIPTOR_HEAP_DESC, heapDesc );
			heapDesc.Type	= getD3dDescriptorHeapType( binding );

			if( binding == GraphicsBufferType_ConstantBuffer )
			{
				heapDesc.Flags	= D3D12_DESCRIPTOR_HEAP_SHADER_VISIBLE;
			}

			if( FAILED( pDevice->CreateDescriptorHeap( &heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pDescriptorHeap ) ) )
			{
				dispose( graphicsSystem );
				return false;
			}
		}

		switch( binding )
		{
		case GraphicsBufferType_ConstantBuffer:
			{
				TIKI_DECLARE_STACKANDZERO( D3D12_CONSTANT_BUFFER_VIEW_DESC, viewDesc );
				viewDesc.SizeInBytes = size;

				pDevice->CreateConstantBufferView( m_pBuffer, &viewDesc, m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart() );
			}
			break;

		case GraphicsBufferType_IndexBuffer:
			{
				IndexBuffer* pBuffer = static_cast<IndexBuffer*>(this);

				TIKI_DECLARE_STACKANDZERO( D3D12_INDEX_BUFFER_VIEW_DESC, viewDesc );
				viewDesc.Format			= getD3dIndexFormat( pBuffer->getIndexType() );
				viewDesc.SizeInBytes	= size;

				pDevice->CreateIndexBufferView( m_pBuffer, &viewDesc, m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart() );
			}
			break;

		case GraphicsBufferType_VertexBuffer:
			{
				VertexBuffer* pBuffer = static_cast<VertexBuffer*>(this);

				TIKI_DECLARE_STACKANDZERO( D3D12_VERTEX_BUFFER_VIEW_DESC, viewDesc );
				viewDesc.SizeInBytes	= size;
				viewDesc.StrideInBytes	= pBuffer->getStride();

				pDevice->CreateVertexBufferView( m_pBuffer, &viewDesc, m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart() );
			}
			break;
		}		

		return true;
	}

	void BaseBuffer::dispose( GraphicsSystem& graphicsSystem )
	{
		graphics::safeRelease( &m_pDescriptorHeap );
		graphics::safeRelease( &m_pBuffer );
	}
}