#pragma once
#ifndef TIKI_VECTOR2_HPP
#define TIKI_VECTOR2_HPP

#include "tiki/base/alignment.hpp"
#include "tiki/base/float32.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/math/functions.hpp"

namespace tiki
{
	TIKI_PRE_ALIGN( 16 ) struct Vector2
	{
	public:

		union
		{
			struct  
			{
				float x, y;
			};
			float xy[ 2u ];
		};
		float pad0;
		float pad1;

		static const Vector2			zero;
		static const Vector2			one;
		static const Vector2			unitX;
		static const Vector2			unitY;

		TIKI_FORCE_INLINE				Vector2();
		TIKI_FORCE_INLINE				Vector2( float all );
		TIKI_FORCE_INLINE				Vector2( const float* data );
		TIKI_FORCE_INLINE				Vector2( float _x, float _y );
		TIKI_FORCE_INLINE				Vector2( const float2& vec );

		TIKI_FORCE_INLINE float&		operator[]( uint index );
		TIKI_FORCE_INLINE float			operator[]( uint index ) const;
		TIKI_FORCE_INLINE Vector2&		operator=( const Vector2& rhs );
		TIKI_FORCE_INLINE Vector2&		operator+=( const Vector2& rhs );
		TIKI_FORCE_INLINE Vector2&		operator-=( const Vector2& rhs );
		TIKI_FORCE_INLINE Vector2		operator-() const;

		TIKI_FORCE_INLINE bool			operator==( const Vector2& vec ) const;
		TIKI_FORCE_INLINE bool			operator!=( const Vector2& vec ) const;

		TIKI_FORCE_INLINE bool			isZero( float epsilon = f32::s_epsilon ) const;
		TIKI_FORCE_INLINE bool			isEquals( const Vector2& other, float epsilon = f32::s_epsilon ) const;

		TIKI_FORCE_INLINE void			negate();
		TIKI_FORCE_INLINE void			negate( const Vector2& vec );

		TIKI_FORCE_INLINE void			add( const Vector2& vec );
		TIKI_FORCE_INLINE void			add( const Vector2& vec1, const Vector2& vec2 );

		TIKI_FORCE_INLINE void			sub( const Vector2& vec );
		TIKI_FORCE_INLINE void			sub( const Vector2& vec1, const Vector2& vec2 );

		TIKI_FORCE_INLINE void			mul( const Vector2& vec );
		TIKI_FORCE_INLINE void			mul( const Vector2& vec1, const Vector2& vec2 );
		TIKI_FORCE_INLINE void			mul( const Vector2& vec, float scalar );
		TIKI_FORCE_INLINE void			mul( float scalar );

		TIKI_FORCE_INLINE void			div( const Vector2& vec );
		TIKI_FORCE_INLINE void			div( const Vector2& vec1, const Vector2& vec2 );
		TIKI_FORCE_INLINE void			div( const Vector2& vec, float scalar );	
		TIKI_FORCE_INLINE void			div( float scalar );

		TIKI_FORCE_INLINE float			length() const;
		TIKI_FORCE_INLINE float			lengthSq() const;

		TIKI_FORCE_INLINE void			normalize();
		TIKI_FORCE_INLINE void			normalize( const Vector2& vec );

		TIKI_FORCE_INLINE void			truncate( float length );
		TIKI_FORCE_INLINE void			truncate( const Vector2& vec, float length );

		TIKI_FORCE_INLINE static float	distance( const Vector2& start, const Vector2& end );
		TIKI_FORCE_INLINE static float	distanceSq( const Vector2& start, const Vector2& end );
		TIKI_FORCE_INLINE static float	dot( const Vector2& vec1, const Vector2& vec2 );

		TIKI_FORCE_INLINE void			reflect( const Vector2& vec, const Vector2& normal );
		TIKI_FORCE_INLINE void			clamp( const Vector2& min, const Vector2& max );
		TIKI_FORCE_INLINE void			clamp( const Vector2& vec, const Vector2& min, const Vector2& max);
		TIKI_FORCE_INLINE void			lerp( const Vector2& vec1, const Vector2& vec2, float amount );
		TIKI_FORCE_INLINE void			smooth( const Vector2& vec1, const Vector2& vec2, float amount );

		TIKI_FORCE_INLINE void			toFloat2( float2& target ) const;

	} TIKI_POST_ALIGN( 16 );
}

#include "../../../source/vector2.inl"

#endif // TIKI_VECTOR2_HPP
