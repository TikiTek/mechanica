#pragma once
#ifndef TIKI_GRAPHICSSYSTEM_D3D11_HPP
#define TIKI_GRAPHICSSYSTEM_D3D11_HPP

#include "graphicstypes_d3d12.hpp"

namespace tiki
{
	struct GraphicsSystemPlatformData
	{
		GraphicsSystemPlatformData()
		{
			pCommandAllocator			= nullptr;
			pCommandQueue				= nullptr;
			pDescriptionHeap			= nullptr;
			pDevice						= nullptr;
			pPipelineState				= nullptr;
			pRootSignature				= nullptr;
			pSwapChain					= nullptr;

			pBackBufferColor			= nullptr;
			pBackBufferDepth			= nullptr;

			swapBufferCount				= 0u;
			currentSwapBufferIndex		= 0u;
		}

		ID3D12CommandAllocator*		pCommandAllocator;
		ID3D12CommandList*			pCommandList;
		ID3D12CommandQueue*			pCommandQueue;
		ID3D12DescriptorHeap*		pDescriptionHeap;
		ID3D12Device*				pDevice;
		ID3D12PipelineState*		pPipelineState;
		ID3D12RootSignature*		pRootSignature;
		IDXGISwapChain*				pSwapChain;

		ID3D12Resource*				pBackBufferColor;
		ID3D12Resource*				pBackBufferDepth;

		uint						swapBufferCount;
		uint						currentSwapBufferIndex;
	};
}

#endif // TIKI_GRAPHICSSYSTEM_D3D11_HPP
