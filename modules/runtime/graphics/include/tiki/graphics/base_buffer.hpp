#ifndef TIKI_BASEBUFFER_HPP__
#define TIKI_BASEBUFFER_HPP__

#include "tiki/base/types.hpp"

namespace tiki
{
	enum BaseBufferTypes : uint8
	{
		BaseBufferTypes_ConstantBuffer,
		BaseBufferTypes_IndexBuffer,
		BaseBufferTypes_VertexBuffer,

		BaseBufferTypes_Count
	};
}

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/basebuffer_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
#	include "../../../source/win_d3d12/basebuffer_d3d12.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_VULKAN )
#	include "../../../source/global_vulkan/basebuffer_vulkan.hpp"
#endif

#endif // TIKI_BASEBUFFER_HPP__
