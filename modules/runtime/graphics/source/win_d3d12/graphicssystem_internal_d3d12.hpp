#ifndef TIKI_GRAPHICSSYSTEM_INTERNAL_D3D12_HPP__INCLUDED__
#define TIKI_GRAPHICSSYSTEM_INTERNAL_D3D12_HPP__INCLUDED__

#include "graphicstypes_d3d12.hpp"

#include <dxgi.h>
#include <d3d12.h>

namespace tiki
{
	class GraphicsSystem;
	class UploadHeapD3d12;
	struct GraphicsSystemPlatformData;

	namespace graphics
	{
		GraphicsSystemPlatformData& getPlatformData( GraphicsSystem& graphicSystem );
		ID3D12Device*				getDevice( GraphicsSystem& graphicsSystem );
		ID3D12GraphicsCommandList*	getCommandList( GraphicsSystem& graphicsSystem );
		UploadHeapD3d12&			getUploadHeap( GraphicsSystem& graphicsSystem );

		DXGI_FORMAT					getD3dIndexFormat( IndexType type );

		void						setResourceBarrier( ID3D12GraphicsCommandList* pCommandList, ID3D12Resource* pResource, UINT stateBefore, UINT stateAfter );

		template<class T>
		TIKI_FORCE_INLINE void safeRelease( T** ppObject )
		{
			if( *ppObject != nullptr )
			{
				(*ppObject)->Release();
				*ppObject = nullptr;
			}
		}
	}
}

#endif // TIKI_GRAPHICSSYSTEM_INTERNAL_D3D12_HPP__INCLUDED__
