#pragma once
#ifndef TIKI_VECTOR3_INL
#define TIKI_VECTOR3_INL

#include <math.h>

namespace tiki
{
	TIKI_FORCE_INLINE Vector3::Vector3( void )
		:x(0.0f), y(0.0f), z(0.0f)
	{
	}

	TIKI_FORCE_INLINE Vector3::Vector3( const float all )
		:x(all), y(all), z(all)
	{
	}

	TIKI_FORCE_INLINE Vector3::Vector3( const float* data )
		:x(data[0]), y(data[1]), z(data[2])
	{
	}

	TIKI_FORCE_INLINE Vector3::Vector3( const float3& data )
		: x( data.x ), y( data.y ), z( data.z )
	{
	}

	TIKI_FORCE_INLINE Vector3::Vector3( const float nx, const float ny, const float nz )
		:x(nx), y(ny), z(nz)
	{
	}

	TIKI_FORCE_INLINE Vector3::Vector3( const Vector3& vec )
		:x(vec.x), y(vec.y), z(vec.z)
	{
	}

	TIKI_FORCE_INLINE Vector3::~Vector3( void )
	{

	}

	TIKI_FORCE_INLINE Vector3& Vector3::operator=( const Vector3& vec )
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	TIKI_FORCE_INLINE bool Vector3::operator==( const Vector3& vec ) const
	{
		return Vector3::isEquals( vec );
	}

	TIKI_FORCE_INLINE bool Vector3::operator!=( const Vector3& vec ) const
	{
		return !Vector3::isEquals( vec );
	}

	TIKI_FORCE_INLINE float& Vector3::operator[]( const int index )
	{
		TIKI_ASSERT( index >= 0 && index <= 2 );
		return arr[ index ];
	}

	TIKI_FORCE_INLINE float Vector3::operator[]( const int index ) const
	{
		TIKI_ASSERT( index >= 0 && index <= 2 );
		return arr[ index ];
	}

	TIKI_FORCE_INLINE Vector3 Vector3::operator-( void ) const
	{
		return Vector3( -x, -y, -z );
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

	TIKI_FORCE_INLINE Vector3::operator float*( void )
	{
		return arr;
	}

	TIKI_FORCE_INLINE Vector3::operator const float*( void ) const
	{
		return arr;
	}

	TIKI_FORCE_INLINE bool Vector3::isEquals( const Vector3& other, const float epsilon /*= f32::s_epsilon*/ ) const
	{
		return f32::isEquals( x, other.x, epsilon ) &&
			f32::isEquals( y, other.y, epsilon ) &&
			f32::isEquals( z, other.z, epsilon );
	}

	TIKI_FORCE_INLINE bool Vector3::isZero( const float epsilon /*= f32::s_epsilon*/ ) const
	{
		return f32::isZero( x ) &&
			f32::isZero( y ) &&
			f32::isZero( z );
	}

	TIKI_FORCE_INLINE void Vector3::negate( void )
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

	TIKI_FORCE_INLINE void Vector3::mul( const Vector3& vec, const float scalar )
	{
		x = vec.x * scalar;
		y = vec.y * scalar;
		z = vec.z * scalar;
	}

	TIKI_FORCE_INLINE void Vector3::mul( const float scalar )
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

	TIKI_FORCE_INLINE void Vector3::div( const Vector3& vec, const float scalar )
	{
		const float tmp = 1.0f / scalar;
		x = vec.x * tmp;
		y = vec.y * tmp;
		z = vec.z * tmp;
	}

	TIKI_FORCE_INLINE void Vector3::div( const float scalar )
	{
		const float tmp = 1.0f / scalar;
		x *= tmp;
		y *= tmp;
		z *= tmp;
	}

	TIKI_FORCE_INLINE float Vector3::length( void ) const
	{
		return tiki::sqrt( x * x + y * y + z * z );
	}

	TIKI_FORCE_INLINE float Vector3::lengthSq( void ) const
	{
		return x * x + y * y + z * z;
	}

	TIKI_FORCE_INLINE void Vector3::normalize( void )
	{
		TIKI_ASSERT( !f32::isZero( this->lengthSq() ) );
		const float num = tiki::rsqrt( this->lengthSq() );
		mul( num );
	}

	TIKI_FORCE_INLINE void Vector3::normalize0()
	{
		if ( f32::isZero( lengthSq() ) )
		{
			return;
		}

		const float num = tiki::rsqrt( this->lengthSq() );
		mul( num );
	}

	TIKI_FORCE_INLINE void Vector3::normalize( const Vector3& vec )
	{
		TIKI_ASSERT( !f32::isZero( vec.lengthSq() ) );
		const float num = tiki::rsqrt( vec.lengthSq() );
		mul( vec, num );
	}

	TIKI_FORCE_INLINE void Vector3::truncate( const float length )
	{
		TIKI_ASSERT( !f32::isZero( length ) );
		const float tmp = length * tiki::rsqrt( this->lengthSq() );
		mul( tmp );
	}

	TIKI_FORCE_INLINE void Vector3::truncate( const Vector3& vec, const float length )
	{
		TIKI_ASSERT( !f32::isZero( length ) );
		const float tmp = length * tiki::rsqrt( vec.lengthSq() );
		mul( vec, tmp );
	}

	TIKI_FORCE_INLINE float Vector3::distance( const Vector3& start, const Vector3& end )
	{
		const float dx = end.x - start.x;
		const float dy = end.y - start.y;
		const float dz = end.z - start.z;

		return sqrtf( dx * dx + dy * dy + dz * dz );
	}

	TIKI_FORCE_INLINE float Vector3::distanceSq( const Vector3& start, const Vector3& end )
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
		x = tiki::clamp( x, min.x, max.x );
		y = tiki::clamp( y, min.y, max.y );
		z = tiki::clamp( z, min.z, max.z );
	}

	TIKI_FORCE_INLINE void Vector3::clamp( const Vector3& vec, const Vector3& min, const Vector3& max )
	{
		x = tiki::clamp( vec.x, min.x, max.x );
		y = tiki::clamp( vec.y, min.y, max.y );
		z = tiki::clamp( vec.z, min.z, max.z );
	}

	TIKI_FORCE_INLINE void Vector3::snap( void )
	{
		x = tiki::snap( x );
		y = tiki::snap( y );
		z = tiki::snap( z );
	}

	TIKI_FORCE_INLINE void Vector3::snap( const Vector3& vec )
	{
		x = tiki::snap( vec.x );
		y = tiki::snap( vec.y );
		z = tiki::snap( vec.z );
	}

	TIKI_FORCE_INLINE void Vector3::lerp( const Vector3& vec1, const Vector3& vec2, const float amount )
	{
		x = tiki::lerp( vec1.x, vec2.x, amount );
		y = tiki::lerp( vec1.y, vec2.y, amount );
		z = tiki::lerp( vec1.z, vec2.z, amount );
	}

	TIKI_FORCE_INLINE void Vector3::smooth( const Vector3& vec1, const Vector3& vec2, const float amount )
	{
		float val = tiki::clamp( amount, 0.0f, 1.0f );
		val = val * val * ( 3.0f - 2.0f * val );

		x = tiki::lerp( vec1.x, vec2.x, val );
		y = tiki::lerp( vec1.y, vec2.y, val );
		z = tiki::lerp( vec1.z, vec2.z, val );
	}

	TIKI_FORCE_INLINE void Vector3::maxVec( const Vector3& vec )
	{
		x = TIKI_MAX ( x, vec.x );
		y = TIKI_MAX ( y, vec.y );
		z = TIKI_MAX ( z, vec.z );
	}

	TIKI_FORCE_INLINE void Vector3::minVec( const Vector3& vec )
	{
		x = TIKI_MIN ( x, vec.x );
		y = TIKI_MIN ( y, vec.y );
		z = TIKI_MIN ( z, vec.z );
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
