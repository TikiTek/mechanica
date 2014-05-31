#ifndef TIKI_BASEBUFFER_HPP__
#define TIKI_BASEBUFFER_HPP__

#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	enum GraphicsBufferType
	{
		GraphicsBufferType_ConstantBuffer,
		GraphicsBufferType_IndexBuffer,
		GraphicsBufferType_VertexBuffer
	};
}

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/basebuffer_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "../../../source/global_opengl4/basebuffer_opengl4.hpp"
#endif

#endif // TIKI_BASEBUFFER_HPP__
