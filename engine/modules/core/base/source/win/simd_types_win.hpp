#pragma once
#ifndef __TIKI_SIMD_TYPES_WIN_HPP_INCLUDED__
#define __TIKI_SIMD_TYPES_WIN_HPP_INCLUDED__

#include <emmintrin.h>
#include <xmmintrin.h>

namespace tiki
{
	typedef __m128	vf32;
	typedef __m128i	vi8;
	typedef __m128i	vi16;
	typedef __m128i	vi32;
	typedef __m128i	vu8;
	typedef __m128i	vu16;
	typedef __m128i	vu32;
}

#endif // __TIKI_SIMD_TYPES_WIN_HPP_INCLUDED__
