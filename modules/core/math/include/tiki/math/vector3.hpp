#ifndef TIKI_VECTOR3_HPP__
#define TIKI_VECTOR3_HPP__

#include "tiki/base/alignment.hpp"
#include "tiki/base/float32.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/math/functions.hpp"

namespace tiki
{
	TIKI_PRE_ALIGN( 16 ) struct Vector3
	{
		union
		{
			struct  
			{
				float x, y, z;
			};
			float xyz[ 3u ];
		};
		float pad;

		static const Vector3			backward;
		static const Vector3			down;
		static const Vector3			forward;
		static const Vector3			left;
		static const Vector3			one;
		static const Vector3			right;
		static const Vector3			unitX;
		static const Vector3			unitY;
		static const Vector3			unitZ;
		static const Vector3			up;
		static const Vector3			zero;

		TIKI_FORCE_INLINE				Vector3();
		TIKI_FORCE_INLINE				Vector3( float all );
		TIKI_FORCE_INLINE				Vector3( const float* data );
		TIKI_FORCE_INLINE				Vector3( float _x, float _y, float _z );
		TIKI_FORCE_INLINE				Vector3( const float3& data );

		TIKI_FORCE_INLINE float&		operator[]( uint index );
		TIKI_FORCE_INLINE float			operator[]( uint index ) const;
		TIKI_FORCE_INLINE Vector3&		operator=( const Vector3& vec );
		TIKI_FORCE_INLINE Vector3&		operator+=( const Vector3& rhs );
		TIKI_FORCE_INLINE Vector3&		operator-=( const Vector3& rhs );
		TIKI_FORCE_INLINE Vector3		operator-() const;

		TIKI_FORCE_INLINE bool			operator==( const Vector3& vec ) const;
		TIKI_FORCE_INLINE bool			operator!=( const Vector3& vec ) const;

		TIKI_FORCE_INLINE bool			isZero( float epsilon = f32::s_epsilon ) const;
		TIKI_FORCE_INLINE bool			isEquals( const Vector3& other, float epsilon = f32::s_epsilon ) const;

		TIKI_FORCE_INLINE void			negate();
		TIKI_FORCE_INLINE void			negate( const Vector3& vec );

		TIKI_FORCE_INLINE void			add( const Vector3& vec );
		TIKI_FORCE_INLINE void			add( const Vector3& vec1, const Vector3& vec2 );

		TIKI_FORCE_INLINE void			sub( const Vector3& vec );
		TIKI_FORCE_INLINE void			sub( const Vector3& vec1, const Vector3& vec2 );

		TIKI_FORCE_INLINE void			mul( const Vector3& vec );
		TIKI_FORCE_INLINE void			mul( const Vector3& vec1, const Vector3& vec2 );
		TIKI_FORCE_INLINE void			mul( const Vector3& vec, float scalar );
		TIKI_FORCE_INLINE void			mul( const float scalar );

		TIKI_FORCE_INLINE void			div( const Vector3& vec );
		TIKI_FORCE_INLINE void			div( const Vector3& vec1, const Vector3& vec2 );
		TIKI_FORCE_INLINE void			div( const Vector3& vec, float scalar );
		TIKI_FORCE_INLINE void			div( float scalar );

		TIKI_FORCE_INLINE float			length() const;
		TIKI_FORCE_INLINE float			squareLength() const;

		TIKI_FORCE_INLINE void			normalize();
		TIKI_FORCE_INLINE void			normalize0();
		TIKI_FORCE_INLINE void			normalize( const Vector3& vec );

		TIKI_FORCE_INLINE void			truncate( float length );
		TIKI_FORCE_INLINE void			truncate( const Vector3& vec, float length );

		TIKI_FORCE_INLINE static float	distance( const Vector3& start, const Vector3& end );
		TIKI_FORCE_INLINE static float	squareDistance( const Vector3& start, const Vector3& end );
		TIKI_FORCE_INLINE static float	dot( const Vector3& vec1, const Vector3& vec2 );

		TIKI_FORCE_INLINE void			cross( const Vector3& vec1, const Vector3& vec2 );
		TIKI_FORCE_INLINE void			reflect( const Vector3& vec, const Vector3& normal );
		TIKI_FORCE_INLINE void			clamp( const Vector3& min, const Vector3& max );
		TIKI_FORCE_INLINE void			clamp( const Vector3& vec, const Vector3& min, const Vector3& max );
		TIKI_FORCE_INLINE void			lerp( const Vector3& vec1, const Vector3& vec2, float amount );
		TIKI_FORCE_INLINE void			smooth( const Vector3& vec1, const Vector3& vec2, float amount );

		TIKI_FORCE_INLINE void			max( const Vector3& vec );
		TIKI_FORCE_INLINE void			min( const Vector3& vec );

		TIKI_FORCE_INLINE void			toFloat3( float3& target ) const;
		TIKI_FORCE_INLINE void			toFloat4( float4& target ) const;

	} TIKI_POST_ALIGN(16);
}

#include "../../../source/vector3.inl"

#endif // TIKI_VECTOR3_HPP__
