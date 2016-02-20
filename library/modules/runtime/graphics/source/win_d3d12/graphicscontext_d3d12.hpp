#ifndef TIKI_GRAPHICSCONTEXT_D3D12_HPP__INCLUDED__
#define TIKI_GRAPHICSCONTEXT_D3D12_HPP__INCLUDED__

#include "tiki/container/fixedsizedarray.hpp"

#include "graphicstypes_d3d12.hpp"

namespace tiki
{
	class DescriptorPoolD3d12;

	enum
	{
		GraphicsSystemLimits_MaxPipelineStates	= 128u,
		GraphicsSystemLimits_MaxDescriptorHeaps	= (GraphicsSystemLimits_VertexShaderTextureSlots * 2u) + (GraphicsSystemLimits_PixelShaderTextureSlots * 2u) + GraphicsSystemLimits_VertexShaderConstantSlots + GraphicsSystemLimits_PixelShaderConstantSlots
	};

	struct GraphicsContextPipelineState
	{
		crc32					crc;
		ID3D12PipelineState*	pPpielineState;
	};

	struct GraphicsContextPlatformData
	{
		GraphicsContextPlatformData()
		{
			pDevice					= nullptr;
			pCommandList			= nullptr;
			pRootSignature			= nullptr;
			pShaderResourcePool		= nullptr;
			pSamplerPool			= nullptr;

			pImmediateBufferData	= nullptr;
			immediateBufferSize		= 0u;
		}

		typedef FixedSizedArray< GraphicsContextPipelineState, GraphicsSystemLimits_MaxPipelineStates > PipelineStateArray;

		ID3D12Device*				pDevice;
		ID3D12GraphicsCommandList*	pCommandList;
		ID3D12RootSignature*		pRootSignature;

		DescriptorPoolD3d12*		pShaderResourcePool;
		DescriptorPoolD3d12*		pSamplerPool;

		PipelineStateArray			pipelineStates;

		uint8*						pImmediateBufferData;
		uint						immediateBufferSize;
		
	};
}

#endif // TIKI_GRAPHICSCONTEXT_D3D12_HPP__INCLUDED__
