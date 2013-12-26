#pragma once
#ifndef __TIKI_MATRIX_INL_INCLUDED__
#define __TIKI_MATRIX_INL_INCLUDED__

#include "tiki/base/float32.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"

#include "tiki/math/matrix.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE bool matrix::isEquals( const Matrix33& lhs, const Matrix33& rhs, float epsilon /*= f32::epsilon*/ )
	{
		return vector::isEquals( lhs.x, rhs.x, epsilon ) && vector::isEquals( lhs.y, rhs.y, epsilon ) && vector::isEquals( lhs.z, rhs.z, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isEquals( const Matrix43& lhs, const Matrix43& rhs, float epsilon /*= f32::epsilon*/ )
	{
		return matrix::isEquals( lhs.rot, rhs.rot, epsilon ) && vector::isEquals( lhs.pos, rhs.pos, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isEquals( const Matrix44& lhs, const Matrix44& rhs, float epsilon /*= f32::epsilon*/ )
	{
		return vector::isEquals( lhs.x, rhs.x, epsilon ) && vector::isEquals( lhs.y, rhs.y, epsilon ) && vector::isEquals( lhs.z, rhs.z, epsilon ) && vector::isEquals( lhs.w, rhs.w, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isIdentity( const Matrix33& mtx, float epsilon /*= f32::epsilon*/ )
	{
		return vector::isEquals( mtx.x, Vector3::unitX, epsilon ) && vector::isEquals( mtx.y, Vector3::unitY, epsilon ) && vector::isEquals( mtx.z, Vector3::unitZ, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isIdentity( const Matrix43& mtx, float epsilon /*= f32::epsilon*/ )
	{
		return matrix::isIdentity( mtx.rot, epsilon ) && vector::isZero( mtx.pos, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isIdentity( const Matrix44& mtx, float epsilon /*= f32::epsilon*/ )
	{
		return vector::isEquals( mtx.x, Vector4::unitX, epsilon ) && vector::isEquals( mtx.y, Vector4::unitY, epsilon ) && vector::isEquals( mtx.z, Vector4::unitZ, epsilon ) && vector::isEquals( mtx.w, Vector4::unitW, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isZero( const Matrix33& mtx, float epsilon /*= f32::epsilon*/ )
	{
		return vector::isZero( mtx.x, epsilon ) && vector::isZero( mtx.y, epsilon ) && vector::isZero( mtx.z, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isZero( const Matrix43& mtx, float epsilon /*= f32::epsilon*/ )
	{
		return matrix::isZero( mtx.rot, epsilon ) && vector::isZero( mtx.pos, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isZero( const Matrix44& mtx, float epsilon /*= f32::epsilon*/ )
	{
		return vector::isZero( mtx.x, epsilon ) && vector::isZero( mtx.y, epsilon ) && vector::isZero( mtx.z, epsilon ) && vector::isZero( mtx.w, epsilon );
	}

	TIKI_FORCE_INLINE Matrix33& matrix::clear( Matrix33& mtx )
	{
		vector::clear( mtx.x );
		vector::clear( mtx.y );
		vector::clear( mtx.z );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix43& matrix::clear( Matrix43& mtx )
	{
		matrix::clear( mtx.rot );
		vector::clear( mtx.pos );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::clear( Matrix44& mtx )
	{
		vector::clear( mtx.x );
		vector::clear( mtx.y );
		vector::clear( mtx.z );
		vector::clear( mtx.w );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::createIdentity( Matrix33& mtx )
	{
		mtx.x = Vector3::unitX;
		mtx.y = Vector3::unitY;
		mtx.z = Vector3::unitZ;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix43& matrix::createIdentity( Matrix43& mtx )
	{
		matrix::createIdentity( mtx.rot );
		vector::clear( mtx.pos );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::createIdentity( Matrix44& mtx )
	{
		mtx.x = Vector4::unitX;
		mtx.y = Vector4::unitY;
		mtx.z = Vector4::unitZ;
		mtx.w = Vector4::unitW;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::set( Matrix33& mtx, const Vector3& x, const Vector3& y, const Vector3& z )
	{
		mtx.x = x;
		mtx.y = y;
		mtx.z = z;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix43& matrix::set( Matrix43& mtx, const Matrix33& rot, const Vector3& pos )
	{
		mtx.rot = rot;
		mtx.pos = pos;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::set( Matrix44& mtx, const Matrix43& rotPos )
	{
		vector::set( mtx.x, rotPos.rot.x, rotPos.pos.x );
		vector::set( mtx.y, rotPos.rot.y, rotPos.pos.y );
		vector::set( mtx.z, rotPos.rot.z, rotPos.pos.z );
		mtx.w = Vector4::unitW;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::set( Matrix44& mtx, const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w )
	{
		mtx.x = x;
		mtx.y = y;
		mtx.z = z;
		mtx.w = w;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::add( Matrix33& mtx, const Matrix33& rhs )
	{
		vector::add( mtx.x, rhs.x );
		vector::add( mtx.y, rhs.y );
		vector::add( mtx.z, rhs.z );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix43& matrix::add( Matrix43& mtx, const Matrix43& rhs )
	{
		matrix::add( mtx.rot, rhs.rot );
		vector::add( mtx.pos, rhs.pos );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::add( Matrix44& mtx, const Matrix44& rhs )
	{
		vector::add( mtx.x, rhs.x );
		vector::add( mtx.y, rhs.y );
		vector::add( mtx.z, rhs.z );
		vector::add( mtx.w, rhs.w );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::sub( Matrix33& mtx, const Matrix33& rhs )
	{
		vector::sub( mtx.x, rhs.x );
		vector::sub( mtx.y, rhs.y );
		vector::sub( mtx.z, rhs.z );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix43& matrix::sub( Matrix43& mtx, const Matrix43& rhs )
	{
		matrix::sub( mtx.rot, rhs.rot );
		vector::sub( mtx.pos, rhs.pos );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::sub( Matrix44& mtx, const Matrix44& rhs )
	{
		vector::sub( mtx.x, rhs.x );
		vector::sub( mtx.y, rhs.y );
		vector::sub( mtx.z, rhs.z );
		vector::sub( mtx.w, rhs.w );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::mul( Matrix33& mtx, const Matrix33& rhs )
	{
		Matrix33 right = rhs;
		transpose( right );
		mtx.x.x = vector::dot( mtx.x, right.x );
		mtx.x.y = vector::dot( mtx.x, right.y );
		mtx.x.z = vector::dot( mtx.x, right.z );
		mtx.y.x = vector::dot( mtx.y, right.x );
		mtx.y.y = vector::dot( mtx.y, right.y );
		mtx.y.z = vector::dot( mtx.y, right.z );
		mtx.z.x = vector::dot( mtx.z, right.x );
		mtx.z.y = vector::dot( mtx.z, right.y );
		mtx.z.z = vector::dot( mtx.z, right.z );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix43& matrix::mul( Matrix43& mtx, const Matrix43& rhs )
	{
		matrix::mul( mtx.rot, rhs.rot );
		vector::add( mtx.pos, rhs.pos );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::mul( Matrix44& mtx, const Matrix44& rhs )
	{
		Matrix44 right = rhs;
		transpose( right );
		mtx.x.x = vector::dot( mtx.x, right.x );
		mtx.x.y = vector::dot( mtx.x, right.y );
		mtx.x.z = vector::dot( mtx.x, right.z );
		mtx.x.w = vector::dot( mtx.x, right.w );
		mtx.y.x = vector::dot( mtx.y, right.x );
		mtx.y.y = vector::dot( mtx.y, right.y );
		mtx.y.z = vector::dot( mtx.y, right.z );
		mtx.y.w = vector::dot( mtx.y, right.w );
		mtx.z.x = vector::dot( mtx.z, right.x );
		mtx.z.y = vector::dot( mtx.z, right.y );
		mtx.z.z = vector::dot( mtx.z, right.z );
		mtx.z.w = vector::dot( mtx.z, right.w );
		mtx.w.x = vector::dot( mtx.w, right.x );
		mtx.w.y = vector::dot( mtx.w, right.y );
		mtx.w.z = vector::dot( mtx.w, right.z );
		mtx.w.w = vector::dot( mtx.w, right.w );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::div( Matrix33& mtx, const Matrix33& rhs )
	{

	}

	TIKI_FORCE_INLINE Matrix43& matrix::div( Matrix43& mtx, const Matrix43& rhs )
	{

	}

	TIKI_FORCE_INLINE Matrix44& matrix::div( Matrix44& mtx, const Matrix44& rhs )
	{

	}

	TIKI_FORCE_INLINE Matrix33& matrix::scale( Matrix33& mtx, float val )
	{
		vector::scale( mtx.x, val );
		vector::scale( mtx.y, val );
		vector::scale( mtx.z, val );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix43& matrix::scale( Matrix43& mtx, float val )
	{
		matrix::scale( mtx.rot, val );
		vector::scale( mtx.pos, val );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::scale( Matrix44& mtx, float val )
	{
		vector::scale( mtx.x, val );
		vector::scale( mtx.y, val );
		vector::scale( mtx.z, val );
		vector::scale( mtx.w, val );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::invert( Matrix33& mtx, const Matrix33& source )
	{
		const float det =
			source.x.x * source.y.y * source.z.z +
			source.x.y * source.y.z * source.z.x +
			source.x.z * source.y.x * source.z.y -
			source.z.x * source.y.y * source.x.z -
			source.z.y * source.y.z * source.x.x -
			source.z.z * source.y.x * source.x.y;
		TIKI_ASSERT( f32::isZero( det ) == false );
		mtx.x.x = source.y.y * source.z.z - source.y.z * source.z.y;
		mtx.x.y = source.x.z * source.z.y - source.x.y * source.z.z;
		mtx.x.z = source.x.y * source.y.z - source.x.z * source.y.y;
		mtx.y.x = source.y.z * source.z.x - source.y.x * source.z.z;
		mtx.y.y = source.x.x * source.z.z - source.x.z * source.z.x;
		mtx.y.z = source.x.z * source.y.x - source.x.x * source.y.z;
		mtx.z.x = source.y.x * source.z.y - source.y.y * source.z.x;
		mtx.z.y = source.x.y * source.z.x - source.x.x * source.z.y;
		mtx.z.z = source.x.x * source.y.y - source.x.y * source.y.x;
		return scale( mtx, 1.0f / det );
	}

	TIKI_FORCE_INLINE Matrix43& matrix::invert( Matrix43& mtx, const Matrix43& source )
	{
		matrix::invert( mtx.rot, source.rot );
		vector::negate( mtx.pos );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::invert( Matrix44& mtx, const Matrix44& source )
	{

	}

	TIKI_FORCE_INLINE Matrix33& matrix::lerp( Matrix33& mtx, const Matrix33& start, const Matrix33& end, const float amount )
	{
		vector::lerp( mtx.x, start.x, end.x, amount );
		vector::lerp( mtx.y, start.y, end.y, amount );
		vector::lerp( mtx.z, start.z, end.z, amount );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix43& matrix::lerp( Matrix43& mtx, const Matrix43& start, const Matrix43& end, const float amount )
	{
		matrix::lerp( mtx.rot, start.rot, end.rot, amount );
		vector::lerp( mtx.pos, start.pos, end.pos, amount );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::lerp( Matrix44& mtx, const Matrix44& start, const Matrix44& end, const float amount )
	{
		vector::lerp( mtx.x, start.x, end.x, amount );
		vector::lerp( mtx.y, start.y, end.y, amount );
		vector::lerp( mtx.z, start.z, end.z, amount );
		vector::lerp( mtx.w, start.w, end.w, amount );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::transpose( Matrix33& mtx )
	{
		const float xy = mtx.y.x;
		const float xz = mtx.z.x;
		const float yx = mtx.x.y;
		const float yz = mtx.z.y;
		const float zx = mtx.x.z;
		const float zy = mtx.y.z;
		mtx.x.y = xy;
		mtx.x.z = xz;
		mtx.y.x = yx;
		mtx.y.z = yz;
		mtx.z.x = zx;
		mtx.z.y = zy;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::transpose( Matrix44& mtx )
	{
		const float xy = mtx.y.x;
		const float xz = mtx.z.x;
		const float xw = mtx.w.x;
		const float yx = mtx.x.y;
		const float yz = mtx.z.y;
		const float yw = mtx.w.y;
		const float zx = mtx.x.z;
		const float zy = mtx.y.z;
		const float zw = mtx.w.z;
		const float wx = mtx.x.w;
		const float wy = mtx.y.w;
		const float wz = mtx.z.w;
		mtx.x.y = xy;
		mtx.x.z = xz;
		mtx.x.w = xw;
		mtx.y.x = yx;
		mtx.y.z = yz;
		mtx.y.w = yw;
		mtx.z.x = zx;
		mtx.z.y = zy;
		mtx.z.w = zw;
		mtx.w.x = wx;
		mtx.w.y = wy;
		mtx.w.z = wz;
		return mtx;
	}

	TIKI_FORCE_INLINE void matrix::transform( Vector3& vec, const Matrix33& mtx )
	{
		vector::set(
			vec,
			vector::dot( vec, mtx.x ),
			vector::dot( vec, mtx.y ),
			vector::dot( vec, mtx.z )
		);
	}

	TIKI_FORCE_INLINE void matrix::transform( Vector3& vec, const Matrix43& mtx )
	{
		matrix::transform( vec, mtx.rot );
		vector::add( vec, mtx.pos );
	}

	TIKI_FORCE_INLINE void matrix::transform( Vector3& vec, const Matrix44& mtx )
	{
		Vector4 vec4;
		vector::set( vec4, vec, 1.0f );
		vector::set(
			vec,
			vector::dot( vec4, mtx.x ),
			vector::dot( vec4, mtx.y ),
			vector::dot( vec4, mtx.z )
		);
		vector::scale(
			vec,
			1.0f / vector::dot( vec4, mtx.w )
		);
	}

	TIKI_FORCE_INLINE void matrix::transform( Vector4& vec, const Matrix44& mtx )
	{
		vector::set(
			vec,
			vector::dot( vec, mtx.x ),
			vector::dot( vec, mtx.y ),
			vector::dot( vec, mtx.z ),
			vector::dot( vec, mtx.w )
		);
	}
}

#endif // __TIKI_MATRIX_INL_INCLUDED__
