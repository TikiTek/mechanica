#pragma once
#ifndef TIKI_VECTOR4_INL
#define TIKI_VECTOR4_INL

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"

#include <math.h>

namespace tiki
{
	TIKI_FORCE_INLINE Vector4::Vector4()
		: x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f )
	{
	}

	TIKI_FORCE_INLINE Vector4::Vector4( float all )
		: x( all ), y( all ), z( all ), w( all )
	{
	}

	TIKI_FORCE_INLINE Vector4::Vector4( const float* data )
		: x( data[ 0u ] ), y( data[ 1u ] ), z( data[ 2u ] ), w( data[ 3u ] )
	{
	}

	TIKI_FORCE_INLINE Vector4::Vector4( float _x, float _y, float _z, float _w )
		: x( _x ), y( _y ), z( _z ), w( _w )
	{
	}

	TIKI_FORCE_INLINE Vector4::Vector4( const float4& data )
		: x( data.x ), y( data.y ), z( data.z ), w( data.w )
	{
	}

	TIKI_FORCE_INLINE float& Vector4::operator[]( uint index )
	{
		TIKI_ASSERT( index >= 0u && index < TIKI_COUNT( xyzw ) );
		return xyzw[ index ];
	}

	TIKI_FORCE_INLINE float Vector4::operator[]( uint index ) const
	{
		TIKI_ASSERT( index >= 0u && index <= TIKI_COUNT( xyzw ) );
		return xyzw[ index ];
	}

	TIKI_FORCE_INLINE Vector4& Vector4::operator=( const Vector4& vec )
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
		return *this;
	}

	TIKI_FORCE_INLINE Vector4& Vector4::operator+=( const Vector4& vec )
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;
		return *this;
	}

	TIKI_FORCE_INLINE Vector4& Vector4::operator-=( const Vector4& vec )
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
		return *this;
	}

	TIKI_FORCE_INLINE Vector4 Vector4::operator-() const
	{
		return Vector4( -x, -y, -z, -w );
	}

	TIKI_FORCE_INLINE bool Vector4::operator==( const Vector4& vec ) const
	{
		return isEquals( vec );
	}

	TIKI_FORCE_INLINE bool Vector4::operator!=( const Vector4& vec ) const
	{
		return !isEquals( vec );
	}

	TIKI_FORCE_INLINE bool Vector4::isZero( float epsilon /*= f32::s_epsilon*/ ) const 
	{
		return f32::isZero( x, epsilon ) &&
			f32::isZero( y, epsilon ) &&
			f32::isZero( z, epsilon ) &&
			f32::isZero( w, epsilon );
	}

	TIKI_FORCE_INLINE bool Vector4::isEquals( const Vector4& other, float epsilon /*= f32::s_epsilon*/ ) const
	{
		return f32::isEquals( x, other.x, epsilon ) &&
			f32::isEquals( y, other.y, epsilon ) &&
			f32::isEquals( z, other.z, epsilon ) &&
			f32::isEquals( w, other.w, epsilon );
	}

	TIKI_FORCE_INLINE void Vector4::negate()
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}

	TIKI_FORCE_INLINE void Vector4::negate( const Vector4& vec )
	{
		x = -vec.x;
		y = -vec.y;
		z = -vec.z;
		w = -vec.w;
	}

	TIKI_FORCE_INLINE void Vector4::add( const Vector4& vec )
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;
	}

	TIKI_FORCE_INLINE void Vector4::add( const Vector4& vec1, const Vector4& vec2 )
	{
		x = vec1.x + vec2.x;
		y = vec1.y + vec2.y;
		z = vec1.z + vec2.z;
		w = vec1.w + vec2.w;
	}

	TIKI_FORCE_INLINE void Vector4::sub( const Vector4& vec )
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
	}

	TIKI_FORCE_INLINE void Vector4::sub( const Vector4& vec1, const Vector4& vec2 )
	{
		x = vec1.x - vec2.x;
		y = vec1.y - vec2.y;
		z = vec1.z - vec2.z;
		w = vec1.w - vec2.w;
	}

	TIKI_FORCE_INLINE void Vector4::mul( const Vector4& vec )
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		w *= vec.w;
	}

	TIKI_FORCE_INLINE void Vector4::mul( const Vector4& vec1, const Vector4& vec2 )
	{
		x = vec1.x * vec2.x;
		y = vec1.y * vec2.y;
		z = vec1.z * vec2.z;
		w = vec1.w * vec2.w;
	}

	TIKI_FORCE_INLINE void Vector4::mul( const Vector4& vec, float scalar )
	{
		x = vec.x * scalar;
		y = vec.y * scalar;
		z = vec.z * scalar;
		w = vec.w * scalar;
	}

	TIKI_FORCE_INLINE void Vector4::mul( float scalar )
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
	}

	TIKI_FORCE_INLINE void Vector4::div( const Vector4& vec )
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
		w /= vec.w;
	}

	TIKI_FORCE_INLINE void Vector4::div( const Vector4& vec1, const Vector4& vec2 )
	{
		x = vec1.x / vec2.x;
		y = vec1.y / vec2.y;
		z = vec1.z / vec2.z;
		w = vec1.w / vec2.w;
	}

	TIKI_FORCE_INLINE void Vector4::div( const Vector4& vec, float scalar )
	{
		const float tmp = 1.0f / scalar;
		x = vec.x * tmp;
		y = vec.y * tmp;
		z = vec.z * tmp;
		w = vec.w * tmp;
	}

	TIKI_FORCE_INLINE void Vector4::div( float scalar )
	{
		const float tmp = 1.0f / scalar;
		x *= tmp;
		y *= tmp;
		z *= tmp;
		w *= tmp;
	}

	TIKI_FORCE_INLINE float Vector4::length() const
	{
		return math::sqrt( x * x + y * y + z * z + w * w );
	}

	TIKI_FORCE_INLINE float Vector4::squareLength() const
	{
		return x * x + y * y + z * z + w * w;
	}

	TIKI_FORCE_INLINE void Vector4::normalize()
	{
		TIKI_ASSERT( !f32::isZero( squareLength() ) );
		const float tmp = math::rsqrt( length() );
		mul( tmp );
	}

	TIKI_FORCE_INLINE void Vector4::normalize( const Vector4& vec )
	{
		TIKI_ASSERT( !f32::isZero( vec.squareLength() ) );
		const float tmp = math::rsqrt( vec.squareLength() );
		mul( vec, tmp );
	}

	TIKI_FORCE_INLINE void Vector4::truncate( float length )
	{
		TIKI_ASSERT( !f32::isZero( length ) );
		const float tmp = length * squareLength();
		mul( tmp );
	}

	TIKI_FORCE_INLINE void Vector4::truncate( const Vector4& vec, float length )
	{
		TIKI_ASSERT( !f32::isZero( length ) );
		const float tmp = length * vec.squareLength();
		mul( vec, tmp );
	}

	TIKI_FORCE_INLINE float Vector4::distance( const Vector4& start, const Vector4& end )
	{
		const float dx = end.x - start.x;
		const float dy = end.y - start.y;
		const float dz = end.z - start.z;
		const float dw = end.w - start.w;

		return sqrtf( dx * dx + dy * dy + dz * dz + dw * dw );
	}

	TIKI_FORCE_INLINE float Vector4::squareDistance( const Vector4& start, const Vector4& end )
	{
		const float dx = end.x - start.x;
		const float dy = end.y - start.y;
		const float dz = end.z - start.z;
		const float dw = end.w - start.w;

		return dx * dx + dy * dy + dz * dz + dw * dw;
	}

	TIKI_FORCE_INLINE float Vector4::dot( const Vector4& vec1, const Vector4& vec2 )
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
	}

	TIKI_FORCE_INLINE void Vector4::clamp( const Vector4& min, const Vector4& max )
	{
		x = f32::clamp( x, min.x, max.x );
		y = f32::clamp( y, min.y, max.y );
		z = f32::clamp( z, min.z, max.z );
	}

	TIKI_FORCE_INLINE void Vector4::clamp( const Vector4& vec, const Vector4& min, const Vector4& max )
	{
		x = f32::clamp( vec.x, min.x, max.x );
		y = f32::clamp( vec.y, min.y, max.y );
		z = f32::clamp( vec.z, min.z, max.z );
	}

	TIKI_FORCE_INLINE void Vector4::lerp( const Vector4& vec1, const Vector4& vec2, float amount )
	{
		x = f32::lerp( vec1.x, vec2.x, amount );
		y = f32::lerp( vec1.y, vec2.y, amount );
		z = f32::lerp( vec1.z, vec2.z, amount );
	}

	TIKI_FORCE_INLINE void Vector4::smooth( const Vector4& vec1, const Vector4& vec2, float amount )
	{
		float val = f32::clamp( amount, 0.0f, 1.0f );
		val = val * val * ( 3.0f - 2.0f * val );

		x = f32::lerp( vec1.x, vec2.x, val );
		y = f32::lerp( vec1.y, vec2.y, val );
		z = f32::lerp( vec1.z, vec2.z, val );
	}

	TIKI_FORCE_INLINE void Vector4::toFloat4( float4& target ) const
	{
		target.x = x;
		target.y = y;
		target.z = z;
		target.w = w;
	}
}

#endif // TIKI_VECTOR4_INL
