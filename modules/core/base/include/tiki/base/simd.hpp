#pragma once
#ifndef TIKI_SIMD_HPP
#define TIKI_SIMD_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/inline.hpp"

#if TIKI_PLATFORM_WIN
#	include <emmintrin.h>
#	include <xmmintrin.h>
#endif

#define TIKI_USE_SIMDINLINE 1
#define TIKI_SIMD_ALIGNMENT 16u

#if TIKI_USE_SIMDINLINE
#	define TIKI_SIMD_INLINE TIKI_FORCE_INLINE
#else
#	define TIKI_SIMD_INLINE
#endif

namespace tiki
{
	typedef __m128	vf32;
	typedef __m128i	vi8;
	typedef __m128i	vi16;
	typedef __m128i	vi32;
	typedef __m128i	vu8;
	typedef __m128i	vu16;
	typedef __m128i	vu32;

	namespace simd
	{
		//////////////////////////////////////////////////////////////////////////
		// load

		TIKI_SIMD_INLINE vf32	set_f32( const float value );
		TIKI_SIMD_INLINE vf32	set_f32( const float x, const float y, const float z, const float w );
		TIKI_SIMD_INLINE vf32	set_f32( const float* pValues );
		TIKI_SIMD_INLINE vf32	set_f32( const float* pValues, const size_t offset );
		TIKI_SIMD_INLINE vf32	set_f32u( const float* pValues );
		TIKI_SIMD_INLINE vf32	set_f32u( const float* pValues, const size_t offset );

		TIKI_SIMD_INLINE vi32	set_i32( const sint32 value );
		TIKI_SIMD_INLINE vi32	set_i32( const sint32 x, const sint32 y, const sint32 z, const sint32 w );
		TIKI_SIMD_INLINE vi32	set_i32( const sint32* pValues );
		TIKI_SIMD_INLINE vi32	set_i32( const sint32* pValues, const size_t offset );
		TIKI_SIMD_INLINE vi32	set_i32u( const sint32* pValues );
		TIKI_SIMD_INLINE vi32	set_i32u( const sint32* pValues, const size_t offset );

		TIKI_SIMD_INLINE vi16	set_i16( const sint16 value );
		TIKI_SIMD_INLINE vi16	set_i16( const sint16 a, const sint16 b, const sint16 c, const sint16 d, const sint16 e, const sint16 f, const sint16 g, const sint16 h );
		TIKI_SIMD_INLINE vi16	set_i16( const sint16* pValues );
		TIKI_SIMD_INLINE vi16	set_i16( const sint16* pValues, const size_t offset );
		TIKI_SIMD_INLINE vi16	set_i16u( const sint16* pValues );
		TIKI_SIMD_INLINE vi16	set_i16u( const sint16* pValues, const size_t offset );

		//TIKI_SIMD_INLINE vi8	set_i8( sint8 value );
		//TIKI_SIMD_INLINE vi8	set_i8( const sint8* value );
		//TIKI_SIMD_INLINE vi8	set_i8( const sint8* value, size_t offset );

		//TIKI_SIMD_INLINE vu32	set_u32( uint32 value );
		//TIKI_SIMD_INLINE vu32	set_u32( const uint32* value );
		//TIKI_SIMD_INLINE vu32	set_u32( const uint32* value, size_t offset );

		//TIKI_SIMD_INLINE vu16	set_u16( uint16 value );
		//TIKI_SIMD_INLINE vu16	set_u16( const uint16* value );
		//TIKI_SIMD_INLINE vu16	set_u16( const uint16* value, size_t offset );

		//TIKI_SIMD_INLINE vu8	set_u8( uint8 value );
		//TIKI_SIMD_INLINE vu8	set_u8( const uint8* value );
		//TIKI_SIMD_INLINE vu8	set_u8( const uint8* value, size_t offset );


		//////////////////////////////////////////////////////////////////////////
		// store

		TIKI_SIMD_INLINE void	get_f32( float* pTarget, const vf32 value );
		TIKI_SIMD_INLINE void	get_f32( float* pTarget, const size_t offset, const vf32 value );
		TIKI_SIMD_INLINE float	get_f32_x( const vf32 value );
		TIKI_SIMD_INLINE float	get_f32_y( const vf32 value );
		TIKI_SIMD_INLINE float	get_f32_z( const vf32 value );
		TIKI_SIMD_INLINE float	get_f32_w( const vf32 value );

		TIKI_SIMD_INLINE void	get_i32( sint32* pTarget, const vi32 value );
		TIKI_SIMD_INLINE void	get_i32( sint32* pTarget, const size_t offset, const vi32 value );
		TIKI_SIMD_INLINE sint32	get_i32_x( const vi32 value );
		TIKI_SIMD_INLINE sint32	get_i32_y( const vi32 value );
		TIKI_SIMD_INLINE sint32	get_i32_z( const vi32 value );
		TIKI_SIMD_INLINE sint32	get_i32_w( const vi32 value );

		TIKI_SIMD_INLINE void	get_i16( sint16* pTarget, const vi16 value );
		TIKI_SIMD_INLINE void	get_i16( sint16* pTarget, const size_t offset, const vi16 value );
		TIKI_SIMD_INLINE sint16	get_i16_x( const vi16 value );
		TIKI_SIMD_INLINE sint16	get_i16_y( const vi16 value );
		TIKI_SIMD_INLINE sint16	get_i16_z( const vi16 value );
		TIKI_SIMD_INLINE sint16	get_i16_w( const vi16 value );

		//TIKI_SIMD_INLINE void	get_i8( sint8* pTarget, vi8 value);
		//TIKI_SIMD_INLINE void	get_i8( sint8* pTarget, size_t offset, vi8 value);
		//TIKI_SIMD_INLINE sint8	get_x_i8( vi8 value );
		//TIKI_SIMD_INLINE sint8	get_y_i8( vi8 value );
		//TIKI_SIMD_INLINE sint8	get_z_i8( vi8 value );
		//TIKI_SIMD_INLINE sint8	get_w_i8( vi8 value );

		//TIKI_SIMD_INLINE void	get_u32( sint32* pTarget, vu32 value);
		//TIKI_SIMD_INLINE void	get_u32( sint32* pTarget, size_t offset, vu32 value);
		//TIKI_SIMD_INLINE sint32	get_x_u32( vu32 value );
		//TIKI_SIMD_INLINE sint32	get_y_u32( vu32 value );
		//TIKI_SIMD_INLINE sint32	get_z_u32( vu32 value );
		//TIKI_SIMD_INLINE sint32	get_w_u32( vu32 value );

		//TIKI_SIMD_INLINE void	get_u16( sint16* pTarget, vu16 value);
		//TIKI_SIMD_INLINE void	get_u16( sint16* pTarget, size_t offset, vu16 value);
		//TIKI_SIMD_INLINE sint16	get_x_u16( vu16 value );
		//TIKI_SIMD_INLINE sint16	get_y_u16( vu16 value );
		//TIKI_SIMD_INLINE sint16	get_z_u16( vu16 value );
		//TIKI_SIMD_INLINE sint16	get_w_u16( vu16 value );

		//TIKI_SIMD_INLINE void	get_u8( sint8* pTarget, vu8 value);
		//TIKI_SIMD_INLINE void	get_u8( sint8* pTarget, size_t offset, vu8 value);
		//TIKI_SIMD_INLINE sint8	get_x_u8( vu8 value );
		//TIKI_SIMD_INLINE sint8	get_y_u8( vu8 value );
		//TIKI_SIMD_INLINE sint8	get_z_u8( vu8 value );
		//TIKI_SIMD_INLINE sint8	get_w_u8( vu8 value );


		//////////////////////////////////////////////////////////////////////////
		// arithmetic

		TIKI_SIMD_INLINE vf32	add_f32( const vf32 v1, const vf32 v2 );
		TIKI_SIMD_INLINE vf32	sub_f32( const vf32 v1, const vf32 v2 );
		TIKI_SIMD_INLINE vf32	mul_f32( const vf32 v1, const vf32 v2 );
		TIKI_SIMD_INLINE vf32	div_f32( const vf32 v1, const vf32 v2 );				
		TIKI_SIMD_INLINE vf32	muladd_f32( const vf32 v1, const vf32 v2, const vf32 v3 );	// v1 * v2 + v3		
		TIKI_SIMD_INLINE vf32	mulsub_f32( const vf32 v1, const vf32 v2, const vf32 v3 );	// v1 * v2 - v3

		TIKI_SIMD_INLINE vi32	add_i32( const vi32 v1, const vi32 v2 );
		TIKI_SIMD_INLINE vi32	sub_i32( const vi32 v1, const vi32 v2 );
		TIKI_SIMD_INLINE vi32	mul_i32( const vi32 v1, const vi32 v2 );
		TIKI_SIMD_INLINE vi32	div_i32( const vi32 v1, const vi32 v2 );
		TIKI_SIMD_INLINE vi32	muladd_i32( const vi32 v1, const vi32 v2, const vi32 v3 );	// v1 * v2 + v3		
		TIKI_SIMD_INLINE vi32	mulsub_i32( const vi32 v1, const vi32 v2, const vi32 v3 );	// v1 * v2 - v3

		//TIKI_SIMD_INLINE vi16	add_i16( vi16 v1, vi16 v2 );
		//TIKI_SIMD_INLINE vi16	sub_i16( vi16 v1, vi16 v2 );
		//TIKI_SIMD_INLINE vi16	mul_i16( vi16 v1, vi16 v2 );
		//TIKI_SIMD_INLINE vi16	div_i16( vi16 v1, vi16 v2 );
		//TIKI_SIMD_INLINE vi16	madd_i16( vi16 v1, vi16 v2, vi16 v3 );	// v1 * v2 + v3		
		//TIKI_SIMD_INLINE vi16	msub_i16( vi16 v1, vi16 v2, vi16 v3 );	// v1 * v2 - v3

		//TIKI_SIMD_INLINE vi8	add_i8( vi8 v1, vi8 v2 );
		//TIKI_SIMD_INLINE vi8	sub_i8( vi8 v1, vi8 v2 );
		//TIKI_SIMD_INLINE vi8	mul_i8( vi8 v1, vi8 v2 );
		//TIKI_SIMD_INLINE vi8	div_i8( vi8 v1, vi8 v2 );
		//TIKI_SIMD_INLINE vi8	madd_i8( vi8 v1, vi8 v2, vi8 v3 );	// v1 * v2 + v3		
		//TIKI_SIMD_INLINE vi8	msub_i8( vi8 v1, vi8 v2, vi8 v3 );	// v1 * v2 - v3

		//TIKI_SIMD_INLINE vu32	add_u32( vu32 v1, vu32 v2 );
		//TIKI_SIMD_INLINE vu32	sub_u32( vu32 v1, vu32 v2 );
		//TIKI_SIMD_INLINE vu32	mul_u32( vu32 v1, vu32 v2 );
		//TIKI_SIMD_INLINE vu32	div_u32( vu32 v1, vu32 v2 );
		//TIKI_SIMD_INLINE vu32	madd_u32( vu32 v1, vu32 v2, vu32 v3 );	// v1 * v2 + v3		
		//TIKI_SIMD_INLINE vu32	msub_u32( vu32 v1, vu32 v2, vu32 v3 );	// v1 * v2 - v3

		//TIKI_SIMD_INLINE vu16	add_u16( vu16 v1, vu16 v2 );
		//TIKI_SIMD_INLINE vu16	sub_u16( vu16 v1, vu16 v2 );
		//TIKI_SIMD_INLINE vu16	mul_u16( vu16 v1, vu16 v2 );
		//TIKI_SIMD_INLINE vu16	div_u16( vu16 v1, vu16 v2 );
		//TIKI_SIMD_INLINE vu16	madd_u16( vu16 v1, vu16 v2, vu16 v3 );	// v1 * v2 + v3		
		//TIKI_SIMD_INLINE vu16	msub_u16( vu16 v1, vu16 v2, vu16 v3 );	// v1 * v2 - v3

		//TIKI_SIMD_INLINE vu8	add_u8( vu8 v1, vu8 v2 );
		//TIKI_SIMD_INLINE vu8	sub_u8( vu8 v1, vu8 v2 );
		//TIKI_SIMD_INLINE vu8	mul_u8( vu8 v1, vu8 v2 );
		//TIKI_SIMD_INLINE vu8	div_u8( vu8 v1, vu8 v2 );
		//TIKI_SIMD_INLINE vu8	madd_u8( vu8 v1, vu8 v2, vu8 v3 );	// v1 * v2 + v3		
		//TIKI_SIMD_INLINE vu8	msub_u8( vu8 v1, vu8 v2, vu8 v3 );	// v1 * v2 - v3


		//////////////////////////////////////////////////////////////////////////
		// compare

		//////////////////////////////////////////////////////////////////////////
		// convert

		TIKI_SIMD_INLINE vf32	convert_i32_to_f32( const vi32 value );

		TIKI_SIMD_INLINE vi32	convert_i16l_to_i32( const vi16 value ); // (vi16)0,1,2,3 => (vi32)0,1,2,3
		TIKI_SIMD_INLINE vi32	convert_i16h_to_i32( const vi16 value ); // (vi16)4,5,6,7 => (vi32)0,1,2,3

		//////////////////////////////////////////////////////////////////////////
		// special

		TIKI_SIMD_INLINE vf32	shuffle_f32( const vf32 v1, const vf32 v2, size_t mask );

		TIKI_SIMD_INLINE vf32	splat_x_f32( const vf32 value );
		TIKI_SIMD_INLINE vf32	splat_y_f32( const vf32 value );
		TIKI_SIMD_INLINE vf32	splat_z_f32( const vf32 value );
		TIKI_SIMD_INLINE vf32	splat_w_f32( const vf32 value );

		TIKI_SIMD_INLINE vf32	rsqrt_f32( const vf32 value ); // 1.0f / sqrt( value )
		TIKI_SIMD_INLINE vf32	negmulsub_f32( const vf32 v1, const vf32 v2, const vf32 v3 );
		TIKI_SIMD_INLINE vf32	dot4_f32( const vf32 v1, const vf32 v2 );

	}
}

#if TIKI_PLATFORM_WIN
#	include "simd_win.inl"
#endif

#endif // TIKI_SIMD_HPP
