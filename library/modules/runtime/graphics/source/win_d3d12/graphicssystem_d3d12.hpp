#pragma once
#ifndef TIKI_GRAPHICSSYSTEM_D3D11_HPP
#define TIKI_GRAPHICSSYSTEM_D3D11_HPP

#include "tiki/threading/mutex.hpp"

#include "uploadheap_d3d12.hpp"
#include "descriptorpool_d3d12.hpp"

namespace tiki
{
	struct GraphicsSystemFrame
	{
		GraphicsSystemFrame()
		{
			pRenderCommandAllocator		= nullptr;
			pResourceCommandAllocator	= nullptr;
			pBackBufferColorResouce		= nullptr;

			backBufferColorHandle		= InvalidDescriptorHandle;

			currentFence				= 0u;
		}

		ID3D12CommandAllocator*		pRenderCommandAllocator;
		ID3D12CommandAllocator*		pResourceCommandAllocator;

		ID3D12Resource*				pBackBufferColorResouce;
		DescriptorHandle			backBufferColorHandle;

		uint64						currentFence;
	};

	struct GraphicsSystemPlatformData
	{
		GraphicsSystemPlatformData()
		{
			pSwapChain					= nullptr;
			pFactory					= nullptr;

			pDevice						= nullptr;
			pCommandQueue				= nullptr;
			pRenderCommandList			= nullptr;
			pResourceCommandList		= nullptr;
			pRootSignature				= nullptr;
			pFence						= nullptr;

			waitEventHandle				= INVALID_HANDLE_VALUE;

			isInFrame					= false;
			currentSwapBufferIndex		= 0u;
		}

		IDXGIFactory4*				pFactory;
		IDXGISwapChain3*			pSwapChain;

		ID3D12Device*				pDevice;
		ID3D12CommandQueue*			pCommandQueue;
		ID3D12GraphicsCommandList*	pRenderCommandList;
		ID3D12RootSignature*		pRootSignature;
		ID3D12Fence*				pFence;

		ID3D12GraphicsCommandList*	pResourceCommandList;
		Mutex						resourceCommandListMutex;

		UploadHeapD3d12				uploadHeap;

		DescriptorPoolD3d12			shaderResourcePool;
		DescriptorPoolD3d12			samplerPool;
		DescriptorPoolD3d12			renderTargetPool;
		DescriptorPoolD3d12			depthStencilPool;
				
		HANDLE						waitEventHandle;

		bool						isInFrame;
		GraphicsSystemFrame			frames[ GraphicsSystemLimits_MaxFrameCount ];
		uint						currentSwapBufferIndex;
		
	};
}

#endif // TIKI_GRAPHICSSYSTEM_D3D11_HPP
