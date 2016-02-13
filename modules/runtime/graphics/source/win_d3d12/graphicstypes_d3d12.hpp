#ifndef TIKI_GRAPHICSTYPES_D3D12_HPP__INCLUDED__
#define TIKI_GRAPHICSTYPES_D3D12_HPP__INCLUDED__

#include "tiki/base/types.hpp"

enum DXGI_FORMAT;

struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandQueue;
struct ID3D12DescriptorHeap;
struct ID3D12Device;
struct ID3D12PipelineState;
struct ID3D12Resource;
struct ID3D12RootSignature;
struct IDXGISwapChain1;
struct IDXGIFactory4;
struct IDXGISwapChain3;

struct D3D12_INPUT_ELEMENT_DESC;
struct D3D12_CPU_DESCRIPTOR_HANDLE;
struct D3D12_GPU_DESCRIPTOR_HANDLE;

namespace tiki
{
	TIKI_DEFINE_HANLE( DescriptorHandle );

	enum
	{
		GraphicsSystemLimits_MaxUploadHeapSize		= 32u * 1024u * 1024u,
		GraphicsSystemLimits_MaxShaderResourceViews	= 1024u,
		GraphicsSystemLimits_MaxSamplers			= 128u,
		GraphicsSystemLimits_MaxRenderTargetViews	= 128u,
		GraphicsSystemLimits_MaxDepthStencilViews	= 128u,
		GraphicsSystemLimits_MaxFrameCount			= 2u
	};
}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
#	define TIKI_SET_DX_OBJECT_NAME( pObject, pName ) D3D_SET_OBJECT_NAME_A( pObject, pName )
#else
#	define TIKI_SET_DX_OBJECT_NAME( pObject, pName )
#endif

#endif // TIKI_GRAPHICSTYPES_D3D12_HPP__INCLUDED__
