#pragma once
#ifndef TIKI_FOURCC_HPP
#define TIKI_FOURCC_HPP

#include "tiki/base/types.hpp"

#define TIKI_FOURCC( a, b, c, d ) ((fourcc)((((fourcc)d) << 24) | (((fourcc)c) << 16) | (((fourcc)b) << 8) | ((fourcc)a)))

namespace tiki
{
	TIKI_FORCE_INLINE fourcc makeFourcc( const char* pString )
	{
		return ((const fourcc*)pString)[ 0u ];
	}

	TIKI_FORCE_INLINE fourcc makeFourcc( uint8 a, uint8 b, uint8 c, uint8 d )
	{
		return ((((fourcc)d) << 24) | (((fourcc)c) << 16) | (((fourcc)b) << 8) | ((fourcc)a));
	}
}

#endif // TIKI_FOURCC_HPP
