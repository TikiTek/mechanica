#pragma once
#ifndef TIKI_SIMD_WIN_INL
#define TIKI_SIMD_WIN_INL

namespace tiki
{
	//////////////////////////////////////////////////////////////////////////
	// load

	TIKI_SIMD_INLINE vf32	simd::set_f32( const float value )												{ return _mm_set_ps1( value ); }
	TIKI_SIMD_INLINE vf32	simd::set_f32( const float x, const float y, const float z, const float w )		{ return _mm_set_ps( x, y, z, w ); }
	TIKI_SIMD_INLINE vf32	simd::set_f32( const float* pValues )											{ return _mm_load_ps( pValues ); }
	TIKI_SIMD_INLINE vf32	simd::set_f32( const float* pValues, const size_t offset )						{ return _mm_load_ps( pValues + offset ); }
	TIKI_SIMD_INLINE vf32	simd::set_f32u( const float* pValues )											{ return _mm_loadu_ps( pValues ); }
	TIKI_SIMD_INLINE vf32	simd::set_f32u( const float* pValues, const size_t offset )						{ return _mm_loadu_ps( pValues + offset ); }

	TIKI_SIMD_INLINE vi32	simd::set_i32( const sint32 value )												{ return _mm_set1_epi32( value ); }
	TIKI_SIMD_INLINE vi32	simd::set_i32( const sint32 x, const sint32 y, const sint32 z, const sint32 w )	{ return _mm_set_epi32( x, y, z, w ); }
	TIKI_SIMD_INLINE vi32	simd::set_i32( const sint32* pValues )											{ return _mm_load_si128( (vi32*)pValues ); }
	TIKI_SIMD_INLINE vi32	simd::set_i32( const sint32* pValues, const size_t offset )						{ return _mm_load_si128( (vi32*)(pValues + offset) ); }
	TIKI_SIMD_INLINE vi32	simd::set_i32u( const sint32* pValues )											{ return _mm_loadu_si128( (vi32*)pValues ); }
	TIKI_SIMD_INLINE vi32	simd::set_i32u( const sint32* pValues, const size_t offset )					{ return _mm_loadu_si128( (vi32*)(pValues + offset) ); }

	TIKI_SIMD_INLINE vi16	simd::set_i16( const sint16 value )												{ return _mm_set1_epi16( value ); }
	TIKI_SIMD_INLINE vi16	simd::set_i16( const sint16 a, const sint16 b, const sint16 c, const sint16 d, const sint16 e, const sint16 f, const sint16 g, const sint16 h )	{ return _mm_set_epi16( a, b, c, d, e, f, g, h ); }
	TIKI_SIMD_INLINE vi16	simd::set_i16( const sint16* pValues )											{ return _mm_load_si128( (vi16*)pValues ); }
	TIKI_SIMD_INLINE vi16	simd::set_i16( const sint16* pValues, const size_t offset )						{ return _mm_load_si128( (vi16*)(pValues + offset) ); }
	TIKI_SIMD_INLINE vi16	simd::set_i16u( const sint16* pValues )											{ return _mm_loadu_si128( (vi16*)pValues ); }
	TIKI_SIMD_INLINE vi16	simd::set_i16u( const sint16* pValues, const size_t offset )					{ return _mm_loadu_si128( (vi16*)(pValues + offset) ); }

	//////////////////////////////////////////////////////////////////////////
	// store

	TIKI_SIMD_INLINE void	simd::get_f32( float* pTarget, const vf32 value)								{ _mm_store_ps( pTarget, value ); }
	TIKI_SIMD_INLINE void	simd::get_f32( float* pTarget, const size_t offset, const vf32 value)			{ _mm_store_ps( pTarget + offset, value ); }
	TIKI_SIMD_INLINE float	simd::get_f32_x( const vf32 value )												{ return value.m128_f32[ 0u ]; }
	TIKI_SIMD_INLINE float	simd::get_f32_y( const vf32 value )												{ return value.m128_f32[ 1u ]; }
	TIKI_SIMD_INLINE float	simd::get_f32_z( const vf32 value )												{ return value.m128_f32[ 2u ]; }
	TIKI_SIMD_INLINE float	simd::get_f32_w( const vf32 value )												{ return value.m128_f32[ 3u ]; }

	TIKI_SIMD_INLINE void	simd::get_i32( sint32* pTarget, const vi32 value )								{ _mm_store_si128( (vi32*)pTarget, value ); }
	TIKI_SIMD_INLINE void	simd::get_i32( sint32* pTarget, const size_t offset, const vi32 value )			{ _mm_store_si128( (vi32*)( pTarget + offset ), value ); }
	TIKI_SIMD_INLINE sint32	simd::get_i32_x( const vi32 value )												{ return value.m128i_i32[ 0u ]; }
	TIKI_SIMD_INLINE sint32	simd::get_i32_y( const vi32 value )												{ return value.m128i_i32[ 1u ]; }
	TIKI_SIMD_INLINE sint32	simd::get_i32_z( const vi32 value )												{ return value.m128i_i32[ 2u ]; }
	TIKI_SIMD_INLINE sint32	simd::get_i32_w( const vi32 value )												{ return value.m128i_i32[ 3u ]; }


	//////////////////////////////////////////////////////////////////////////
	// arithmetic

	TIKI_SIMD_INLINE vf32	simd::add_f32( const vf32 v1, const vf32 v2 )									{ return _mm_add_ps( v1, v2 ); }
	TIKI_SIMD_INLINE vf32	simd::sub_f32( const vf32 v1, const vf32 v2 )									{ return _mm_sub_ps( v1, v2 ); }
	TIKI_SIMD_INLINE vf32	simd::mul_f32( const vf32 v1, const vf32 v2 )									{ return _mm_mul_ps( v1, v2 ); }
	TIKI_SIMD_INLINE vf32	simd::div_f32( const vf32 v1, const vf32 v2 )									{ return _mm_div_ps( v1, v2 ); }
	TIKI_SIMD_INLINE vf32	simd::muladd_f32( const vf32 v1, const vf32 v2, const vf32 v3 )					{ return _mm_add_ps( _mm_mul_ps( v1, v2 ), v3 ); } // r = v1 * v2 + v3
	TIKI_SIMD_INLINE vf32	simd::mulsub_f32( const vf32 v1, const vf32 v2, const vf32 v3 )					{ return _mm_sub_ps( _mm_mul_ps( v1, v2 ), v3 ); } // r = v1 * v2 - v3
	TIKI_SIMD_INLINE vf32	simd::negmulsub_f32( const vf32 v1, const vf32 v2, const vf32 v3 )				{ return _mm_sub_ps( v3, _mm_mul_ps( v1, v2 ) ); } // r = v3 - v1 * v1

	TIKI_SIMD_INLINE vi32	simd::add_i32( const vi32 v1, const vi32 v2 )									{ return _mm_add_epi32( v1, v2 ); }
	TIKI_SIMD_INLINE vi32	simd::sub_i32( const vi32 v1, const vi32 v2 )									{ return _mm_sub_epi32( v1, v2 ); }
	//TIKI_SIMD_INLINE vi32	simd::mul_i32( const vi32 v1, const vi32 v2 )									{ return _mm_mul_epi32( v1, v2 ); }
	//TIKI_SIMD_INLINE vi32	simd::div_i32( const vi32 v1, const vi32 v2 )									{ return _mm_div_si32( v1, v2 ); }
	//TIKI_SIMD_INLINE vi32	simd::muladd_i32( const vi32 v1, const vi32 v2, const vi32 v3 )					{ return _mm_add_epi32( _mm_mul_epi32( v1, v2 ), v3 ); }
	//TIKI_SIMD_INLINE vi32	simd::mulsub_i32( const vi32 v1, const vi32 v2, const vi32 v3 )					{ return _mm_sub_epi32( _mm_mul_epi32( v1, v2 ), v3 ); }


	//////////////////////////////////////////////////////////////////////////
	// convert

	TIKI_SIMD_INLINE vf32	simd::convert_i32_to_f32( const vi32 value )									{ return _mm_cvtepi32_ps( value ); }

	TIKI_SIMD_INLINE vi32	simd::convert_i16l_to_i32( const vi16 value )									{ return _mm_srai_epi32( _mm_unpacklo_epi16( value, value ), 16u ); }
	TIKI_SIMD_INLINE vi32	simd::convert_i16h_to_i32( const vi16 value )									{ return _mm_srai_epi32( _mm_unpackhi_epi16( value, value ), 16u ); }
	

	//////////////////////////////////////////////////////////////////////////
	// special

	TIKI_SIMD_INLINE vf32	simd::splat_x_f32( const vf32 value )											{ return _mm_shuffle_ps( value, value, _MM_SHUFFLE( 0u, 0u, 0u, 0u ) ); }
	TIKI_SIMD_INLINE vf32	simd::splat_y_f32( const vf32 value )											{ return _mm_shuffle_ps( value, value, _MM_SHUFFLE( 1u, 1u, 1u, 1u ) ); }
	TIKI_SIMD_INLINE vf32	simd::splat_z_f32( const vf32 value )											{ return _mm_shuffle_ps( value, value, _MM_SHUFFLE( 2u, 2u, 2u, 2u ) ); }
	TIKI_SIMD_INLINE vf32	simd::splat_w_f32( const vf32 value )											{ return _mm_shuffle_ps( value, value, _MM_SHUFFLE( 3u, 3u, 3u, 3u ) ); }

	TIKI_SIMD_INLINE vf32	simd::rsqrt_f32( const vf32 value )												{ return _mm_rsqrt_ps( value ); } // r = 1.0f / sqrt( value )
	TIKI_SIMD_INLINE vf32	simd::dot4_f32( const vf32 v1, const vf32 v2 )									{ const vf32 mul = mul_f32( v1, v2 ); return add_f32( mul, add_f32( _mm_shuffle_ps( mul, mul, _MM_SHUFFLE( 0u, 3u, 2u, 1u ) ), add_f32( _mm_shuffle_ps( mul, mul, _MM_SHUFFLE( 1u, 0u, 3u, 2u ) ), _mm_shuffle_ps( mul, mul, _MM_SHUFFLE( 2u, 1u, 0u, 3u ) ) ) ) ); }

}
#endif // TIKI_SIMD_WIN_INL
