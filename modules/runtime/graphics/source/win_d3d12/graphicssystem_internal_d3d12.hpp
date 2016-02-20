#ifndef TIKI_GRAPHICSSYSTEM_INTERNAL_D3D12_HPP__INCLUDED__
#define TIKI_GRAPHICSSYSTEM_INTERNAL_D3D12_HPP__INCLUDED__

#include "graphicstypes_d3d12.hpp"

#include "tiki/graphics/graphicssystemlimits.hpp"
#include "tiki/graphics/pixelformat.hpp"

#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3dx12.h>

namespace tiki
{
	class DescriptorPoolD3d12;
	class GraphicsSystem;
	class UploadHeapD3d12;
	enum IndexType : uint8;
	struct GraphicsSystemPlatformData;

	enum GraphicsDiscriptorRange
	{
		GraphicsDiscriptorRange_VertexConstants,
		GraphicsDiscriptorRange_VertexTextures,
		GraphicsDiscriptorRange_VertexSamplers,
		GraphicsDiscriptorRange_PixelConstants,
		GraphicsDiscriptorRange_PixelTextures,
		GraphicsDiscriptorRange_PixelSamplers,

		GraphicsDiscriptorRange_Count
	};

	enum GraphicsDiscriptorIndex
	{
		GraphicsDiscriptorIndex_FirstVertexConstant		= 0,
		GraphicsDiscriptorIndex_FirstVertexTexture		= GraphicsDiscriptorIndex_FirstVertexConstant + GraphicsSystemLimits_VertexShaderConstantSlots,
		GraphicsDiscriptorIndex_FirstVertexSampler		= GraphicsDiscriptorIndex_FirstVertexTexture + GraphicsSystemLimits_VertexShaderTextureSlots,
		GraphicsDiscriptorIndex_FirstPixelConstant		= GraphicsDiscriptorIndex_FirstVertexSampler + GraphicsSystemLimits_VertexShaderTextureSlots,
		GraphicsDiscriptorIndex_FirstPixelTexture		= GraphicsDiscriptorIndex_FirstPixelConstant + GraphicsSystemLimits_PixelShaderConstantSlots,
		GraphicsDiscriptorIndex_FirstPixelSampler		= GraphicsDiscriptorIndex_FirstPixelTexture + GraphicsSystemLimits_PixelShaderTextureSlots,

		GraphicsDiscriptorIndex_Count					= GraphicsDiscriptorIndex_FirstPixelSampler + GraphicsSystemLimits_PixelShaderTextureSlots
	};
	
	class GraphicsSystemPlatform
	{
	public:

		static ID3D12Device*				getDevice( GraphicsSystem& graphicsSystem );
		static IDXGISwapChain1*				getSwapChain( GraphicsSystem& graphicsSystem );
		static ID3D12GraphicsCommandList*	getRenderCommandList( GraphicsSystem& graphicsSystem );
		static ID3D12RootSignature*			getRootSignature( GraphicsSystem& graphicsSystem );
		static UploadHeapD3d12&				getUploadHeap( GraphicsSystem& graphicsSystem );

		static DescriptorPoolD3d12&			getShaderResourcePool( GraphicsSystem& graphicsSystem );
		static DescriptorPoolD3d12&			getSamplerPool( GraphicsSystem& graphicsSystem );
		static DescriptorPoolD3d12&			getRenderTargetPool( GraphicsSystem& graphicsSystem );
		static DescriptorPoolD3d12&			getDepthStencilPool( GraphicsSystem& graphicsSystem );

		static DXGI_FORMAT					getD3dPixelFormat( PixelFormat pixelFormat, bool useTypelessFormat );
		static DXGI_FORMAT					getD3dIndexFormat( IndexType type );

		static ID3D12GraphicsCommandList*	lockResourceCommandList( GraphicsSystem& graphicsSystem );
		static void							unlockResourceCommandList( GraphicsSystem& graphicsSystem, ID3D12GraphicsCommandList*& pCommandList );

		static void							setResourceBarrier( ID3D12GraphicsCommandList* pCommandList, ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter );

		static bool							waitForGpu( GraphicsSystem& graphicsSystem );

		template<class T>
		static TIKI_FORCE_INLINE void safeRelease( T** ppObject )
		{
			if( *ppObject != nullptr )
			{
				char nameBuffer[ 1024 ] = "Unnamed";
				UINT length = TIKI_COUNT( nameBuffer );
				(*ppObject)->GetPrivateData( WKPDID_D3DDebugObjectName, &length, nameBuffer );
				if( length != 0 )
				{
					nameBuffer[ length ] = '\0';
				}

				ULONG refCount = (*ppObject)->Release();
				TIKI_ASSERT( refCount >= 0 );

				TIKI_TRACE_DEBUG( "[graphics] Release %s ref count is now: %i\n", nameBuffer, refCount );
				*ppObject = nullptr;
			}
		}

		template<class T>
		static TIKI_FORCE_INLINE void safeReleaseBla( T** ppObject )
		{
			if( *ppObject != nullptr )
			{
				(*ppObject)->Release();
				*ppObject = nullptr;
			}
		}
	};
}

#endif // TIKI_GRAPHICSSYSTEM_INTERNAL_D3D12_HPP__INCLUDED__
