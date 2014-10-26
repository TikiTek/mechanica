#ifndef TIKI_GRAPHICSSYSTEM_INTERNAL_D3D12_HPP__INCLUDED__
#define TIKI_GRAPHICSSYSTEM_INTERNAL_D3D12_HPP__INCLUDED__

#include "graphicstypes_d3d12.hpp"

#include <dxgi.h>
#include <d3d12.h>

namespace tiki
{
	class GraphicsSystem;

	namespace graphics
	{
		ID3D12Device*			getDevice( GraphicsSystem& graphicsSystem );
		ID3D12CommandQueue*		getCommandQueue( GraphicsSystem& graphicsSystem );
	}
}

#endif // TIKI_GRAPHICSSYSTEM_INTERNAL_D3D12_HPP__INCLUDED__
