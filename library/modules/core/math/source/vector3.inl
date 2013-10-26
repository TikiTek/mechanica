#pragma once
#ifndef TIKI_VECTOR3_INL
#define TIKI_VECTOR3_INL

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"

#include <math.h>

namespace tiki
{
	TIKI_FORCE_INLINE Vector3::Vector3()
		:x( 0.0f ), y( 0.0f ), z( 0.0f )
	{
	}

	TIKI_FORCE_INLINE Vector3::Vector3( float all )
		: x( all ), y( all ), z( all )
	{
	}

	TIKI_FORCE_INLINE Vector3::Vector3( const float* data )
		: x( data[ 0u ] ), y( data[ 1u ] ), z( data[ 2u ] )
	{
	}

	TIKI_FORCE_INLINE Vector3::Vector3( float _x, float _y, float _z )
		: x( _x ), y( _y ), z( _z )
	{
	}

	TIKI_FORCE_INLINE Vector3::Vector3( const float3& data )
		: x( data.x ), y( data.y ), z( data.z )
	{
	}

	TIKI_FORCE_INLINE float& Vector3::operator[]( uint index )
	{
		TIKI_ASSERT( index >= 0u && index < TIKI_COUNT( xyz ) );
		return xyz[ index ];
	}

	TIKI_FORCE_INLINE float Vector3::operator[]( uint index ) const
	{
		TIKI_ASSERT( index >= 0u && index < TIKI_COUNT( xyz ) );
		return xyz[ index ];
	}

	TIKI_FORCE_INLINE Vector3& Vector3::operator=( const Vector3& vec )
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	TIKI_FORCE_INLINE Vector3& Vector3::operator+=( const Vector3& vec )
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	TIKI_FORCE_INLINE Vector3& Vector3::operator-=( const Vector3& vec )
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	TIKI_FORCE_INLINE Vector3 Vector3::operator-() const
	{
		return Vector3( -x, -y, -z );
	}

	TIKI_FORCE_INLINE bool Vector3::operator==( const Vector3& vec ) const
	{
		return Vector3::isEquals( vec );
	}

	TIKI_FORCE_INLINE bool Vector3::operator!=( const Vector3& vec ) const
	{
		return !Vector3::isEquals( vec );
	}

	TIKI_FORCE_INLINE bool Vector3::isEquals( const Vector3& other, float epsilon /*= f32::s_epsilon*/ ) const
	{
		return f32::isEquals( x, other.x, epsilon ) &&
			f32::isEquals( y, other.y, epsilon ) &&
			f32::isEquals( z, other.z, epsilon );
	}

	TIKI_FORCE_INLINE bool Vector3::isZero( float epsilon /*= f32::s_epsilon*/ ) const
	{
		return f32::isZero( x ) &&
			f32::isZero( y ) &&
			f32::isZero( z );
	}

	TIKI_FORCE_INLINE void Vector3::negate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	TIKI_FORCE_INLINE void Vector3::negate( const Vector3& vec )
	{
		x = -vec.x;
		y = -vec.y;
		z = -vec.z;
	}

	TIKI_FORCE_INLINE void Vector3::add( const Vector3& vec )
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	TIKI_FORCE_INLINE void Vector3::add( const Vector3& vec1, const Vector3& vec2 )
	{
		x = vec1.x + vec2.x;
		y = vec1.y + vec2.y;
		z = vec1.z + vec2.z;
	}

	TIKI_FORCE_INLINE void Vector3::sub( const Vector3& vec )
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	TIKI_FORCE_INLINE void Vector3::sub( const Vector3& vec1, const Vector3& vec2 )
	{
		x = vec1.x - vec2.x;
		y = vec1.y - vec2.y;
		z = vec1.z - vec2.z;
	}

	TIKI_FORCE_INLINE void Vector3::mul( const Vector3& vec )
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
	}

	TIKI_FORCE_INLINE void Vector3::mul( const Vector3& vec1, const Vector3& vec2 )
	{
		x = vec1.x * vec2.x;
		y = vec1.y * vec2.y;
		z = vec1.z * vec2.z;
	}

	TIKI_FORCE_INLINE void Vector3::mul( const Vector3& vec, float scalar )
	{
		x = vec.x * scalar;
		y = vec.y * scalar;
		z = vec.z * scalar;
	}

	TIKI_FORCE_INLINE void Vector3::mul( float scalar )
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	TIKI_FORCE_INLINE void Vector3::div( const Vector3& vec )
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
	}

	TIKI_FORCE_INLINE void Vector3::div( const Vector3& vec1, const Vector3& vec2 )
	{
		x = vec1.x / vec2.x;
		y = vec1.y / vec2.y;
		z = vec1.z / vec2.z;
	}

	TIKI_FORCE_INLINE void Vector3::div( const Vector3& vec, float scalar )
	{
		const float tmp = 1.0f / scalar;
		x = vec.x * tmp;
		y = vec.y * tmp;
		z = vec.z * tmp;
	}

	TIKI_FORCE_INLINE void Vector3::div( float scalar )
	{
		const float tmp = 1.0f / scalar;
		x *= tmp;
		y *= tmp;
		z *= tmp;
	}

	TIKI_FORCE_INLINE float Vector3::length() const
	{
		return math::sqrt( x * x + y * y + z * z );
	}

	TIKI_FORCE_INLINE float Vector3::squareLength() const
	{
		return x * x + y * y + z * z;
	}

	TIKI_FORCE_INLINE void Vector3::normalize()
	{
		TIKI_ASSERT( !f32::isZero( squareLength() ) );
		const float num = math::rsqrt( squareLength() );
		mul( num );
	}

	TIKI_FORCE_INLINE void Vector3::normalize( const Vector3& vec )
	{
		TIKI_ASSERT( !f32::isZero( vec.squareLength() ) );
		const float num = math::rsqrt( vec.squareLength() );
		mul( vec, num );
	}

	TIKI_FORCE_INLINE void Vector3::normalize0()
	{
		if ( f32::isZero( squareLength() ) )
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			return;
		}

		const float num = math::rsqrt( squareLength() );
		mul( num );
	}

	TIKI_FORCE_INLINE void Vector3::truncate( float length )
	{
		TIKI_ASSERT( !f32::isZero( length ) );
		const float tmp = length * math::rsqrt( squareLength() );
		mul( tmp );
	}

	TIKI_FORCE_INLINE void Vector3::truncate( const Vector3& vec, float length )
	{
		TIKI_ASSERT( !f32::isZero( length ) );
		const float tmp = length * math::rsqrt( vec.squareLength() );
		mul( vec, tmp );
	}

	TIKI_FORCE_INLINE float Vector3::distance( const Vector3& start, const Vector3& end )
	{
		const float dx = end.x - start.x;
		const float dy = end.y - start.y;
		const float dz = end.z - start.z;

		return sqrtf( dx * dx + dy * dy + dz * dz );
	}

	TIKI_FORCE_INLINE float Vector3::squareDistance( const Vector3& start, const Vector3& end )
	{
		const float dx = end.x - start.x;
		const float dy = end.y - start.y;
		const float dz = end.z - start.z;

		return dx * dx + dy * dy + dz * dz;
	}

	TIKI_FORCE_INLINE float Vector3::dot( const Vector3& vec1, const Vector3& vec2 )
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}

	TIKI_FORCE_INLINE void Vector3::cross( const Vector3& vec1, const Vector3& vec2 )
	{
		x = (vec1.y * vec2.z) - (vec1.z * vec2.y);
		y = (vec1.z * vec2.x) - (vec1.x * vec2.z);
		z = (vec1.x * vec2.y) - (vec1.y * vec2.x);
	}

	TIKI_FORCE_INLINE void Vector3::reflect( const Vector3& vec, const Vector3& normal )
	{
		const float num = vec.x * normal.x + vec.y * normal.y + vec.z * normal.z;
		x = vec.x - 2.0f * num * normal.x;
		y = vec.y - 2.0f * num * normal.y;
		z = vec.z - 2.0f * num * normal.z;
	}

	TIKI_FORCE_INLINE void Vector3::clamp( const Vector3& min, const Vector3& max )
	{
		x = f32::clamp( x, min.x, max.x );
		y = f32::clamp( y, min.y, max.y );
		z = f32::clamp( z, min.z, max.z );
	}

	TIKI_FORCE_INLINE void Vector3::clamp( const Vector3& vec, const Vector3& min, const Vector3& max )
	{
		x = f32::clamp( vec.x, min.x, max.x );
		y = f32::clamp( vec.y, min.y, max.y );
		z = f32::clamp( vec.z, min.z, max.z );
	}

	TIKI_FORCE_INLINE void Vector3::lerp( const Vector3& vec1, const Vector3& vec2, float amount )
	{
		x = f32::lerp( vec1.x, vec2.x, amount );
		y = f32::lerp( vec1.y, vec2.y, amount );
		z = f32::lerp( vec1.z, vec2.z, amount );
	}

	TIKI_FORCE_INLINE void Vector3::smooth( const Vector3& vec1, const Vector3& vec2, float amount )
	{
		float val = f32::clamp( amount, 0.0f, 1.0f );
		val = val * val * ( 3.0f - 2.0f * val );

		x = f32::lerp( vec1.x, vec2.x, val );
		y = f32::lerp( vec1.y, vec2.y, val );
		z = f32::lerp( vec1.z, vec2.z, val );
	}

	TIKI_FORCE_INLINE void Vector3::max( const Vector3& vec )
	{
		x = TIKI_MAX( x, vec.x );
		y = TIKI_MAX( y, vec.y );
		z = TIKI_MAX( z, vec.z );
	}

	TIKI_FORCE_INLINE void Vector3::min( const Vector3& vec )
	{
		x = TIKI_MIN( x, vec.x );
		y = TIKI_MIN( y, vec.y );
		z = TIKI_MIN( z, vec.z );
	}

	TIKI_FORCE_INLINE void Vector3::toFloat3( float3& target ) const
	{
		target.x = x;
		target.y = y;
		target.z = z;
	}

	TIKI_FORCE_INLINE void Vector3::toFloat4( float4& target ) const
	{
		target.x = x;
		target.y = y;
		target.z = z;
		target.w = 1;
	}
}

#endif // TIKI_VECTOR3_INL
