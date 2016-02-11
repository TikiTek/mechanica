#pragma once
#ifndef TIKI_GRAPHICSSYSTEM_D3D11_HPP
#define TIKI_GRAPHICSSYSTEM_D3D11_HPP

#include "uploadheap_d3d12.hpp"
#include "descriptorpool_d3d12.hpp"

namespace tiki
{
	enum
	{
		GraphicsSystemLimits_MaxUploadHeapSize		= 32u * 1024u * 1024u,
		GraphicsSystemLimits_MaxShaderResourceViews	= 1024u,
		GraphicsSystemLimits_MaxSamplers			= 128u,
		GraphicsSystemLimits_MaxRenderTargetViews	= 128u,
		GraphicsSystemLimits_MaxDepthStencilViews	= 128u,
		GraphicsSystemLimits_MaxFrameCount			= 2u
	};

	struct GraphicsSystemFrame
	{
		GraphicsSystemFrame()
		{
			pCommandAllocator		= nullptr;
			pBackBufferColorResouce	= nullptr;

			backBufferColorHandle	= InvalidDescriptorHandle;

			currentFench			= 0u;
		}

		ID3D12CommandAllocator*		pCommandAllocator;

		ID3D12Resource*				pBackBufferColorResouce;
		DescriptorHandle			backBufferColorHandle;

		uint64						currentFench;
	};

	struct GraphicsSystemPlatformData
	{
		GraphicsSystemPlatformData()
		{
			pSwapChain					= nullptr;
			pFactory					= nullptr;

			pDevice						= nullptr;
			pCommandQueue				= nullptr;
			pCommandList				= nullptr;
			pRootSignature				= nullptr;
			pFence						= nullptr;

			waitEventHandle				= INVALID_HANDLE_VALUE;

			currentSwapBufferIndex		= 0u;
		}

		IDXGIFactory4*				pFactory;
		IDXGISwapChain3*			pSwapChain;

		ID3D12Device*				pDevice;
		ID3D12CommandQueue*			pCommandQueue;
		ID3D12GraphicsCommandList*	pCommandList;
		ID3D12RootSignature*		pRootSignature;
		ID3D12Fence*				pFence;

		UploadHeapD3d12				uploadHeap;

		DescriptorPoolD3d12			shaderResourcePool;
		DescriptorPoolD3d12			samplerPool;
		DescriptorPoolD3d12			renderTargetPool;
		DescriptorPoolD3d12			depthStencilPool;
				
		HANDLE						waitEventHandle;

		GraphicsSystemFrame			frames[ GraphicsSystemLimits_MaxFrameCount ];
		uint						currentSwapBufferIndex;
		
	};
}

#endif // TIKI_GRAPHICSSYSTEM_D3D11_HPP
