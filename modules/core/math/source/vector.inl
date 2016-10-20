#pragma once
#ifndef __TIKI_VECTOR_INL_INCLUDED__
#define __TIKI_VECTOR_INL_INCLUDED__

#include "tiki/base/assert.hpp"
#include "tiki/math/basetypes.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE bool vector::isEquals( const Vector2& lhs, const Vector2& rhs, float epsilon /*= f32::epsilon*/ )
	{
		return f32::isEquals( lhs.x, rhs.x, epsilon ) && f32::isEquals( lhs.y, rhs.y, epsilon );
	}

	TIKI_FORCE_INLINE bool vector::isEquals( const Vector3& lhs, const Vector3& rhs, float epsilon /*= f32::epsilon*/ )
	{
		return f32::isEquals( lhs.x, rhs.x, epsilon ) && f32::isEquals( lhs.y, rhs.y, epsilon ) && f32::isEquals( lhs.z, rhs.z, epsilon );
	}

	TIKI_FORCE_INLINE bool vector::isEquals( const Vector4& lhs, const Vector4& rhs, float epsilon /*= f32::epsilon*/ )
	{
		return f32::isEquals( lhs.x, rhs.x, epsilon ) && f32::isEquals( lhs.y, rhs.y, epsilon ) && f32::isEquals( lhs.z, rhs.z, epsilon ) && f32::isEquals( lhs.w, rhs.w, epsilon );
	}

	TIKI_FORCE_INLINE bool vector::isZero( const Vector2& vec, float epsilon /*= f32::epsilon*/ )
	{
		return f32::isZero( vec.x, epsilon ) && f32::isZero( vec.y, epsilon );
	}

	TIKI_FORCE_INLINE bool vector::isZero( const Vector3& vec, float epsilon /*= f32::epsilon*/ )
	{
		return f32::isZero( vec.x, epsilon ) && f32::isZero( vec.y, epsilon ) && f32::isZero( vec.z, epsilon );
	}

	TIKI_FORCE_INLINE bool vector::isZero( const Vector4& vec, float epsilon /*= f32::epsilon*/ )
	{
		return f32::isZero( vec.x, epsilon ) && f32::isZero( vec.y, epsilon ) && f32::isZero( vec.z, epsilon ) && f32::isZero( vec.w, epsilon );
	}
	
	TIKI_FORCE_INLINE bool vector::isNormalized( const Vector2& vec, float epsilon /*= f32::epsilon */ )
	{
		return f32::isEquals( vector::length( vec ), 1.0f, epsilon );
	}

	TIKI_FORCE_INLINE bool vector::isNormalized( const Vector3& vec, float epsilon /*= f32::epsilon */ )
	{
		return f32::isEquals( vector::length( vec ), 1.0f, epsilon );
	}

	TIKI_FORCE_INLINE bool vector::isNormalized( const Vector4& vec, float epsilon /*= f32::epsilon */ )
	{
		return f32::isEquals( vector::length( vec ), 1.0f, epsilon );
	}

	TIKI_FORCE_INLINE Vector2 vector::create( float x, float y )
	{
		const Vector2 vec = { x, y };
		return vec;
	}

	TIKI_FORCE_INLINE Vector3 vector::create( float x, float y, float z )
	{
		const Vector3 vec = { x, y, z };
		return vec;
	}

	TIKI_FORCE_INLINE Vector4 vector::create( float x, float y, float z, float w )
	{
		const Vector4 vec = { x, y, z, w };
		return vec;
	}

	TIKI_FORCE_INLINE Vector2 vector::create( const float2& source )
	{
		const Vector2 vec = { source.x, source.y };
		return vec;
	}

	TIKI_FORCE_INLINE Vector3 vector::create( const float3& source )
	{
		const Vector3 vec = { source.x, source.y, source.z };
		return vec;
	}

	TIKI_FORCE_INLINE Vector4 vector::create( const float4& source )
	{
		const Vector4 vec = { source.x, source.y, source.z, source.w };
		return vec;
	}

	TIKI_FORCE_INLINE void vector::toFloat( float2& target, const Vector2& source )
	{
		createFloat2( target, source.x, source.y );
	}

	TIKI_FORCE_INLINE void vector::toFloat( float3& target, const Vector3& source )
	{
		createFloat3( target, source.x, source.y, source.z );
	}
	TIKI_FORCE_INLINE void vector::toFloat( float4& target, const Vector4& source )
	{
		createFloat4( target, source.x, source.y, source.z, source.w );
	}

	TIKI_FORCE_INLINE Vector2& vector::clear( Vector2& vec )
	{
		vec.x = 0.0f;
		vec.y = 0.0f;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::clear( Vector3& vec )
	{
		vec.x = 0.0f;
		vec.y = 0.0f;
		vec.z = 0.0f;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::clear( Vector4& vec )
	{
		vec.x = 0.0f;
		vec.y = 0.0f;
		vec.z = 0.0f;
		vec.w = 0.0f;
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::set( Vector2& vec, const float2& source )
	{
		vec.x = source.x;
		vec.y = source.y;
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::set( Vector2& vec, float x, float y )
	{
		vec.x = x;
		vec.y = y;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::set( Vector3& vec, const float3& source )
	{
		vec.x = source.x;
		vec.y = source.y;
		vec.z = source.z;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::set( Vector3& vec, const Vector2& source, float z )
	{
		vec.x = source.x;
		vec.y = source.y;
		vec.z = z;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::set( Vector3& vec, float x, float y, float z )
	{
		vec.x = x;
		vec.y = y;
		vec.z = z;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::set( Vector4& vec, const float4& source )
	{
		vec.x = source.x;
		vec.y = source.y;
		vec.z = source.z;
		vec.w = source.w;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::set( Vector4& vec, const Vector3& source, float w )
	{
		vec.x = source.x;
		vec.y = source.y;
		vec.z = source.z;
		vec.w = w;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::set( Vector4& vec, float x, float y, float z, float w )
	{
		vec.x = x;
		vec.y = y;
		vec.z = z;
		vec.w = w;
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::add( Vector2& vec, const Vector2& rhs )
	{
		vec.x += rhs.x;
		vec.y += rhs.y;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::add( Vector3& vec, const Vector3& rhs )
	{
		vec.x += rhs.x;
		vec.y += rhs.y;
		vec.z += rhs.z;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::add( Vector4& vec, const Vector4& rhs )
	{
		vec.x += rhs.x;
		vec.y += rhs.y;
		vec.z += rhs.z;
		vec.w += rhs.w;
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::add( Vector2& vec, const Vector2& lhs, const Vector2& rhs )
	{
		vec.x = lhs.x + rhs.x;
		vec.y = lhs.y + rhs.y;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::add( Vector3& vec, const Vector3& lhs, const Vector3& rhs )
	{
		vec.x = lhs.x + rhs.x;
		vec.y = lhs.y + rhs.y;
		vec.z = lhs.z + rhs.z;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::add( Vector4& vec, const Vector4& lhs, const Vector4& rhs )
	{
		vec.x = lhs.x + rhs.x;
		vec.y = lhs.y + rhs.y;
		vec.z = lhs.z + rhs.z;
		vec.w = lhs.w + rhs.w;
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::sub( Vector2& vec, const Vector2& rhs )
	{
		vec.x -= rhs.x;
		vec.y -= rhs.y;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::sub( Vector3& vec, const Vector3& rhs )
	{
		vec.x -= rhs.x;
		vec.y -= rhs.y;
		vec.z -= rhs.z;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::sub( Vector4& vec, const Vector4& rhs )
	{
		vec.x -= rhs.x;
		vec.y -= rhs.y;
		vec.z -= rhs.z;
		vec.w -= rhs.w;
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::sub( Vector2& vec, const Vector2& lhs, const Vector2& rhs )
	{
		vec.x = lhs.x - rhs.x;
		vec.y = lhs.y - rhs.y;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::sub( Vector3& vec, const Vector3& lhs, const Vector3& rhs )
	{
		vec.x = lhs.x - rhs.x;
		vec.y = lhs.y - rhs.y;
		vec.z = lhs.z - rhs.z;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::sub( Vector4& vec, const Vector4& lhs, const Vector4& rhs )
	{
		vec.x = lhs.x - rhs.x;
		vec.y = lhs.y - rhs.y;
		vec.z = lhs.z - rhs.z;
		vec.w = lhs.w - rhs.w;
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::mul( Vector2& vec, const Vector2& rhs )
	{
		vec.x *= rhs.x;
		vec.y *= rhs.y;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::mul( Vector3& vec, const Vector3& rhs )
	{
		vec.x *= rhs.x;
		vec.y *= rhs.y;
		vec.z *= rhs.z;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::mul( Vector4& vec, const Vector4& rhs )
	{
		vec.x *= rhs.x;
		vec.y *= rhs.y;
		vec.z *= rhs.z;
		vec.w *= rhs.w;
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::mul( Vector2& vec, const Vector2& lhs, const Vector2& rhs )
	{
		vec.x = lhs.x * rhs.x;
		vec.y = lhs.y * rhs.y;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::mul( Vector3& vec, const Vector3& lhs, const Vector3& rhs )
	{
		vec.x = lhs.x * rhs.x;
		vec.y = lhs.y * rhs.y;
		vec.z = lhs.z * rhs.z;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::mul( Vector4& vec, const Vector4& lhs, const Vector4& rhs )
	{
		vec.x = lhs.x * rhs.x;
		vec.y = lhs.y * rhs.y;
		vec.z = lhs.z * rhs.z;
		vec.w = lhs.w * rhs.w;
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::div( Vector2& vec, const Vector2& rhs )
	{
		vec.x /= rhs.x;
		vec.y /= rhs.y;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::div( Vector3& vec, const Vector3& rhs )
	{
		vec.x /= rhs.x;
		vec.y /= rhs.y;
		vec.z /= rhs.z;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::div( Vector4& vec, const Vector4& rhs )
	{
		vec.x /= rhs.x;
		vec.y /= rhs.y;
		vec.z /= rhs.z;
		vec.w /= rhs.w;
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::div( Vector2& vec, const Vector2& lhs, const Vector2& rhs )
	{
		vec.x = lhs.x / rhs.x;
		vec.y = lhs.y / rhs.y;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::div( Vector3& vec, const Vector3& lhs, const Vector3& rhs )
	{
		vec.x = lhs.x / rhs.x;
		vec.y = lhs.y / rhs.y;
		vec.z = lhs.z / rhs.z;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::div( Vector4& vec, const Vector4& lhs, const Vector4& rhs )
	{
		vec.x = lhs.x / rhs.x;
		vec.y = lhs.y / rhs.y;
		vec.z = lhs.z / rhs.z;
		vec.w = lhs.w / rhs.w;
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::scale( Vector2& vec, float val )
	{
		vec.x *= val;
		vec.y *= val;
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::scale( Vector3& vec, float val )
	{
		vec.x *= val;
		vec.y *= val;
		vec.z *= val;
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::scale( Vector4& vec, float val )
	{
		vec.x *= val;
		vec.y *= val;
		vec.z *= val;
		vec.w *= val;
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::negate( Vector2& vec )
	{
		return scale( vec, -1.0f );
	}

	TIKI_FORCE_INLINE Vector3& vector::negate( Vector3& vec )
	{
		return scale( vec, -1.0f );
	}

	TIKI_FORCE_INLINE Vector4& vector::negate( Vector4& vec )
	{
		return scale( vec, -1.0f );
	}

	TIKI_FORCE_INLINE float vector::length( const Vector2& vec )
	{
		return f32::sqrt( lengthSquared( vec ) );
	}

	TIKI_FORCE_INLINE float vector::length( const Vector3& vec )
	{
		return f32::sqrt( lengthSquared( vec ) );
	}

	TIKI_FORCE_INLINE float vector::length( const Vector4& vec )
	{
		return f32::sqrt( lengthSquared( vec ) );
	}

	TIKI_FORCE_INLINE float vector::lengthSquared( const Vector2& vec )
	{
		return dot( vec, vec );
	}

	TIKI_FORCE_INLINE float vector::lengthSquared( const Vector3& vec )
	{
		return dot( vec, vec );
	}

	TIKI_FORCE_INLINE float vector::lengthSquared( const Vector4& vec )
	{
		return dot( vec, vec );
	}

	TIKI_FORCE_INLINE Vector2& vector::normalize( Vector2& vec )
	{
		TIKI_ASSERT( f32::isZero( lengthSquared( vec ) ) == false );
		return scale(
			vec,
			f32::rsqrt( lengthSquared( vec ) )
		);
	}

	TIKI_FORCE_INLINE Vector3& vector::normalize( Vector3& vec )
	{
		TIKI_ASSERT( f32::isZero( lengthSquared( vec ) ) == false );
		return scale(
			vec,
			f32::rsqrt( lengthSquared( vec ) )
		);
	}

	TIKI_FORCE_INLINE Vector4& vector::normalize( Vector4& vec )
	{
		TIKI_ASSERT( f32::isZero( lengthSquared( vec ) ) == false );
		return scale(
			vec,
			f32::rsqrt( lengthSquared( vec ) )
		);
	}

	TIKI_FORCE_INLINE Vector2& vector::normalizeZero( Vector2& vec )
	{
		const float lenghtSqr = lengthSquared( vec );
		if ( f32::isZero( lenghtSqr ) )
		{
			return clear( vec );
		}

		return scale( vec, f32::rsqrt( lenghtSqr ) );
	}

	TIKI_FORCE_INLINE Vector3& vector::normalizeZero( Vector3& vec )
	{
		const float lenghtSqr = lengthSquared( vec );
		if ( f32::isZero( lenghtSqr ) )
		{
			return clear( vec );
		}

		return scale( vec, f32::rsqrt( lenghtSqr ) );
	}

	TIKI_FORCE_INLINE Vector4& vector::normalizeZero( Vector4& vec )
	{
		const float lenghtSqr = lengthSquared( vec );
		if ( f32::isZero( lenghtSqr ) )
		{
			return clear( vec );
		}

		return scale( vec, f32::rsqrt( lenghtSqr ) );
	}

	TIKI_FORCE_INLINE Vector2& vector::truncate( Vector2& vec, float len )
	{
		TIKI_ASSERT( !f32::isZero( len ) );
		return scale(
			vec,
			len * f32::rsqrt( lengthSquared( vec ) )
		);
	}

	TIKI_FORCE_INLINE Vector3& vector::truncate( Vector3& vec, float len )
	{
		TIKI_ASSERT( !f32::isZero( len ) );
		return scale(
			vec,
			len * f32::rsqrt( lengthSquared( vec ) )
		);
	}

	TIKI_FORCE_INLINE Vector4& vector::truncate( Vector4& vec, float len )
	{
		TIKI_ASSERT( !f32::isZero( len ) );
		return scale(
			vec,
			len * f32::rsqrt( lengthSquared( vec ) )
		);
	}

	TIKI_FORCE_INLINE float vector::distance( const Vector2& start, const Vector2& end )
	{
		return f32::sqrt( distanceSquared( start, end ) );
	}

	TIKI_FORCE_INLINE float vector::distance( const Vector3& start, const Vector3& end )
	{
		return f32::sqrt( distanceSquared( start, end ) );
	}

	TIKI_FORCE_INLINE float vector::distance( const Vector4& start, const Vector4& end )
	{
		return f32::sqrt( distanceSquared( start, end ) );
	}

	TIKI_FORCE_INLINE float vector::distanceSquared( const Vector2& start, const Vector2& end )
	{
		Vector2 vec = end;
		sub( vec, start );
		return dot( vec, vec );
	}

	TIKI_FORCE_INLINE float vector::distanceSquared( const Vector3& start, const Vector3& end )
	{
		Vector3 vec = end;
		sub( vec, start );
		return dot( vec, vec );
	}

	TIKI_FORCE_INLINE float vector::distanceSquared( const Vector4& start, const Vector4& end )
	{
		Vector4 vec = end;
		sub( vec, start );
		return dot( vec, vec );
	}

	TIKI_FORCE_INLINE float vector::dot( const Vector2& lhs, const Vector2& rhs )
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	TIKI_FORCE_INLINE float vector::dot( const Vector3& lhs, const Vector3& rhs )
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	TIKI_FORCE_INLINE float vector::dot( const Vector4& lhs, const Vector4& rhs )
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	TIKI_FORCE_INLINE Vector3& vector::cross( Vector3& vec, const Vector3& lhs, const Vector3& rhs )
	{
		vec.x = ( lhs.y * rhs.z ) - ( lhs.z * rhs.y );
		vec.y = ( lhs.z * rhs.x ) - ( lhs.x * rhs.z );
		vec.z = ( lhs.x * rhs.y ) - ( lhs.y * rhs.x );
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::reflect( Vector2& vec, const Vector2& source, const Vector2& normal )
	{
		TIKI_ASSERT( isNormalized( normal ) );

		Vector2 modNormal = normal;
		scale( modNormal, dot( source, normal ) * 2.0f );
		vec = source;
		return sub( vec, modNormal );
	}

	TIKI_FORCE_INLINE Vector3& vector::reflect( Vector3& vec, const Vector3& source, const Vector3& normal )
	{
		TIKI_ASSERT( isNormalized( normal ) );

		Vector3 modNormal = normal;
		scale( modNormal, dot( source, normal ) * 2.0f );
		vec = source;
		return sub( vec, modNormal );
	}

	TIKI_FORCE_INLINE Vector4& vector::reflect( Vector4& vec, const Vector4& source, const Vector4& normal )
	{
		TIKI_ASSERT( isNormalized( normal ) );

		Vector4 modNormal = normal;
		scale( modNormal, dot( source, normal ) * 2.0f );
		vec = source;
		return sub( vec, modNormal );
	}

	TIKI_FORCE_INLINE Vector2& vector::clamp( Vector2& vec, const Vector2& min, const Vector2& max )
	{
		vec.x = f32::clamp( vec.x, min.x, max.x );
		vec.y = f32::clamp( vec.y, min.y, max.y );
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::clamp( Vector3& vec, const Vector3& min, const Vector3& max )
	{
		vec.x = f32::clamp( vec.x, min.x, max.x );
		vec.y = f32::clamp( vec.y, min.y, max.y );
		vec.z = f32::clamp( vec.z, min.z, max.z );
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::clamp( Vector4& vec, const Vector4& min, const Vector4& max )
	{
		vec.x = f32::clamp( vec.x, min.x, max.x );
		vec.y = f32::clamp( vec.y, min.y, max.y );
		vec.z = f32::clamp( vec.z, min.z, max.z );
		vec.w = f32::clamp( vec.w, min.w, max.w );
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::floor( Vector2& vec )
	{
		vec.x = f32::floor( vec.x );
		vec.y = f32::floor( vec.y );
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::floor( Vector3& vec )
	{
		vec.x = f32::floor( vec.x );
		vec.y = f32::floor( vec.y );
		vec.z = f32::floor( vec.z );
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::floor( Vector4& vec )
	{
		vec.x = f32::floor( vec.x );
		vec.y = f32::floor( vec.y );
		vec.z = f32::floor( vec.z );
		vec.w = f32::floor( vec.w );
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::ceil( Vector2& vec )
	{
		vec.x = f32::ceil( vec.x );
		vec.y = f32::ceil( vec.y );
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::ceil( Vector3& vec )
	{
		vec.x = f32::ceil( vec.x );
		vec.y = f32::ceil( vec.y );
		vec.z = f32::ceil( vec.z );
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::ceil( Vector4& vec )
	{
		vec.x = f32::ceil( vec.x );
		vec.y = f32::ceil( vec.y );
		vec.z = f32::ceil( vec.z );
		vec.w = f32::ceil( vec.w );
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::lerp( Vector2& vec, const Vector2& start, const Vector2& end, float amount )
	{
		TIKI_ASSERT( amount >= 0.0f && amount <= 1.0f );
		vec.x = f32::lerp( start.x, end.x, amount );
		vec.y = f32::lerp( start.y, end.y, amount );
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::lerp( Vector3& vec, const Vector3& start, const Vector3& end, float amount )
	{
		TIKI_ASSERT( amount >= 0.0f && amount <= 1.0f );
		vec.x = f32::lerp( start.x, end.x, amount );
		vec.y = f32::lerp( start.y, end.y, amount );
		vec.z = f32::lerp( start.z, end.z, amount );
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::lerp( Vector4& vec, const Vector4& start, const Vector4& end, float amount )
	{
		TIKI_ASSERT( amount >= 0.0f && amount <= 1.0f );
		vec.x = f32::lerp( start.x, end.x, amount );
		vec.y = f32::lerp( start.y, end.y, amount );
		vec.z = f32::lerp( start.z, end.z, amount );
		vec.w = f32::lerp( start.w, end.w, amount );
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::smooth( Vector2& vec, const Vector2& start, const Vector2& end, float amount )
	{
		return lerp( vec, start, end, amount * amount * ( 3.0f - 2.0f * amount ) );
	}

	TIKI_FORCE_INLINE Vector3& vector::smooth( Vector3& vec, const Vector3& start, const Vector3& end, float amount )
	{
		return lerp( vec, start, end, amount * amount * ( 3.0f - 2.0f * amount ) );
	}

	TIKI_FORCE_INLINE Vector4& vector::smooth( Vector4& vec, const Vector4& start, const Vector4& end, float amount )
	{
		return lerp( vec, start, end, amount * amount * ( 3.0f - 2.0f * amount ) );
	}

	TIKI_FORCE_INLINE Vector2& vector::minimum( Vector2& vec, const Vector2& lhs, const Vector2& rhs )
	{
		vec.x = TIKI_MIN( lhs.x, rhs.x );
		vec.y = TIKI_MIN( lhs.y, rhs.y );
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::minimum( Vector3& vec, const Vector3& lhs, const Vector3& rhs )
	{
		vec.x = TIKI_MIN( lhs.x, rhs.x );
		vec.y = TIKI_MIN( lhs.y, rhs.y );
		vec.z = TIKI_MIN( lhs.z, rhs.z );
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::minimum( Vector4& vec, const Vector4& lhs, const Vector4& rhs )
	{
		vec.x = TIKI_MIN( lhs.x, rhs.x );
		vec.y = TIKI_MIN( lhs.y, rhs.y );
		vec.z = TIKI_MIN( lhs.z, rhs.z );
		vec.w = TIKI_MIN( lhs.w, rhs.w );
		return vec;
	}

	TIKI_FORCE_INLINE Vector2& vector::maximum( Vector2& vec, const Vector2& lhs, const Vector2& rhs )
	{
		vec.x = TIKI_MAX( lhs.x, rhs.x );
		vec.y = TIKI_MAX( lhs.y, rhs.y );
		return vec;
	}

	TIKI_FORCE_INLINE Vector3& vector::maximum( Vector3& vec, const Vector3& lhs, const Vector3& rhs )
	{
		vec.x = TIKI_MAX( lhs.x, rhs.x );
		vec.y = TIKI_MAX( lhs.y, rhs.y );
		vec.z = TIKI_MAX( lhs.z, rhs.z );
		return vec;
	}

	TIKI_FORCE_INLINE Vector4& vector::maximum( Vector4& vec, const Vector4& lhs, const Vector4& rhs )
	{
		vec.x = TIKI_MAX( lhs.x, rhs.x );
		vec.y = TIKI_MAX( lhs.y, rhs.y );
		vec.z = TIKI_MAX( lhs.z, rhs.z );
		vec.w = TIKI_MAX( lhs.w, rhs.w );
		return vec;
	}
}

#endif // __TIKI_VECTOR_INL_INCLUDED__
