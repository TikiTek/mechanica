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

	TIKI_FORCE_INLINE Matrix33& matrix::createScale( Matrix33& mtx, const Vector3& scale )
	{
		matrix::clear( mtx );
		mtx.x.x = scale.x;
		mtx.y.y = scale.y;
		mtx.z.z = scale.z;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::createRotationX( Matrix33& mtx, float angle )
	{
		const float s = sinf( angle );
		const float c = cosf( angle );
		matrix::clear( mtx );
		mtx.x.x = 1.0f;
		mtx.y.y = c;
		mtx.y.z = s;
		mtx.z.y = -s;
		mtx.z.z = c;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::createRotationY( Matrix33& mtx, float angle )
	{
		const float s = sinf( angle );
		const float c = cosf( angle );
		matrix::clear( mtx );
		mtx.x.x = c;
		mtx.x.z = -s;
		mtx.y.y = 1.0f;
		mtx.z.x = s;
		mtx.z.z = c;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::createRotationZ( Matrix33& mtx, float angle )
	{
		const float s = sinf( angle );
		const float c = cosf( angle );
		matrix::clear( mtx );
		mtx.x.x = c;
		mtx.x.y = s;
		mtx.y.x = -s;
		mtx.y.y = c;
		mtx.z.z = 1.0f;
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

	TIKI_FORCE_INLINE Matrix44& matrix::set( Matrix44& mtx, const Matrix33& rot )
	{
		vector::set( mtx.x, rot.x, 0.0f );
		vector::set( mtx.y, rot.y, 0.0f );
		vector::set( mtx.z, rot.z, 0.0f );
		vector::set( mtx.w, 0.0f, 0.0f, 0.0f, 1.0f );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::set( Matrix44& mtx, const Matrix43& rotPos )
	{
		vector::set( mtx.x, rotPos.rot.x, 0.0f );
		vector::set( mtx.y, rotPos.rot.y, 0.0f );
		vector::set( mtx.z, rotPos.rot.z, 0.0f );
		vector::set( mtx.w, rotPos.pos, 1.0f );
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
		const Vector3 x = mtx.x;
		const Vector3 y = mtx.y;
		const Vector3 z = mtx.z;
		mtx.x.x = vector::dot( x, right.x );
		mtx.x.y = vector::dot( x, right.y );
		mtx.x.z = vector::dot( x, right.z );
		mtx.y.x = vector::dot( y, right.x );
		mtx.y.y = vector::dot( y, right.y );
		mtx.y.z = vector::dot( y, right.z );
		mtx.z.x = vector::dot( z, right.x );
		mtx.z.y = vector::dot( z, right.y );
		mtx.z.z = vector::dot( z, right.z );
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
		const Vector4 x = mtx.x;
		const Vector4 y = mtx.y;
		const Vector4 z = mtx.z;
		const Vector4 w = mtx.w;
		mtx.x.x = vector::dot( x, right.x );
		mtx.x.y = vector::dot( x, right.y );
		mtx.x.z = vector::dot( x, right.z );
		mtx.x.w = vector::dot( x, right.w );
		mtx.y.x = vector::dot( y, right.x );
		mtx.y.y = vector::dot( y, right.y );
		mtx.y.z = vector::dot( y, right.z );
		mtx.y.w = vector::dot( y, right.w );
		mtx.z.x = vector::dot( z, right.x );
		mtx.z.y = vector::dot( z, right.y );
		mtx.z.z = vector::dot( z, right.z );
		mtx.z.w = vector::dot( z, right.w );
		mtx.w.x = vector::dot( w, right.x );
		mtx.w.y = vector::dot( w, right.y );
		mtx.w.z = vector::dot( w, right.z );
		mtx.w.w = vector::dot( w, right.w );
		return mtx;
	}

	//TIKI_FORCE_INLINE Matrix33& matrix::div( Matrix33& mtx, const Matrix33& rhs )
	//{

	//}

	//TIKI_FORCE_INLINE Matrix43& matrix::div( Matrix43& mtx, const Matrix43& rhs )
	//{

	//}

	//TIKI_FORCE_INLINE Matrix44& matrix::div( Matrix44& mtx, const Matrix44& rhs )
	//{

	//}

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
		// source: https://github.com/sharpdx/SharpDX/blob/master/Source/SharpDX/Matrix.cs

		const float b0 = (source.z.x * source.w.y) - (source.z.y * source.w.x);
		const float b1 = (source.z.x * source.w.z) - (source.z.z * source.w.x);
		const float b2 = (source.z.w * source.w.x) - (source.z.x * source.w.w);
		const float b3 = (source.z.y * source.w.z) - (source.z.z * source.w.y);
		const float b4 = (source.z.w * source.w.y) - (source.z.y * source.w.w);
		const float b5 = (source.z.z * source.w.w) - (source.z.w * source.w.z);

		const float d11 = source.y.y * b5 + source.y.z * b4 + source.y.w * b3;
		const float d12 = source.y.x * b5 + source.y.z * b2 + source.y.w * b1;
		const float d13 = source.y.x * -b4 + source.y.y * b2 + source.y.w * b0;
		const float d14 = source.y.x * b3 + source.y.y * -b1 + source.y.z * b0;

		const float det = source.x.x * d11 - source.x.y * d12 + source.x.z * d13 - source.x.w * d14;
		if ( f32::abs( det ) == 0.0f)
		{
			mtx = Matrix44::zero;
			return mtx;
		}
		const float inverseDet = 1.0f / det;

		float a0 = (source.x.x * source.y.y) - (source.x.y * source.y.x);
		float a1 = (source.x.x * source.y.z) - (source.x.z * source.y.x);
		float a2 = (source.x.w * source.y.x) - (source.x.x * source.y.w);
		float a3 = (source.x.y * source.y.z) - (source.x.z * source.y.y);
		float a4 = (source.x.w * source.y.y) - (source.x.y * source.y.w);
		float a5 = (source.x.z * source.y.w) - (source.x.w * source.y.z);

		float d21 = source.x.y * b5 + source.x.z * b4 + source.x.w * b3;
		float d22 = source.x.x * b5 + source.x.z * b2 + source.x.w * b1;
		float d23 = source.x.x * -b4 + source.x.y * b2 + source.x.w * b0;
		float d24 = source.x.x * b3 + source.x.y * -b1 + source.x.z * b0;

		float d31 = source.w.y * a5 + source.w.z * a4 + source.w.w * a3;
		float d32 = source.w.x * a5 + source.w.z * a2 + source.w.w * a1;
		float d33 = source.w.x * -a4 + source.w.y * a2 + source.w.w * a0;
		float d34 = source.w.x * a3 + source.w.y * -a1 + source.w.z * a0;

		float d41 = source.z.y * a5 + source.z.z * a4 + source.z.w * a3;
		float d42 = source.z.x * a5 + source.z.z * a2 + source.z.w * a1;
		float d43 = source.z.x * -a4 + source.z.y * a2 + source.z.w * a0;
		float d44 = source.z.x * a3 + source.z.y * -a1 + source.z.z * a0;

		mtx.x.x = +d11 * inverseDet; mtx.x.y = -d21 * inverseDet; mtx.x.z = +d31 * inverseDet; mtx.x.w = -d41 * inverseDet;
		mtx.y.x = -d12 * inverseDet; mtx.y.y = +d22 * inverseDet; mtx.y.z = -d32 * inverseDet; mtx.y.w = +d42 * inverseDet;
		mtx.z.x = +d13 * inverseDet; mtx.z.y = -d23 * inverseDet; mtx.z.z = +d33 * inverseDet; mtx.z.w = -d43 * inverseDet;
		mtx.w.x = -d14 * inverseDet; mtx.w.y = +d24 * inverseDet; mtx.w.z = -d34 * inverseDet; mtx.w.w = +d44 * inverseDet;

		return mtx;
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
			vector::dot( vec4, mtx.x ) + mtx.w.x,
			vector::dot( vec4, mtx.y ) + mtx.w.y,
			vector::dot( vec4, mtx.z ) + mtx.w.z
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


	TIKI_FORCE_INLINE void matrix::transformNormal( Vector3& vec, const Matrix43& mtx )
	{
		// TODO: check if this works
		TIKI_ASSERT( false );
		matrix::transform( vec, mtx.rot );
	}

	TIKI_FORCE_INLINE void matrix::transformCoordinate( Vector3& vec, const Matrix44& mtx )
	{
		float w = 1 / ( ( ( ( vec.x * mtx.x.w ) + ( vec.y * mtx.y.w ) ) + ( vec.z * mtx.z.w ) ) + mtx.w.w );

		vector::set(
			vec,
			( ( ( ( vec.x * mtx.x.x ) + ( vec.y * mtx.y.x ) ) + ( vec.z * mtx.z.x ) ) + mtx.w.x ) * w,
			( ( ( ( vec.x * mtx.x.y ) + ( vec.y * mtx.y.y ) ) + ( vec.z * mtx.z.y ) ) + mtx.w.y ) * w,
			( ( ( ( vec.x * mtx.x.z ) + ( vec.y * mtx.y.z ) ) + ( vec.z * mtx.z.z ) ) + mtx.w.z ) * w
			);
	}

	TIKI_FORCE_INLINE void matrix::project( Vector3& vec, float x, float y, float width, float height, float minZ, float maxZ, const Matrix44& mtx )
	{
		transform( vec, mtx );

		vector::set( vec,
					 ( ( 1.0f + vec.x ) * 0.5f * width ) + x,
					 ( ( 1.0f - vec.y ) * 0.5f * height ) + y,
					 ( vec.z * ( maxZ - minZ ) ) + minZ );
	}

	TIKI_FORCE_INLINE void matrix::unproject( Vector3& vec, float x, float y, float width, float height, float minZ, float maxZ, const Matrix44& mtx )
	{
		Matrix44 inverseMtx;
		invert( inverseMtx, mtx );

		vector::set(vec,
					( ( ( vec.x - x )  / width ) * 2.0f ) - 1.0f,
				   -( ( ( ( vec.y - y ) / height ) * 2.0f ) - 1.0f ),
					( vec.z - minZ ) / ( maxZ - minZ ) );

		transformCoordinate( vec, inverseMtx );
	}

}

#endif // __TIKI_MATRIX_INL_INCLUDED__
