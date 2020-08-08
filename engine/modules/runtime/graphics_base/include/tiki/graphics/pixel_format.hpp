#pragma once

#include "tiki/base/assert.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	enum PixelFormat : uint8
	{
		PixelFormat_R8,
		PixelFormat_R8G8B8A8,
		PixelFormat_R8G8B8A8_Gamma,
		PixelFormat_R10G10B10A2,
		PixelFormat_R16G16B16A16_Float,
		PixelFormat_R32_Float,
		PixelFormat_R32G32B32_Float,
		PixelFormat_R32G32B32A32_Float,

		PixelFormat_Depth24Stencil8,

		PixelFormat_Count,
		PixelFormat_Invalid	= 0xffu
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
