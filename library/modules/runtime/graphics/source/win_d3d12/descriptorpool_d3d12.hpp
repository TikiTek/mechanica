#pragma once
#ifndef TIKI_DESCRIPTORPOOL_D3D12_HPP_INCLUDED
#define TIKI_DESCRIPTORPOOL_D3D12_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"

#include "graphicstypes_d3d12.hpp"

namespace tiki
{
	enum DescriptorType
	{
		DescriptorType_ShaderResource,
		DescriptorType_Sampler,
		DescriptorType_RenderTarget,
		DescriptorType_DepthStencil,

		DescriptorType_Count
	};

	class DescriptorPoolD3d12
	{
		TIKI_NONCOPYABLE_CLASS( DescriptorPoolD3d12 );

	public:

									DescriptorPoolD3d12();
									~DescriptorPoolD3d12();

		bool						create( ID3D12Device* pDevice, uint maxDescriptorCount, DescriptorType type );
		void						dispose();

		DescriptorHandle			allocateDescriptor();
		void						freeDescriptor( DescriptorHandle handle );

		D3D12_CPU_DESCRIPTOR_HANDLE	getCpuHandle( DescriptorHandle handle ) const;
		D3D12_GPU_DESCRIPTOR_HANDLE	getGpuHandle( DescriptorHandle handle ) const;

		ID3D12DescriptorHeap*		getHeap() const;

	private:

		ID3D12DescriptorHeap*		m_pHeap;
		size_t						m_descriptorSize;
		Array< uint64 >				m_freeMask;

	};
}

#endif // TIKI_DESCRIPTORPOOL_D3D12_HPP_INCLUDED
