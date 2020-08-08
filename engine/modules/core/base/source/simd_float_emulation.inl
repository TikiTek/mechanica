#pragma once
#ifndef __TIKI_SIMD_FLOATEMULATION_INL_INCLUDED__
#define __TIKI_SIMD_FLOATEMULATION_INL_INCLUDED__

namespace tiki
{
#warning not implemented
	//////////////////////////////////////////////////////////////////////////
	// load

	TIKI_SIMD_INLINE vf32	simd::set_f32( const float value )												{ return SimdVector(); }
	TIKI_SIMD_INLINE vf32	simd::set_f32( const float x, const float y, const float z, const float w )		{ return SimdVector(); }
	TIKI_SIMD_INLINE vf32	simd::set_f32( const float* pValues )											{ return SimdVector(); }
	TIKI_SIMD_INLINE vf32	simd::set_f32( const float* pValues, const uintreg offset )						{ return SimdVector(); }
	TIKI_SIMD_INLINE vf32	simd::set_f32u( const float* pValues )											{ return SimdVector(); }
	TIKI_SIMD_INLINE vf32	simd::set_f32u( const float* pValues, const uintreg offset )					{ return SimdVector(); }

	TIKI_SIMD_INLINE vi32	simd::set_i32( const sint32 value )												{ return SimdVector(); }
	TIKI_SIMD_INLINE vi32	simd::set_i32( const sint32 x, const sint32 y, const sint32 z, const sint32 w )	{ return SimdVector(); }
	TIKI_SIMD_INLINE vi32	simd::set_i32( const sint32* pValues )											{ return SimdVector(); }
	TIKI_SIMD_INLINE vi32	simd::set_i32( const sint32* pValues, const uintreg offset )					{ return SimdVector(); }
	TIKI_SIMD_INLINE vi32	simd::set_i32u( const sint32* pValues )											{ return SimdVector(); }
	TIKI_SIMD_INLINE vi32	simd::set_i32u( const sint32* pValues, const uintreg offset )					{ return SimdVector(); }

	TIKI_SIMD_INLINE vi16	simd::set_i16( const sint16 value )												{ return SimdVector(); }
	TIKI_SIMD_INLINE vi16	simd::set_i16( const sint16 a, const sint16 b, const sint16 c, const sint16 d, const sint16 e, const sint16 f, const sint16 g, const sint16 h )	{ return SimdVector(); }
	TIKI_SIMD_INLINE vi16	simd::set_i16( const sint16* pValues )											{ return SimdVector(); }
	TIKI_SIMD_INLINE vi16	simd::set_i16( const sint16* pValues, const uintreg offset )					{ return SimdVector(); }
	TIKI_SIMD_INLINE vi16	simd::set_i16u( const sint16* pValues )											{ return SimdVector(); }
	TIKI_SIMD_INLINE vi16	simd::set_i16u( const sint16* pValues, const uintreg offset )					{ return SimdVector(); }

	//////////////////////////////////////////////////////////////////////////
	// store

	TIKI_SIMD_INLINE void	simd::get_f32( float* pTarget, const vf32 value)								{ }
	TIKI_SIMD_INLINE void	simd::get_f32( float* pTarget, const uintreg offset, const vf32 value)			{ }
	TIKI_SIMD_INLINE float	simd::get_f32_x( const vf32 value )												{ return value.f32s.x; }
	TIKI_SIMD_INLINE float	simd::get_f32_y( const vf32 value )												{ return value.f32s.y; }
	TIKI_SIMD_INLINE float	simd::get_f32_z( const vf32 value )												{ return value.f32s.z; }
	TIKI_SIMD_INLINE float	simd::get_f32_w( const vf32 value )												{ return value.f32s.w; }

	TIKI_SIMD_INLINE void	simd::get_i32( sint32* pTarget, const vi32 value )								{ }
	TIKI_SIMD_INLINE void	simd::get_i32( sint32* pTarget, const uintreg offset, const vi32 value )		{ }
	TIKI_SIMD_INLINE sint32	simd::get_i32_x( const vi32 value )												{ return value.s32s.x; }
	TIKI_SIMD_INLINE sint32	simd::get_i32_y( const vi32 value )												{ return value.s32s.y; }
	TIKI_SIMD_INLINE sint32	simd::get_i32_z( const vi32 value )												{ return value.s32s.z; }
	TIKI_SIMD_INLINE sint32	simd::get_i32_w( const vi32 value )												{ return value.s32s.w; }


	//////////////////////////////////////////////////////////////////////////
	// arithmetic

	TIKI_SIMD_INLINE vf32	simd::add_f32( const vf32 v1, const vf32 v2 )									{ return SimdVector(); }
	TIKI_SIMD_INLINE vf32	simd::sub_f32( const vf32 v1, const vf32 v2 )									{ return SimdVector(); }
	TIKI_SIMD_INLINE vf32	simd::mul_f32( const vf32 v1, const vf32 v2 )									{ return SimdVector(); }
	TIKI_SIMD_INLINE vf32	simd::div_f32( const vf32 v1, const vf32 v2 )									{ return SimdVector(); }
	TIKI_SIMD_INLINE vf32	simd::muladd_f32( const vf32 v1, const vf32 v2, const vf32 v3 )					{ return SimdVector(); } // r = v1 * v2 + v3
	TIKI_SIMD_INLINE vf32	simd::mulsub_f32( const vf32 v1, const vf32 v2, const vf32 v3 )					{ return SimdVector(); } // r = v1 * v2 - v3
	TIKI_SIMD_INLINE vf32	simd::negmulsub_f32( const vf32 v1, const vf32 v2, const vf32 v3 )				{ return SimdVector(); } // r = v3 - v1 * v1

	TIKI_SIMD_INLINE vi32	simd::add_i32( const vi32 v1, const vi32 v2 )									{ return SimdVector(); }
	TIKI_SIMD_INLINE vi32	simd::sub_i32( const vi32 v1, const vi32 v2 )									{ return SimdVector(); }
	//TIKI_SIMD_INLINE vi32	simd::mul_i32( const vi32 v1, const vi32 v2 )									{ return _mm_mul_epi32( v1, v2 ); }
	//TIKI_SIMD_INLINE vi32	simd::div_i32( const vi32 v1, const vi32 v2 )									{ return _mm_div_si32( v1, v2 ); }
	//TIKI_SIMD_INLINE vi32	simd::muladd_i32( const vi32 v1, const vi32 v2, const vi32 v3 )					{ return _mm_add_epi32( _mm_mul_epi32( v1, v2 ), v3 ); }
	//TIKI_SIMD_INLINE vi32	simd::mulsub_i32( const vi32 v1, const vi32 v2, const vi32 v3 )					{ return _mm_sub_epi32( _mm_mul_epi32( v1, v2 ), v3 ); }


	//////////////////////////////////////////////////////////////////////////
	// convert

	TIKI_SIMD_INLINE vf32	simd::convert_i32_to_f32( const vi32 value )									{ return SimdVector(); }

	TIKI_SIMD_INLINE vi32	simd::convert_i16l_to_i32( const vi16 value )									{ return SimdVector(); }
	TIKI_SIMD_INLINE vi32	simd::convert_i16h_to_i32( const vi16 value )									{ return SimdVector(); }


	//////////////////////////////////////////////////////////////////////////
	// special

	TIKI_SIMD_INLINE vf32	simd::splat_x_f32( const vf32 value )											{ const SimdVector result = { value.f32s.x, value.f32s.x, value.f32s.x, value.f32s.x }; return result; }
	TIKI_SIMD_INLINE vf32	simd::splat_y_f32( const vf32 value )											{ const SimdVector result = { value.f32s.y, value.f32s.y, value.f32s.y, value.f32s.y }; return result; }
	TIKI_SIMD_INLINE vf32	simd::splat_z_f32( const vf32 value )											{ const SimdVector result = { value.f32s.z, value.f32s.z, value.f32s.z, value.f32s.z }; return result; }
	TIKI_SIMD_INLINE vf32	simd::splat_w_f32( const vf32 value )											{ const SimdVector result = { value.f32s.w, value.f32s.w, value.f32s.w, value.f32s.w }; return result; }

	TIKI_SIMD_INLINE vf32	simd::rsqrt_f32( const vf32 value )												{ return SimdVector(); } // r = 1.0f / sqrt( value )
	TIKI_SIMD_INLINE vf32	simd::dot4_f32( const vf32 v1, const vf32 v2 )									{ const float dot = (v1.f32s.x * v2.f32s.x) + (v1.f32s.y * v2.f32s.y) + (v1.f32s.z * v2.f32s.z) + (v1.f32s.w * v2.f32s.w); const SimdVector result = { dot, dot, dot, dot }; return result; }
}

#endif // __TIKI_SIMD_FLOATEMULATION_INL_INCLUDED__
