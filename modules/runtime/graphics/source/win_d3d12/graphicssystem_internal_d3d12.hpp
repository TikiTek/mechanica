#ifndef TIKI_GRAPHICSSYSTEM_INTERNAL_D3D12_HPP__INCLUDED__
#define TIKI_GRAPHICSSYSTEM_INTERNAL_D3D12_HPP__INCLUDED__

#include "graphicstypes_d3d12.hpp"

#include "tiki/graphics/pixelformat.hpp"

#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3dx12.h>

namespace tiki
{
	class GraphicsSystem;
	class UploadHeapD3d12;
	struct GraphicsSystemPlatformData;
	enum IndexType;
	
	class GraphicsSystemPlatform
	{
	public:

		static ID3D12Device*				getDevice( GraphicsSystem& graphicsSystem );
		static ID3D12GraphicsCommandList*	getCommandList( GraphicsSystem& graphicsSystem );
		static ID3D12RootSignature*			getRootSignature( GraphicsSystem& graphicsSystem );
		static UploadHeapD3d12&				getUploadHeap( GraphicsSystem& graphicsSystem );

		static DXGI_FORMAT					getD3dPixelFormat( PixelFormat pixelFormat, bool useTypelessFormat );
		static DXGI_FORMAT					getD3dIndexFormat( IndexType type );

		static void							setResourceBarrier( ID3D12GraphicsCommandList* pCommandList, ID3D12Resource* pResource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter );

		template<class T>
		static TIKI_FORCE_INLINE void safeRelease( T** ppObject )
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
