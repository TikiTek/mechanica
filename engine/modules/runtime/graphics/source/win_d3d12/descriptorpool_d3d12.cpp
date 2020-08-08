#include "descriptorpool_d3d12.hpp"

#include "tiki/base/functions.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	DescriptorPoolD3d12::DescriptorPoolD3d12()
	{
		m_pHeap = nullptr;
	}

	DescriptorPoolD3d12::~DescriptorPoolD3d12()
	{
		TIKI_ASSERT( m_pHeap == nullptr );
	}

	bool DescriptorPoolD3d12::create( ID3D12Device* pDevice, uintreg maxDescriptorCount, DescriptorType type, const char* pDebugName )
	{
		static const D3D12_DESCRIPTOR_HEAP_TYPE s_aDescriptorTypeMapping[] =
		{
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,		// DescriptorType_ShaderResource
			D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,			// DescriptorType_Sampler
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV,				// DescriptorType_RenderTarget
			D3D12_DESCRIPTOR_HEAP_TYPE_DSV				// DescriptorType_DepthStencil
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aDescriptorTypeMapping ) == DescriptorType_Count );

		const D3D12_DESCRIPTOR_HEAP_TYPE descriptorType = s_aDescriptorTypeMapping[ type ];
		m_descriptorSize		= pDevice->GetDescriptorHandleIncrementSize( descriptorType );

		TIKI_DECLARE_STACKANDZERO( D3D12_DESCRIPTOR_HEAP_DESC, heapDesc );
		heapDesc.Type			= descriptorType;
		heapDesc.NumDescriptors	= (UINT)maxDescriptorCount;
		heapDesc.Flags			= (type == DescriptorType_ShaderResource || type == DescriptorType_Sampler ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		const HRESULT result = pDevice->CreateDescriptorHeap( &heapDesc, IID_PPV_ARGS( &m_pHeap ) );
		if( FAILED( result ) )
		{
			TIKI_TRACE_ERROR( "[graphics] Unable to create Descriptor Heap.\n" );
			dispose();
			return false;
		}
		TIKI_SET_DX_OBJECT_NAME( m_pHeap, pDebugName );

		const uintreg maskCount = alignValue< uintreg >( maxDescriptorCount, 64u ) / 64u;
		if( !m_freeMask.create( maskCount ) )
		{
			dispose();
			return nullptr;
		}

		for( uintreg i = 0u; i < m_freeMask.getCount(); ++i )
		{
			m_freeMask[ i ] = (uint64)-1;
		}

		return true;
	}

	void DescriptorPoolD3d12::dispose()
	{
		m_freeMask.dispose();
		GraphicsSystemPlatform::safeRelease( &m_pHeap );
	}

	DescriptorHandle DescriptorPoolD3d12::allocateDescriptor()
	{
		uintreg maskIndex = 0;
		while( maskIndex < m_freeMask.getCount() && m_freeMask[ maskIndex ] == 0u )
		{
			maskIndex++;
		}

		if ( maskIndex < m_freeMask.getCount() )
		{
			const uint64 mask			= m_freeMask[ maskIndex ];
			const uint64 firstBit		= mask & (-(sint64)mask);
			const uint64 newMask		= mask & (~firstBit);
			const uint32 indexInMask	= uint32( 63u - countLeadingZeros64( firstBit ) );
			m_freeMask[ maskIndex ]		= newMask;

			return (DescriptorHandle)( (maskIndex * 64u) + indexInMask );
		}

		return InvalidDescriptorHandle;
	}

	bool DescriptorPoolD3d12::allocateRange( DescriptorHandle* pOutput, uintreg count )
	{
		for( uintreg maskIndex = 0; maskIndex < m_freeMask.getCount(); ++maskIndex )
		{
			if( m_freeMask[ maskIndex ] == 0u )
			{
				continue;
			}

			const uint64 mask			= m_freeMask[ maskIndex ];
			const uint64 firstBit		= mask & (-(sint64)mask);
			const uint64 checkBits		= (1ull << count) - 1;
			const uint64 checkMask		= checkBits << firstBit;

			if( (checkMask & mask) == checkMask )
			{
				continue;
			}

			const uint32 indexInMask	= uint32( 63u - countLeadingZeros64( firstBit ) );
			const uint32 firstIndex		= uint32( (maskIndex * 64u) + indexInMask );
			for( uintreg i = 0u; i < count; ++i )
			{
				pOutput[ i ] = (DescriptorHandle)(firstIndex + i);
			}

			const uint64 newMask		= mask & (~checkMask);
			m_freeMask[ maskIndex ]		= newMask;

			return true;
		}

		return false;
	}

	void DescriptorPoolD3d12::freeDescriptor( DescriptorHandle handle )
	{
		TIKI_ASSERT( handle != InvalidDescriptorHandle );

		const uintreg handleValue	= (uintreg)handle;
		const uintreg maskIndex		= handleValue / 64u;
		const uint64 indexInMask	= handleValue % 64u;
		const uint64 bitToSet		= 1ull << indexInMask;

		m_freeMask[ maskIndex ] |= bitToSet;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorPoolD3d12::getCpuHandle( DescriptorHandle handle ) const
	{
		TIKI_ASSERT( handle != InvalidDescriptorHandle );

		D3D12_CPU_DESCRIPTOR_HANDLE d3dHandle = m_pHeap->GetCPUDescriptorHandleForHeapStart();
		d3dHandle.ptr += uintreg( handle ) * m_descriptorSize;

		return d3dHandle;
	}

	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorPoolD3d12::getGpuHandle( DescriptorHandle handle ) const
	{
		TIKI_ASSERT( handle != InvalidDescriptorHandle );

		D3D12_GPU_DESCRIPTOR_HANDLE d3dHandle = m_pHeap->GetGPUDescriptorHandleForHeapStart();
		d3dHandle.ptr += uintreg( handle ) * m_descriptorSize;

		return d3dHandle;
	}

	ID3D12DescriptorHeap* DescriptorPoolD3d12::getHeap() const
	{
		return m_pHeap;
	}
}