#pragma once
#ifndef TIKI_VECTOR4_INL
#define TIKI_VECTOR4_INL

namespace tiki
{
	TIKI_FORCE_INLINE Vector4::Vector4( void )
		:x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{

	}

	TIKI_FORCE_INLINE Vector4::Vector4( const float all )
		:x(all), y(all), z(all), w(all)
	{

	}

	TIKI_FORCE_INLINE Vector4::Vector4( const float* data )
		:x(data[0]), y(data[1]), z(data[2]), w(data[3])
	{

	}

	TIKI_FORCE_INLINE Vector4::Vector4( const float4& data )
		: x( data.x ), y( data.y ), z( data.z ), w( data.w )
	{
	}

	TIKI_FORCE_INLINE Vector4::Vector4( const float nx, const float ny, const float nz, const float nw )
		:x(nx), y(ny), z(nz), w(nw)
	{

	}

	TIKI_FORCE_INLINE Vector4::Vector4( const Vector4& vec )
		:x(vec.x), y(vec.y), z(vec.z), w(vec.w)
	{

	}

	TIKI_FORCE_INLINE Vector4::~Vector4( void )
	{

	}

	TIKI_FORCE_INLINE Vector4& Vector4::operator=( const Vector4& vec )
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
	}

	TIKI_FORCE_INLINE bool Vector4::operator==( const Vector4& vec ) const
	{
		return isEquals( vec );
	}

	TIKI_FORCE_INLINE bool Vector4::operator!=( const Vector4& vec ) const
	{
		return !isEquals( vec );
	}

	TIKI_FORCE_INLINE float& Vector4::operator[]( const int index )
	{
		TIKI_ASSERT( index >= 0 && index <= 3 );
		return arr[index];
	}

	TIKI_FORCE_INLINE float Vector4::operator[]( const int index ) const
	{
		TIKI_ASSERT( index >= 0 && index <= 3 );
		return arr[index];
	}

	TIKI_FORCE_INLINE Vector4 Vector4::operator-( void ) const
	{
		return Vector4( -x, -y, -z, -w );
	}

	TIKI_FORCE_INLINE Vector4::operator float *( void )
	{
		return arr;
	}

	TIKI_FORCE_INLINE Vector4::operator const float*( void ) const
	{
		return arr;
	}

	TIKI_FORCE_INLINE bool Vector4::isZero( const float epsilon /*= f32::s_epsilon*/ ) const 
	{
		return f32::isZero( x, epsilon ) &&
			f32::isZero( y, epsilon ) &&
			f32::isZero( z, epsilon ) &&
			f32::isZero( w, epsilon );
	}

	TIKI_FORCE_INLINE bool Vector4::isEquals( const Vector4& other, const float epsilon /*= f32::s_epsilon*/ ) const
	{
		return f32::isEquals( x, other.x, epsilon ) &&
			f32::isEquals( y, other.y, epsilon ) &&
			f32::isEquals( z, other.z, epsilon ) &&
			f32::isEquals( w, other.w, epsilon );
	}

	TIKI_FORCE_INLINE void Vector4::negate( void )
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

	TIKI_FORCE_INLINE void Vector4::mul( const Vector4& vec, const float scalar )
	{
		x = vec.x * scalar;
		y = vec.y * scalar;
		z = vec.z * scalar;
		w = vec.w * scalar;
	}

	TIKI_FORCE_INLINE void Vector4::mul( const float scalar )
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

	TIKI_FORCE_INLINE void Vector4::div( const Vector4& vec, const float scalar )
	{
		const float tmp = 1.0f / scalar;
		x = vec.x * tmp;
		y = vec.y * tmp;
		z = vec.z * tmp;
		w = vec.w * tmp;
	}

	TIKI_FORCE_INLINE void Vector4::div( const float scalar )
	{
		const float tmp = 1.0f / scalar;
		x *= tmp;
		y *= tmp;
		z *= tmp;
		w *= tmp;
	}

	TIKI_FORCE_INLINE float Vector4::length( void ) const
	{
		return tiki::sqrt( x * x + y * y + z * z + w * w );
	}

	TIKI_FORCE_INLINE float Vector4::lengthSq( void ) const
	{
		return x * x + y * y + z * z + w * w;
	}

	TIKI_FORCE_INLINE void Vector4::normalize( void )
	{
		TIKI_ASSERT( !f32::isZero( lengthSq() ) );
		const float tmp = tiki::rsqrt( this->length() );
		mul( tmp );
	}

	TIKI_FORCE_INLINE void Vector4::normalize( const Vector4& vec )
	{
		TIKI_ASSERT( !f32::isZero( vec.lengthSq() ) );
		const float tmp = tiki::rsqrt( vec.lengthSq() );
		mul( vec, tmp );
	}

	TIKI_FORCE_INLINE void Vector4::truncate( const float length )
	{
		TIKI_ASSERT( !f32::isZero( length ) );
		const float tmp = length * this->lengthSq();
		mul( tmp );
	}

	TIKI_FORCE_INLINE void Vector4::truncate( const Vector4& vec, const float length )
	{
		TIKI_ASSERT( !f32::isZero( length ) );
		const float tmp = length * vec.lengthSq();
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

	TIKI_FORCE_INLINE float Vector4::distanceSq( const Vector4& start, const Vector4& end )
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
		x = tiki::clamp( x, min.x, max.x );
		y = tiki::clamp( y, min.y, max.y );
		z = tiki::clamp( z, min.z, max.z );
	}

	TIKI_FORCE_INLINE void Vector4::clamp( const Vector4& vec, const Vector4& min, const Vector4& max )
	{
		x = tiki::clamp( vec.x, min.x, max.x );
		y = tiki::clamp( vec.y, min.y, max.y );
		z = tiki::clamp( vec.z, min.z, max.z );
	}

	TIKI_FORCE_INLINE void Vector4::snap( void )
	{
		x = tiki::snap( x );
		y = tiki::snap( y );
		z = tiki::snap( z );
		w = tiki::snap( w );
	}

	TIKI_FORCE_INLINE void Vector4::snap( const Vector4& vec )
	{
		x = tiki::snap( vec.x );
		y = tiki::snap( vec.y );
		z = tiki::snap( vec.z );
		w = tiki::snap( vec.w );
	}

	TIKI_FORCE_INLINE void Vector4::lerp( const Vector4& vec1, const Vector4& vec2, const float amount )
	{
		x = tiki::lerp( vec1.x, vec2.x, amount );
		y = tiki::lerp( vec1.y, vec2.y, amount );
		z = tiki::lerp( vec1.z, vec2.z, amount );
	}

	TIKI_FORCE_INLINE void Vector4::smooth( const Vector4& vec1, const Vector4& vec2, const float amount )
	{
		float val = tiki::clamp( amount, 0.0f, 1.0f );
		val = val * val * ( 3.0f - 2.0f * val );

		x = tiki::lerp( vec1.x, vec2.x, val );
		y = tiki::lerp( vec1.y, vec2.y, val );
		z = tiki::lerp( vec1.z, vec2.z, val );
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
