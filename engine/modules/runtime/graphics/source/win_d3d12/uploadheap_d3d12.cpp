#include "uploadheap_d3d12.hpp"

#include "tiki/base/functions.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	UploadHeapD3d12::UploadHeapD3d12()
	{
		m_pBuffer		= nullptr;
		m_pDataStart	= nullptr;
		m_pDataEnd		= nullptr;
	}

	UploadHeapD3d12::~UploadHeapD3d12()
	{
		TIKI_ASSERT( m_pBuffer == nullptr );
		TIKI_ASSERT( m_pDataStart == nullptr );
		TIKI_ASSERT( m_pDataEnd == nullptr );
	}

	bool UploadHeapD3d12::create( ID3D12Device* pDevice, uint size, uint maxAllocationCount, const char* pDebugName )
	{
		TIKI_ASSERT( pDevice != nullptr );

		HRESULT result = pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer( size ),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, 
			IID_PPV_ARGS( &m_pBuffer )
		);

		if( FAILED( result ) )
		{
			return false;
		}
		TIKI_SET_DX_OBJECT_NAME( m_pBuffer, pDebugName );

		if( FAILED( m_pBuffer->Map( 0u, nullptr, (void**)&m_pDataStart ) ) )
		{
			return false;
		}

		m_size			= size;
		m_pDataEnd		= m_pDataStart + size;

		m_allocations.create( maxAllocationCount );

		return true;
	}

	void UploadHeapD3d12::dispose()
	{
		m_allocations.dispose();

		m_pDataStart	= nullptr;
		m_pDataEnd		= nullptr;
		
		if( m_pBuffer != nullptr )
		{
			m_pBuffer->Unmap( 0u, nullptr );
		}
		GraphicsSystemPlatform::safeRelease( &m_pBuffer );
	}

	void UploadHeapD3d12::finalizeFrame( uint currentFrameIndex )
	{
		if( currentFrameIndex > 0u )
		{
			const uint lastCompleteFrameIndex = currentFrameIndex - 1u;

			AllocationData allocation;
			while( !m_allocations.isEmpty() && m_allocations.getTop().frameIndex <= lastCompleteFrameIndex )
			{
				m_allocations.pop( allocation );
			}
		}

		m_currentFrameIndex = currentFrameIndex;
	}

	bool UploadHeapD3d12::allocateData( UploadHeapAllocationResult& result, uint size, uint alignment )
	{
		const uint requiredSpace = size + alignment;

		uint8* pData = nullptr;
		if( !m_allocations.isEmpty() )
		{
			const AllocationData& topAllocation = m_allocations.getBottom();
			uint8* pTopData						= topAllocation.pData;

			const AllocationData& bottomAllocation	= m_allocations.getBottom();
			uint8* pBottomData						= (bottomAllocation.pData + bottomAllocation.size);

			const bool tlb = (pTopData < pBottomData);
			const uint endSpace		= tlb ? m_pDataEnd - pBottomData : pBottomData - pTopData;
			const uint startSpace	= tlb ? pTopData - m_pDataStart : 0u;

			bool useEnd = true;
			if( endSpace < requiredSpace && startSpace >= requiredSpace )
			{
				useEnd = false;
			}
			else if (endSpace < requiredSpace && startSpace < requiredSpace)
			{
				return false;
			}

			pData = alignPointer( useEnd ? pBottomData : m_pDataStart, alignment );
		}
		else if( requiredSpace > m_size )
		{
			return false;
		}
		else
		{
			pData = m_pDataStart;
		}

		AllocationData& allocation = m_allocations.push();
		allocation.frameIndex	= m_currentFrameIndex;
		allocation.size			= size;
		allocation.pData		= alignPointer( pData, alignment );

		result.pData		= allocation.pData;
		result.size			= size;
		result.offset		= allocation.pData - m_pDataStart;

		return true;
	}
}