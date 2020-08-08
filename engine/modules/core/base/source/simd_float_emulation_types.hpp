#pragma once
#ifndef __TIKI_SIMD_FLOATEMULATION_TYPES_HPP_INCLUDED__
#define __TIKI_SIMD_FLOATEMULATION_TYPES_HPP_INCLUDED__

namespace tiki
{
	TIKI_PRE_ALIGN( 16 ) union SimdVector
	{
		float																f32a[ 4u ];
		struct { float x, y, z, w; }										f32s;
		sint32																s32a[ 4u ];
		struct { sint32 x, y, z, w; }										s32s;
		uint32																u32a[ 4u ];
		struct { uint32 x, y, z, w; }										u32s;
		sint16																s16a[ 8u ];
		struct { sint16 a, b, c, d, e, f, g, h; }							s16s;
		uint16																u16a[ 8u ];
		struct { uint16 a, b, c, d, e, f, g, h; }							u16s;
		sint8																s8a[ 16u ];
		struct { sint8 a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p; }	s8s;
		uint8																u8a[ 16u ];
		struct { uint8 a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p; }	u8s;
	} TIKI_POST_ALIGN( 16 ) ;

	typedef SimdVector	vf32;
	typedef SimdVector	vi8;
	typedef SimdVector	vi16;
	typedef SimdVector	vi32;
	typedef SimdVector	vu8;
	typedef SimdVector	vu16;
	typedef SimdVector	vu32;
}

#endif // __TIKI_SIMD_FLOATEMULATION_TYPES_HPP_INCLUDED__
