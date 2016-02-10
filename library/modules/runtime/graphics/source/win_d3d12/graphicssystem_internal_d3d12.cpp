#include "graphicssystem_internal_d3d12.hpp"

#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/indexbuffer.hpp"

namespace tiki
{
	/*static*/ ID3D12Device* GraphicsSystemPlatform::getDevice( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.pDevice;
	}

	/*static*/ ID3D12GraphicsCommandList* GraphicsSystemPlatform::getCommandList( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.pCommandList;
	}

	/*static*/ ID3D12RootSignature* GraphicsSystemPlatform::getRootSignature( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.pRootSignature;
	}

	/*static*/ UploadHeapD3d12& GraphicsSystemPlatform::getUploadHeap( GraphicsSystem& graphicsSystem )
	{
		return graphicsSystem.m_platformData.uploadHeap;
	}

	/*static*/ DXGI_FORMAT GraphicsSystemPlatform::getD3dPixelFormat( PixelFormat pixelFormat, bool useTypelessFormat )
	{
		TIKI_ASSERT( pixelFormat < PixelFormat_Count );

		static DXGI_FORMAT s_formatLookup[] =
		{
			DXGI_FORMAT_R8_UNORM,				// PixelFormat_R8,
			DXGI_FORMAT_R8G8B8A8_UNORM,			// PixelFormat_R8G8B8A8
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,	// PixelFormat_R8G8B8A8_Gamma
			DXGI_FORMAT_R16G16B16A16_FLOAT,		// PixelFormat_R16G16B16A16_Float
			DXGI_FORMAT_R32_FLOAT,				// PixelFormat_R32_Float
			DXGI_FORMAT_R32G32B32_FLOAT,		// PixelFormat_R32G32B32_Float
			DXGI_FORMAT_R32G32B32A32_FLOAT,		// PixelFormat_R32G32B32A32_Float
			DXGI_FORMAT_D24_UNORM_S8_UINT		// PixelFormat_Depth24Stencil8
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_formatLookup ) == PixelFormat_Count );

		if( useTypelessFormat && pixelFormat == PixelFormat_Depth24Stencil8 )
		{
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		}

		return s_formatLookup[ pixelFormat ];
	}

	/*static*/ DXGI_FORMAT GraphicsSystemPlatform::getD3dIndexFormat( IndexType type )
	{
		if( type == IndexType_UInt16 )
		{
			return DXGI_FORMAT_R16_UINT;
		}
		else
		{
			return DXGI_FORMAT_R32_UINT;
		}
	}

	/*static*/ void GraphicsSystemPlatform::setResourceBarrier( ID3D12GraphicsCommandList* pCommandList, ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter )
	{
		TIKI_DECLARE_STACKANDZERO( D3D12_RESOURCE_BARRIER, descBarrier );
		descBarrier.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		descBarrier.Transition.pResource	= pResource;
		descBarrier.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		descBarrier.Transition.StateBefore	= stateBefore;
		descBarrier.Transition.StateAfter	= stateAfter;

		pCommandList->ResourceBarrier( 1, &descBarrier );
	}
}
