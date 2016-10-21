#pragma once
#ifndef __TIKI_VECTOR_HPP_INCLUDED__
#define __TIKI_VECTOR_HPP_INCLUDED__

#include "tiki/base/float32.hpp"
#include "tiki/base/types.hpp"

#include "base.hpp"

namespace tiki
{
	TIKI_ALIGN_PREFIX( 8 ) struct Vector2
	{
		float x;
		float y;

		static const Vector2 zero;
		static const Vector2 one;
		static const Vector2 negativeOne;
		static const Vector2 unitX;
		static const Vector2 unitY;
	}
	TIKI_ALIGN_POSTFIX( 8 );

	TIKI_ALIGN_PREFIX( 16 ) struct Vector3
	{
		float x;
		float y;
		float z;

		float _pad;

		static const Vector3 zero;
		static const Vector3 one;
		static const Vector3 negativeOne;
		static const Vector3 unitX;
		static const Vector3 unitY;
		static const Vector3 unitZ;
	}
	TIKI_ALIGN_POSTFIX( 16 );

	TIKI_ALIGN_PREFIX( 16 ) struct Vector4
	{
		float x;
		float y;
		float z;
		float w;

		static const Vector4 zero;
		static const Vector4 one;
		static const Vector4 negativeOne;
		static const Vector4 unitX;
		static const Vector4 unitY;
		static const Vector4 unitZ;
		static const Vector4 unitW;
	}
	TIKI_ALIGN_POSTFIX( 16 );

	namespace vector
	{
		TIKI_FORCE_INLINE bool		isEquals( const Vector2& lhs, const Vector2& rhs, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool		isEquals( const Vector3& lhs, const Vector3& rhs, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool		isEquals( const Vector4& lhs, const Vector4& rhs, float epsilon = f32::epsilon );

		TIKI_FORCE_INLINE bool		isZero( const Vector2& vec, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool		isZero( const Vector3& vec, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool		isZero( const Vector4& vec, float epsilon = f32::epsilon );

		TIKI_FORCE_INLINE bool		isNormalized( const Vector2& vec, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool		isNormalized( const Vector3& vec, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool		isNormalized( const Vector4& vec, float epsilon = f32::epsilon );

		TIKI_FORCE_INLINE Vector2	create( float x, float y );
		TIKI_FORCE_INLINE Vector3	create( float x, float y, float z );
		TIKI_FORCE_INLINE Vector4	create( float x, float y, float z, float w );
		TIKI_FORCE_INLINE Vector2	create( const float2& source );
		TIKI_FORCE_INLINE Vector3	create( const float3& source );
		TIKI_FORCE_INLINE Vector4	create( const float4& source );

		TIKI_FORCE_INLINE void		toFloat( float2& target, const Vector2& source );
		TIKI_FORCE_INLINE void		toFloat( float3& target, const Vector3& source );
		TIKI_FORCE_INLINE void		toFloat( float3& target, const Vector2& source, float z );
		TIKI_FORCE_INLINE void		toFloat( float4& target, const Vector4& source );
		TIKI_FORCE_INLINE void		toFloat( float4& target, const Vector3& source, float w );
		TIKI_FORCE_INLINE void		toFloat( float4& target, const Vector2& source, float z, float w );

		TIKI_FORCE_INLINE Vector2&	clear( Vector2& vec );
		TIKI_FORCE_INLINE Vector3&	clear( Vector3& vec );
		TIKI_FORCE_INLINE Vector4&	clear( Vector4& vec );

		TIKI_FORCE_INLINE Vector2&	set( Vector2& vec, const float2& source );
		TIKI_FORCE_INLINE Vector2&	set( Vector2& vec, float x, float y );
		TIKI_FORCE_INLINE Vector3&	set( Vector3& vec, const float3& source );
		TIKI_FORCE_INLINE Vector3&	set( Vector3& vec, const Vector2& source, float z );
		TIKI_FORCE_INLINE Vector3&	set( Vector3& vec, float x, float y, float z );
		TIKI_FORCE_INLINE Vector4&	set( Vector4& vec, const float4& source );
		TIKI_FORCE_INLINE Vector4&	set( Vector4& vec, const Vector3& source, float w );
		TIKI_FORCE_INLINE Vector4&	set( Vector4& vec, float x, float y, float z, float w );

		TIKI_FORCE_INLINE Vector2&	add( Vector2& vec, const Vector2& rhs );
		TIKI_FORCE_INLINE Vector3&	add( Vector3& vec, const Vector3& rhs );
		TIKI_FORCE_INLINE Vector4&	add( Vector4& vec, const Vector4& rhs );
		TIKI_FORCE_INLINE Vector2&	add( Vector2& vec, const Vector2& lhs, const Vector2& rhs );
		TIKI_FORCE_INLINE Vector3&	add( Vector3& vec, const Vector3& lhs, const Vector3& rhs );
		TIKI_FORCE_INLINE Vector4&	add( Vector4& vec, const Vector4& lhs, const Vector4& rhs );

		TIKI_FORCE_INLINE Vector2&	sub( Vector2& vec, const Vector2& rhs );
		TIKI_FORCE_INLINE Vector3&	sub( Vector3& vec, const Vector3& rhs );
		TIKI_FORCE_INLINE Vector4&	sub( Vector4& vec, const Vector4& rhs );
		TIKI_FORCE_INLINE Vector2&	sub( Vector2& vec, const Vector2& lhs, const Vector2& rhs );
		TIKI_FORCE_INLINE Vector3&	sub( Vector3& vec, const Vector3& lhs, const Vector3& rhs );
		TIKI_FORCE_INLINE Vector4&	sub( Vector4& vec, const Vector4& lhs, const Vector4& rhs );

		TIKI_FORCE_INLINE Vector2&	mul( Vector2& vec, const Vector2& rhs );
		TIKI_FORCE_INLINE Vector3&	mul( Vector3& vec, const Vector3& rhs );
		TIKI_FORCE_INLINE Vector4&	mul( Vector4& vec, const Vector4& rhs );
		TIKI_FORCE_INLINE Vector2&	mul( Vector2& vec, const Vector2& lhs, const Vector2& rhs );
		TIKI_FORCE_INLINE Vector3&	mul( Vector3& vec, const Vector3& lhs, const Vector3& rhs );
		TIKI_FORCE_INLINE Vector4&	mul( Vector4& vec, const Vector4& lhs, const Vector4& rhs );

		TIKI_FORCE_INLINE Vector2&	div( Vector2& vec, const Vector2& rhs );
		TIKI_FORCE_INLINE Vector3&	div( Vector3& vec, const Vector3& rhs );
		TIKI_FORCE_INLINE Vector4&	div( Vector4& vec, const Vector4& rhs );
		TIKI_FORCE_INLINE Vector2&	div( Vector2& vec, const Vector2& lhs, const Vector2& rhs );
		TIKI_FORCE_INLINE Vector3&	div( Vector3& vec, const Vector3& lhs, const Vector3& rhs );
		TIKI_FORCE_INLINE Vector4&	div( Vector4& vec, const Vector4& lhs, const Vector4& rhs );

		TIKI_FORCE_INLINE Vector2&	scale( Vector2& vec, float val );
		TIKI_FORCE_INLINE Vector3&	scale( Vector3& vec, float val );
		TIKI_FORCE_INLINE Vector4&	scale( Vector4& vec, float val );

		TIKI_FORCE_INLINE Vector2&	negate( Vector2& vec );
		TIKI_FORCE_INLINE Vector3&	negate( Vector3& vec );
		TIKI_FORCE_INLINE Vector4&	negate( Vector4& vec );

		TIKI_FORCE_INLINE float		length( const Vector2& vec );
		TIKI_FORCE_INLINE float		length( const Vector3& vec );
		TIKI_FORCE_INLINE float		length( const Vector4& vec );
		TIKI_FORCE_INLINE float		lengthSquared( const Vector2& vec );
		TIKI_FORCE_INLINE float		lengthSquared( const Vector3& vec );
		TIKI_FORCE_INLINE float		lengthSquared( const Vector4& vec );

		TIKI_FORCE_INLINE Vector2&	normalize( Vector2& vec );
		TIKI_FORCE_INLINE Vector3&	normalize( Vector3& vec );
		TIKI_FORCE_INLINE Vector4&	normalize( Vector4& vec );
		TIKI_FORCE_INLINE Vector2&	normalizeZero( Vector2& vec );
		TIKI_FORCE_INLINE Vector3&	normalizeZero( Vector3& vec );
		TIKI_FORCE_INLINE Vector4&	normalizeZero( Vector4& vec );

		TIKI_FORCE_INLINE Vector2&	truncate( Vector2& vec, float len );
		TIKI_FORCE_INLINE Vector3&	truncate( Vector3& vec, float len );
		TIKI_FORCE_INLINE Vector4&	truncate( Vector4& vec, float len );

		TIKI_FORCE_INLINE float		distance( const Vector2& start, const Vector2& end );
		TIKI_FORCE_INLINE float		distance( const Vector3& start, const Vector3& end );
		TIKI_FORCE_INLINE float		distance( const Vector4& start, const Vector4& end );
		TIKI_FORCE_INLINE float		distanceSquared( const Vector2& start, const Vector2& end );
		TIKI_FORCE_INLINE float		distanceSquared( const Vector3& start, const Vector3& end );
		TIKI_FORCE_INLINE float		distanceSquared( const Vector4& start, const Vector4& end );

		TIKI_FORCE_INLINE float		dot( const Vector2& lhs, const Vector2& rhs );
		TIKI_FORCE_INLINE float		dot( const Vector3& lhs, const Vector3& rhs );
		TIKI_FORCE_INLINE float		dot( const Vector4& lhs, const Vector4& rhs );

		TIKI_FORCE_INLINE Vector3&	cross( Vector3& vec, const Vector3& lhs, const Vector3& rhs );

		TIKI_FORCE_INLINE Vector2&	reflect( Vector2& vec, const Vector2& source, const Vector2& normal );
		TIKI_FORCE_INLINE Vector3&	reflect( Vector3& vec, const Vector3& source, const Vector3& normal );
		TIKI_FORCE_INLINE Vector4&	reflect( Vector4& vec, const Vector4& source, const Vector4& normal );

		TIKI_FORCE_INLINE Vector2&	clamp( Vector2& vec, const Vector2& min, const Vector2& max );
		TIKI_FORCE_INLINE Vector3&	clamp( Vector3& vec, const Vector3& min, const Vector3& max );
		TIKI_FORCE_INLINE Vector4&	clamp( Vector4& vec, const Vector4& min, const Vector4& max );

		TIKI_FORCE_INLINE Vector2&	floor( Vector2& vec );
		TIKI_FORCE_INLINE Vector3&	floor( Vector3& vec );
		TIKI_FORCE_INLINE Vector4&	floor( Vector4& vec );
		TIKI_FORCE_INLINE Vector2&	ceil( Vector2& vec );
		TIKI_FORCE_INLINE Vector3&	ceil( Vector3& vec );
		TIKI_FORCE_INLINE Vector4&	ceil( Vector4& vec );

		TIKI_FORCE_INLINE Vector2&	lerp( Vector2& vec, const Vector2& lhs, const Vector2& rhs, float amount );
		TIKI_FORCE_INLINE Vector3&	lerp( Vector3& vec, const Vector3& lhs, const Vector3& rhs, float amount );
		TIKI_FORCE_INLINE Vector4&	lerp( Vector4& vec, const Vector4& lhs, const Vector4& rhs, float amount );
		TIKI_FORCE_INLINE Vector2&	smooth( Vector2& vec, const Vector2& lhs, const Vector2& rhs, float amount );
		TIKI_FORCE_INLINE Vector3&	smooth( Vector3& vec, const Vector3& lhs, const Vector3& rhs, float amount );
		TIKI_FORCE_INLINE Vector4&	smooth( Vector4& vec, const Vector4& lhs, const Vector4& rhs, float amount );

		TIKI_FORCE_INLINE Vector2&	minimum( Vector2& vec, const Vector2& lhs, const Vector2& rhs );
		TIKI_FORCE_INLINE Vector3&	minimum( Vector3& vec, const Vector3& lhs, const Vector3& rhs );
		TIKI_FORCE_INLINE Vector4&	minimum( Vector4& vec, const Vector4& lhs, const Vector4& rhs );
		TIKI_FORCE_INLINE Vector2&	maximum( Vector2& vec, const Vector2& lhs, const Vector2& rhs );
		TIKI_FORCE_INLINE Vector3&	maximum( Vector3& vec, const Vector3& lhs, const Vector3& rhs );
		TIKI_FORCE_INLINE Vector4&	maximum( Vector4& vec, const Vector4& lhs, const Vector4& rhs );
	}
}

#include "../../../source/vector.inl"

#endif // __TIKI_VECTOR_HPP_INCLUDED__
