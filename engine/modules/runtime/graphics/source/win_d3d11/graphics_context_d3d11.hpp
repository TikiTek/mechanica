#pragma once

#include "graphics_types_d3d11.hpp"

namespace tiki
{
	struct GraphicsContextPlatformData
	{
		GraphicsContextPlatformData()
		{
			pDevice					= nullptr;
			pContext				= nullptr;

			immediateVertexCount	= 0u;
			immediateVertexStride	= 0u;
		}

		ID3D11Device*			pDevice;
		ID3D11DeviceContext*	pContext;

		uint					immediateVertexCount;
		uint					immediateVertexStride;
	};
}
