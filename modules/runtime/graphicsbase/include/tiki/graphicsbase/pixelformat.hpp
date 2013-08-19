#pragma once
#ifndef TIKI_PIXELFORMAT_HPP__INCLUDED
#define TIKI_PIXELFORMAT_HPP__INCLUDED

#include "tiki/base/inline.hpp"

namespace tiki
{
	enum PixelFormat
	{
		PixelFormat_Invalid,

		PixelFormat_R8,

		PixelFormat_R8G8B8A8,
		PixelFormat_R8G8B8A8_Gamma,

		PixelFormat_R32_Float,
		PixelFormat_R32G32B32_Float,
		PixelFormat_R32G32B32A32_Float,

		PixelFormat_Depth24Stencil8,

		PixelFormat_Count
	};

	TIKI_FORCE_INLINE size_t getBitsPerPixel( PixelFormat format )
	{
		TIKI_ASSERT( format < PixelFormat_Count );

		switch ( format )
		{
			case PixelFormat_R8:
				return 8u;
			case PixelFormat_R8G8B8A8:
				return 32u;
			case PixelFormat_R8G8B8A8_Gamma:
				return 32u;
			case PixelFormat_R32_Float:
				return 16u;
			case PixelFormat_R32G32B32_Float:
				return 48u;
			case PixelFormat_R32G32B32A32_Float:
				return 64u;
			case PixelFormat_Depth24Stencil8:
				return 32u;
		}

		TIKI_BREAK( "[graphics] Unkown pixel format.\n" );
		return s_unsignedMinusOne;
	}
}

#endif // TIKI_PIXELFORMAT_HPP__INCLUDED
