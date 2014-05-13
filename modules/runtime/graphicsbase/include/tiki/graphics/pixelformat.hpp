#pragma once
#ifndef TIKI_PIXELFORMAT_HPP__INCLUDED
#define TIKI_PIXELFORMAT_HPP__INCLUDED

#include "tiki/base/assert.hpp"
#include "tiki/base/inline.hpp"

namespace tiki
{
	enum PixelFormat
	{
		PixelFormat_Invalid = -1,

		PixelFormat_R8,
		PixelFormat_R8G8B8A8,
		PixelFormat_R8G8B8A8_Gamma,
		PixelFormat_R16G16B16A16_Float,
		PixelFormat_R32_Float,
		PixelFormat_R32G32B32_Float,
		PixelFormat_R32G32B32A32_Float,
		PixelFormat_Depth24Stencil8,

		PixelFormat_Count,

		PixelFormat_Color	= PixelFormat_R8G8B8A8,
		PixelFormat_Depth	= PixelFormat_Depth24Stencil8
	};

	TIKI_FORCE_INLINE uint getBitsPerPixel( PixelFormat format )
	{
		TIKI_ASSERT( format < PixelFormat_Count );

		static const uint s_bitMapping[ PixelFormat_Count ] =
		{
			8u,		// PixelFormat_R8
			32u,	// PixelFormat_R8G8B8A8
			32u,	// PixelFormat_R8G8B8A8_Gamma
			64u,	// PixelFormat_R16G16B16A16_Float
			32u,	// PixelFormat_R32_Float
			96u,	// PixelFormat_R32G32B32_Float
			128u,	// PixelFormat_R32G32B32A32_Float
			32u,	// PixelFormat_Depth24Stencil8
		};

		return s_bitMapping[ format ];
	}
}

#endif // TIKI_PIXELFORMAT_HPP__INCLUDED
