#ifndef TIKI_VECTOR4_HPP__
#define TIKI_VECTOR4_HPP__

#include "tiki/base/alignment.hpp"
#include "tiki/base/float32.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/math/functions.hpp"

namespace tiki
{
	TIKI_PRE_ALIGN( 16 ) struct Vector4
	{
		union
		{
			struct
			{
				float x, y, z, w;
			};
			float xyzw[ 4u ];
		};

		static const Vector4			zero;
		static const Vector4			one;
		static const Vector4			unitX;
		static const Vector4			unitY;
		static const Vector4			unitZ;
		static const Vector4			unitW;

		TIKI_FORCE_INLINE				Vector4();
		TIKI_FORCE_INLINE				Vector4( float all );
		TIKI_FORCE_INLINE				Vector4( const float* data );
		TIKI_FORCE_INLINE				Vector4( float _x, float _y, float _z, float _w );
		TIKI_FORCE_INLINE				Vector4( const float4& data );

		TIKI_FORCE_INLINE float&		operator[]( uint index );
		TIKI_FORCE_INLINE float			operator[]( uint index ) const;
		TIKI_FORCE_INLINE Vector4&		operator=( const Vector4& vec );
		TIKI_FORCE_INLINE Vector4&		operator+=( const Vector4& rhs );
		TIKI_FORCE_INLINE Vector4&		operator-=( const Vector4& rhs );
		TIKI_FORCE_INLINE Vector4		operator-() const;

		TIKI_FORCE_INLINE bool			operator==( const Vector4& vec ) const;
		TIKI_FORCE_INLINE bool			operator!=( const Vector4& vec ) const;

		TIKI_FORCE_INLINE bool			isZero( float epsilon = f32::s_epsilon ) const;
		TIKI_FORCE_INLINE bool			isEquals( const Vector4& other, float epsilon = f32::s_epsilon ) const;

		TIKI_FORCE_INLINE void			negate();
		TIKI_FORCE_INLINE void			negate( const Vector4& vec );

		TIKI_FORCE_INLINE void			add( const Vector4& vec );
		TIKI_FORCE_INLINE void			add( const Vector4& vec1, const Vector4& vec2 );

		TIKI_FORCE_INLINE void			sub( const Vector4& vec );
		TIKI_FORCE_INLINE void			sub( const Vector4& vec1, const Vector4& vec2 );

		TIKI_FORCE_INLINE void			mul( const Vector4& vec );
		TIKI_FORCE_INLINE void			mul( const Vector4& vec1, const Vector4& vec2 );
		TIKI_FORCE_INLINE void			mul( const Vector4& vec, float scalar );
		TIKI_FORCE_INLINE void			mul( float scalar );

		TIKI_FORCE_INLINE void			div( const Vector4& vec );
		TIKI_FORCE_INLINE void			div( const Vector4& vec1, const Vector4& vec2 );
		TIKI_FORCE_INLINE void			div( const Vector4& vec, float scalar );
		TIKI_FORCE_INLINE void			div( float scalar );

		TIKI_FORCE_INLINE float			length() const;
		TIKI_FORCE_INLINE float			squareLength() const;

		TIKI_FORCE_INLINE void			normalize();
		TIKI_FORCE_INLINE void			normalize( const Vector4& vec );

		TIKI_FORCE_INLINE void			truncate( float length );
		TIKI_FORCE_INLINE void			truncate( const Vector4& vec, float length );

		TIKI_FORCE_INLINE static float	distance( const Vector4& start, const Vector4& end );
		TIKI_FORCE_INLINE static float	squareDistance( const Vector4& start, const Vector4& end );
		TIKI_FORCE_INLINE static float	dot( const Vector4& vec1, const Vector4& vec2 );

		TIKI_FORCE_INLINE void			clamp( const Vector4& min, const Vector4& max );
		TIKI_FORCE_INLINE void			clamp( const Vector4& vec, const Vector4& min, const Vector4& max );
		TIKI_FORCE_INLINE void			lerp( const Vector4& vec1, const Vector4& vec2, float amount );
		TIKI_FORCE_INLINE void			smooth( const Vector4& vec1, const Vector4& vec2, float amount );

		TIKI_FORCE_INLINE void			toFloat4( float4& target ) const;

	} TIKI_POST_ALIGN( 16 );
}

#include "../../../source/vector4.inl"

#endif // TIKI_VECTOR4_HPP__
