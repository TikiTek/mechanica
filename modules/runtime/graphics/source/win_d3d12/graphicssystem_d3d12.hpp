#pragma once
#ifndef TIKI_GRAPHICSSYSTEM_D3D11_HPP
#define TIKI_GRAPHICSSYSTEM_D3D11_HPP

#include "graphicstypes_d3d12.hpp"
#include "uploadheap_d3d12.hpp"

namespace tiki
{
	enum
	{
		GraphicsSystemLimits_MaxUploadHeapSize	= 32u * 1024u * 1024u
	};

	struct GraphicsSystemPlatformData
	{
		GraphicsSystemPlatformData()
		{
			pCommandAllocator			= nullptr;
			pCommandQueue				= nullptr;
			pDevice						= nullptr;
			pRootSignature				= nullptr;
			pSwapChain					= nullptr;
			pFence						= nullptr;

			pBackBufferColor					= nullptr;
			pBackBufferColorDescriptionHeap		= nullptr;
			pBackBufferDepth					= nullptr;
			pBackBufferDepthDescriptionHeap		= nullptr;

			swapBufferCount				= 0u;
			currentSwapBufferIndex		= 0u;
			currentFench				= 0u;
		}

		ID3D12CommandAllocator*		pCommandAllocator;
		ID3D12GraphicsCommandList*	pCommandList;
		ID3D12CommandQueue*			pCommandQueue;
		ID3D12Device*				pDevice;
		ID3D12RootSignature*		pRootSignature;
		IDXGISwapChain*				pSwapChain;
		ID3D12Fence*				pFence;

		ID3D12Resource*				pBackBufferColor;
		ID3D12DescriptorHeap*		pBackBufferColorDescriptionHeap;
		ID3D12Resource*				pBackBufferDepth;
		ID3D12DescriptorHeap*		pBackBufferDepthDescriptionHeap;

		UploadHeapD3d12				uploadHeap;

		HANDLE						waitEventHandle;

		uint						swapBufferCount;
		uint						currentSwapBufferIndex;
		uint64						currentFench;
	};
}

#endif // TIKI_GRAPHICSSYSTEM_D3D11_HPP
