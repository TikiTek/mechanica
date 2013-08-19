#pragma once
#ifndef TIKI_VECTOR2_INL
#define TIKI_VECTOR2_INL

#include <math.h>

namespace tiki
{
	TIKI_FORCE_INLINE Vector2::Vector2( void )
		:x(0), y(0)
	{
	}

	TIKI_FORCE_INLINE Vector2::Vector2( const float all )
		:x(all), y(all)
	{
	}

	TIKI_FORCE_INLINE Vector2::Vector2( const float* data )
		:x(data[0]), y(data[1])
	{
	}

	TIKI_FORCE_INLINE Vector2::Vector2( const float nx, const float ny )
		:x(nx), y(ny)
	{

	}
	
	TIKI_FORCE_INLINE Vector2::Vector2( const float2& vec )
		: x( vec.x ), y( vec.y )
	{

	}

	TIKI_FORCE_INLINE Vector2::Vector2( const Vector2& vec )
		:x(vec.x), y(vec.y)
	{
	}

	TIKI_FORCE_INLINE Vector2::~Vector2( void )
	{

	}

	TIKI_FORCE_INLINE Vector2& Vector2::operator=( const Vector2& rhs )
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	TIKI_FORCE_INLINE bool Vector2::operator==( const Vector2& vec ) const
	{
		return isEquals( vec );
	}

	TIKI_FORCE_INLINE bool Vector2::operator!=( const Vector2& vec ) const
	{
		return !isEquals( vec );
	}

	TIKI_FORCE_INLINE float& Vector2::operator[]( const int index )
	{
		TIKI_ASSERT( index >= 0 && index <= 1 );
		return arr[ index ];
	}

	TIKI_FORCE_INLINE float Vector2::operator[]( const int index ) const
	{
		TIKI_ASSERT( index >= 0 && index <= 1 );
		return arr[ index ];
	}

	TIKI_FORCE_INLINE Vector2 Vector2::operator-( void ) const
	{
		return Vector2( -x, -y );
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

	TIKI_FORCE_INLINE Vector2::operator float*( void )
	{
		return arr;
	}

	TIKI_FORCE_INLINE Vector2::operator const float*( void ) const
	{
		return arr;
	}

	TIKI_FORCE_INLINE bool Vector2::isZero( const float epsilon /*= f32::s_epsilon*/) const 
	{
		return f32::isZero( x, epsilon ) &&
			f32::isZero( y, epsilon );
	}

	TIKI_FORCE_INLINE bool Vector2::isEquals( const Vector2& other, const float epsilon /*= f32::s_epsilon*/) const 
	{
		return f32::isEquals( x, other.x, epsilon ) &&
			f32::isEquals( y, other.y, epsilon );
	}

	TIKI_FORCE_INLINE void Vector2::negate( void )
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

	TIKI_FORCE_INLINE void Vector2::mul( const Vector2& vec, const float scalar )
	{
		x = vec.x * scalar;
		y = vec.y * scalar;
	}

	TIKI_FORCE_INLINE void Vector2::mul( const float scalar )
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

	TIKI_FORCE_INLINE void Vector2::div( const Vector2& vec, const float scalar )
	{
		const float tmp = 1.0f / scalar;
		x = vec.x * tmp;
		y = vec.y * tmp;
	}

	TIKI_FORCE_INLINE void Vector2::div( const float scalar )
	{
		const float tmp = 1.0f / scalar;
		x *= tmp;
		y *= tmp;
	}

	TIKI_FORCE_INLINE float Vector2::length( void ) const
	{
		return tiki::sqrt( x * x + y * y );
	}

	TIKI_FORCE_INLINE float Vector2::lengthSq( void ) const
	{
		return ( x * x + y * y );
	}

	TIKI_FORCE_INLINE void Vector2::normalize( void )
	{
		TIKI_ASSERT( !f32::isZero( lengthSq() ) );
		const float num = tiki::rsqrt( lengthSq() );
		mul( num );
	}

	TIKI_FORCE_INLINE void Vector2::normalize( const Vector2& vec )
	{
		TIKI_ASSERT( !f32::isZero( vec.lengthSq() ) );
		const float tmp = tiki::rsqrt( vec.lengthSq() );
		mul( vec, tmp );
	}

	TIKI_FORCE_INLINE void Vector2::truncate( const float length )
	{
		TIKI_ASSERT( !f32::isZero( length ) );
		const float tmp = length * tiki::rsqrt( this->lengthSq() );
		mul( tmp );
	}

	TIKI_FORCE_INLINE void Vector2::truncate( const Vector2& vec, const float length )
	{
		TIKI_ASSERT( !f32::isZero( length ) );
		const float tmp = length * tiki::rsqrt( vec.lengthSq() );
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
		x = tiki::clamp( x, min.x, max.x );
		y = tiki::clamp( y, min.y, max.y );
	}

	TIKI_FORCE_INLINE void Vector2::clamp( const Vector2& vec, const Vector2& min, const Vector2& max)
	{
		x = tiki::clamp( vec.x, min.x, max.x );
		y = tiki::clamp( vec.y, min.y, max.y );
	}

	TIKI_FORCE_INLINE void Vector2::snap( void )
	{
		x = tiki::snap( x );
		y = tiki::snap( y );
	}

	TIKI_FORCE_INLINE void Vector2::snap( const Vector2& vec )
	{
		x = tiki::snap( vec.x );
		y = tiki::snap( vec.y );
	}

	TIKI_FORCE_INLINE void Vector2::lerp( const Vector2& vec1, const Vector2& vec2, const float amount )
	{
		x = tiki::lerp( vec1.x, vec2.x, amount );
		y = tiki::lerp( vec1.y, vec2.y, amount );
	}

	TIKI_FORCE_INLINE void Vector2::smooth( const Vector2& vec1, const Vector2& vec2, const float amount )
	{
		float val =tiki::clamp( amount, 0.0f, 1.0f );
		val = val * val * ( 3.0f - 2.0f * val );

		x = tiki::lerp( vec1.x, vec2.x, val );
		y = tiki::lerp( vec1.y, vec2.y, val );
	}

	TIKI_FORCE_INLINE void Vector2::toFloat2( float2& target ) const
	{
		target.x = x;
		target.y = y;
	}
}

#endif // TIKI_VECTOR2_INL
