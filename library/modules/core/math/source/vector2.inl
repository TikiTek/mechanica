#pragma once
#ifndef TIKI_VECTOR2_INL
#define TIKI_VECTOR2_INL

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"

#include <math.h>

namespace tiki
{
	TIKI_FORCE_INLINE Vector2::Vector2()
		: x( 0.0f ), y( 0.0f )
	{
	}

	TIKI_FORCE_INLINE Vector2::Vector2( float all )
		: x( all ), y( all )
	{
	}

	TIKI_FORCE_INLINE Vector2::Vector2( const float* data )
		: x( data[ 0u ] ), y( data[ 1u ] )
	{
	}

	TIKI_FORCE_INLINE Vector2::Vector2( float _x, float _y )
		: x( _x ), y( _y )
	{
	}
	
	TIKI_FORCE_INLINE Vector2::Vector2( const float2& vec )
		: x( vec.x ), y( vec.y )
	{
	}

	TIKI_FORCE_INLINE float& Vector2::operator[]( uint index )
	{
		TIKI_ASSERT( index >= 0u && index < TIKI_COUNT( xy ) );
		return xy[ index ];
	}

	TIKI_FORCE_INLINE float Vector2::operator[]( uint index ) const
	{
		TIKI_ASSERT( index >= 0u && index < TIKI_COUNT( xy ) );
		return xy[ index ];
	}

	TIKI_FORCE_INLINE Vector2& Vector2::operator=( const Vector2& rhs )
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	
	TIKI_FORCE_INLINE Vector2& Vector2::operator+=( const Vector2& rhs )
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	TIKI_FORCE_INLINE Vector2& Vector2::operator-=( const Vector2& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	TIKI_FORCE_INLINE Vector2 Vector2::operator-() const
	{
		return Vector2( -x, -y );
	}

	TIKI_FORCE_INLINE bool Vector2::operator==( const Vector2& vec ) const
	{
		return isEquals( vec );
	}

	TIKI_FORCE_INLINE bool Vector2::operator!=( const Vector2& vec ) const
	{
		return !isEquals( vec );
	}

	TIKI_FORCE_INLINE bool Vector2::isZero( float epsilon /*= f32::s_epsilon*/) const 
	{
		return f32::isZero( x, epsilon ) &&
			f32::isZero( y, epsilon );
	}

	TIKI_FORCE_INLINE bool Vector2::isEquals( const Vector2& other, float epsilon /*= f32::s_epsilon*/) const 
	{
		return f32::isEquals( x, other.x, epsilon ) &&
			f32::isEquals( y, other.y, epsilon );
	}

	TIKI_FORCE_INLINE void Vector2::negate()
	{
		x = -x;
		y = -y;
	}

	TIKI_FORCE_INLINE void Vector2::negate( const Vector2& vec )
	{
		x = -vec.x;
		y = -vec.y;
	}

	TIKI_FORCE_INLINE void Vector2::add( const Vector2& vec )
	{
		x += vec.x;
		y += vec.y;
	}

	TIKI_FORCE_INLINE void Vector2::add( const Vector2& vec1, const Vector2& vec2 )
	{
		x = vec1.x + vec2.x;
		y = vec1.y + vec2.y;
	}

	TIKI_FORCE_INLINE void Vector2::sub( const Vector2& vec )
	{
		x -= vec.x;
		y -= vec.y;
	}

	TIKI_FORCE_INLINE void Vector2::sub( const Vector2& vec1, const Vector2& vec2 )
	{
		x = vec1.x - vec2.x;
		y = vec1.y - vec2.y;
	}

	TIKI_FORCE_INLINE void Vector2::mul( const Vector2& vec )
	{
		x *= vec.x;
		y *= vec.y;
	}

	TIKI_FORCE_INLINE void Vector2::mul( const Vector2& vec1, const Vector2& vec2 )
	{
		x = vec1.x * vec2.x;
		y = vec1.y * vec2.y;
	}

	TIKI_FORCE_INLINE void Vector2::mul( const Vector2& vec, float scalar )
	{
		x = vec.x * scalar;
		y = vec.y * scalar;
	}

	TIKI_FORCE_INLINE void Vector2::mul( float scalar )
	{
		x *= scalar;
		y *= scalar;
	}

	TIKI_FORCE_INLINE void Vector2::div( const Vector2& vec )
	{
		x /= vec.x;
		y /= vec.y;
	}

	TIKI_FORCE_INLINE void Vector2::div( const Vector2& vec1, const Vector2& vec2 )
	{
		x = vec1.x / vec2.x;
		y = vec1.y / vec2.y;
	}

	TIKI_FORCE_INLINE void Vector2::div( const Vector2& vec, float scalar )
	{
		const float tmp = 1.0f / scalar;
		x = vec.x * tmp;
		y = vec.y * tmp;
	}

	TIKI_FORCE_INLINE void Vector2::div( float scalar )
	{
		const float tmp = 1.0f / scalar;
		x *= tmp;
		y *= tmp;
	}

	TIKI_FORCE_INLINE float Vector2::length() const
	{
		return math::sqrt( x * x + y * y );
	}

	TIKI_FORCE_INLINE float Vector2::lengthSq() const
	{
		return ( x * x + y * y );
	}

	TIKI_FORCE_INLINE void Vector2::normalize()
	{
		TIKI_ASSERT( !f32::isZero( lengthSq() ) );
		const float num = math::rsqrt( lengthSq() );
		mul( num );
	}

	TIKI_FORCE_INLINE void Vector2::normalize( const Vector2& vec )
	{
		TIKI_ASSERT( !f32::isZero( vec.lengthSq() ) );
		const float tmp = math::rsqrt( vec.lengthSq() );
		mul( vec, tmp );
	}

	TIKI_FORCE_INLINE void Vector2::truncate( float length )
	{
		TIKI_ASSERT( !f32::isZero( length ) );
		const float tmp = length * math::rsqrt( this->lengthSq() );
		mul( tmp );
	}

	TIKI_FORCE_INLINE void Vector2::truncate( const Vector2& vec, float length )
	{
		TIKI_ASSERT( !f32::isZero( length ) );
		const float tmp = length * math::rsqrt( vec.lengthSq() );
		mul( vec, tmp );
	}

	TIKI_FORCE_INLINE float Vector2::distance( const Vector2& start, const Vector2& end )
	{
		const float dx = end.x - start.x;
		const float dy = end.y - start.y;
		return sqrtf( dx * dx + dy * dy );
	}

	TIKI_FORCE_INLINE float Vector2::distanceSq( const Vector2& start, const Vector2& end )
	{
		const float dx = end.x - start.x;
		const float dy = end.y - start.y;
		return dx * dx + dy * dy;
	}

	TIKI_FORCE_INLINE float Vector2::dot( const Vector2& vec1, const Vector2& vec2 )
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	TIKI_FORCE_INLINE void Vector2::reflect( const Vector2& vec, const Vector2& normal )
	{
		const float num = vec.x * normal.x + vec.y * normal.y;
		x = vec.x - 2.0f * num * normal.x;
		y = vec.y - 2.0f * num * normal.y;
	}

	TIKI_FORCE_INLINE void Vector2::clamp( const Vector2& min, const Vector2& max )
	{
		x = f32::clamp( x, min.x, max.x );
		y = f32::clamp( y, min.y, max.y );
	}

	TIKI_FORCE_INLINE void Vector2::clamp( const Vector2& vec, const Vector2& min, const Vector2& max)
	{
		x = f32::clamp( vec.x, min.x, max.x );
		y = f32::clamp( vec.y, min.y, max.y );
	}

	TIKI_FORCE_INLINE void Vector2::lerp( const Vector2& vec1, const Vector2& vec2, float amount )
	{
		x = f32::lerp( vec1.x, vec2.x, amount );
		y = f32::lerp( vec1.y, vec2.y, amount );
	}

	TIKI_FORCE_INLINE void Vector2::smooth( const Vector2& vec1, const Vector2& vec2, float amount )
	{
		float val = f32::clamp( amount, 0.0f, 1.0f );
		val = val * val * ( 3.0f - 2.0f * val );

		x = f32::lerp( vec1.x, vec2.x, val );
		y = f32::lerp( vec1.y, vec2.y, val );
	}

	TIKI_FORCE_INLINE void Vector2::toFloat2( float2& target ) const
	{
		target.x = x;
		target.y = y;
	}
}

#endif // TIKI_VECTOR2_INL
