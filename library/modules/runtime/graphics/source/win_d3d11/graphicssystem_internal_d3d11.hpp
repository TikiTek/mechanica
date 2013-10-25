#ifndef TIKI_GRAPHICSSYSTEM_INTERNAL_D3D11_HPP
#define TIKI_GRAPHICSSYSTEM_INTERNAL_D3D11_HPP

#include "graphicstypes_d3d11.hpp"

#include <dxgi.h>
#include <d3d11.h>

namespace tiki
{
	class GraphicsSystem;

	namespace graphics
	{
		TGDevice*	getDevice( GraphicsSystem& graphicsSystem );
		TGContext*	getContext( GraphicsSystem& graphicsSystem );
	}
}

#endif // TIKI_GRAPHICSSYSTEM_INTERNAL_D3D11_HPP