#pragma once
#ifndef __TIKI_MATRIX_INL_INCLUDED__
#define __TIKI_MATRIX_INL_INCLUDED__

#include "tiki/base/float32.hpp"
#include "tiki/base/types.hpp"

#include "tiki/math/matrix.hpp"

namespace tiki
{
	//////////////////////////////////////////////////////////////////////////
	// isEquals

	TIKI_FORCE_INLINE bool matrix::isEquals( const Matrix22& lhs, const Matrix22& rhs, float epsilon /*= f32::epsilon */ )
	{
		return vector::isEquals( lhs.x, rhs.x, epsilon ) && vector::isEquals( lhs.y, rhs.y, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isEquals( const Matrix32& lhs, const Matrix32& rhs, float epsilon /*= f32::epsilon */ )
	{
		return matrix::isEquals( lhs.rot, rhs.rot, epsilon ) && vector::isEquals( lhs.pos, rhs.pos, epsilon );
	}

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

	//////////////////////////////////////////////////////////////////////////
	// isIdentity

	TIKI_FORCE_INLINE bool matrix::isIdentity( const Matrix22& mtx, float epsilon /*= f32::epsilon */ )
	{
		return vector::isEquals( mtx.x, Vector2::unitX, epsilon ) && vector::isEquals( mtx.y, Vector2::unitY, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isIdentity( const Matrix32& mtx, float epsilon /*= f32::epsilon */ )
	{
		return matrix::isIdentity( mtx.rot, epsilon ) && vector::isZero( mtx.pos, epsilon );
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

	//////////////////////////////////////////////////////////////////////////
	// isZero

	TIKI_FORCE_INLINE bool matrix::isZero( const Matrix22& mtx, float epsilon /*= f32::epsilon */ )
	{
		return vector::isZero( mtx.x, epsilon ) && vector::isZero( mtx.y, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isZero( const Matrix32& mtx, float epsilon /*= f32::epsilon */ )
	{
		return matrix::isZero( mtx.rot, epsilon ) && vector::isZero( mtx.pos, epsilon );
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

	//////////////////////////////////////////////////////////////////////////
	// clear

	TIKI_FORCE_INLINE Matrix22& matrix::clear( Matrix22& mtx )
	{
		vector::clear( mtx.x );
		vector::clear( mtx.y );
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix32& matrix::clear( Matrix32& mtx )
	{
		matrix::clear( mtx.rot );
		vector::clear( mtx.pos );
		return mtx;
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

	//////////////////////////////////////////////////////////////////////////
	// createIdentity

	TIKI_FORCE_INLINE Matrix22& matrix::createIdentity( Matrix22& mtx )
	{
		mtx.x = Vector2::unitX;
		mtx.y = Vector2::unitY;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix32& matrix::createIdentity( Matrix32& mtx )
	{
		matrix::createIdentity( mtx.rot );
		vector::clear( mtx.pos );
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

	//////////////////////////////////////////////////////////////////////////
	// createScale
	
	TIKI_FORCE_INLINE Matrix22& matrix::createScale( Matrix22& mtx, const Vector2& scale )
	{
		matrix::clear( mtx );
		mtx.x.x = scale.x;
		mtx.y.y = scale.y;
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

	TIKI_FORCE_INLINE Matrix44& matrix::createScale( Matrix44& mtx, const Vector3& scale )
	{
		matrix::clear( mtx );
		mtx.x.x = scale.x;
		mtx.y.y = scale.y;
		mtx.z.z = scale.z;
		mtx.w.w = 1.0f;
		return mtx;
	}

	//////////////////////////////////////////////////////////////////////////
	// createRotationX
	
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

	TIKI_FORCE_INLINE Matrix44& matrix::createRotationX( Matrix44& mtx, float angle )
	{
		const float s = sinf( angle );
		const float c = cosf( angle );
		matrix::clear( mtx );
		mtx.x.x = 1.0f;
		mtx.y.y = c;
		mtx.y.z = s;
		mtx.z.y = -s;
		mtx.z.z = c;
		mtx.w.w = 1.0f;
		return mtx;
	}

	//////////////////////////////////////////////////////////////////////////
	// createRotationY

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

	TIKI_FORCE_INLINE Matrix44& matrix::createRotationY( Matrix44& mtx, float angle )
	{
		const float s = sinf( angle );
		const float c = cosf( angle );
		matrix::clear( mtx );
		mtx.x.x = c;
		mtx.x.z = -s;
		mtx.y.y = 1.0f;
		mtx.z.x = s;
		mtx.z.z = c;
		mtx.w.w = 1.0f;
		return mtx;
	}

	//////////////////////////////////////////////////////////////////////////
	// createRotationZ

	TIKI_FORCE_INLINE Matrix22& matrix::createRotationZ( Matrix22& mtx, float angle )
	{
		const float s = sinf( angle );
		const float c = cosf( angle );
		matrix::clear( mtx );
		mtx.x.x = c;
		mtx.x.y = s;
		mtx.y.x = -s;
		mtx.y.y = c;
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

	TIKI_FORCE_INLINE Matrix44& matrix::createRotationZ( Matrix44& mtx, float angle )
	{
		const float s = sinf( angle );
		const float c = cosf( angle );
		matrix::clear( mtx );
		mtx.x.x = c;
		mtx.x.y = s;
		mtx.y.x = -s;
		mtx.y.y = c;
		mtx.z.z = 1.0f;
		mtx.w.w = 1.0f;
		return mtx;
	}

	//////////////////////////////////////////////////////////////////////////
	// createTranslation

	TIKI_FORCE_INLINE Matrix32& matrix::createTranslation( Matrix32& mtx, const Vector2& position )
	{
		matrix::createIdentity( mtx.rot );
		mtx.pos = position;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix43& matrix::createTranslation( Matrix43& mtx, const Vector3& position )
	{
		matrix::createIdentity( mtx.rot );
		mtx.pos = position;
		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::createTranslation( Matrix44& mtx, const Vector3& position )
	{
		matrix::createIdentity( mtx );
		vector::set( mtx.w, position, 1.0f );
		return mtx;
	}

	//////////////////////////////////////////////////////////////////////////
	// set

	TIKI_FORCE_INLINE Matrix32& matrix::set( Matrix32& mtx, const Matrix22& rot, const Vector2& pos )
	{
		mtx.rot = rot;
		mtx.pos = pos;
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

	//////////////////////////////////////////////////////////////////////////
	// mul

	TIKI_FORCE_INLINE Matrix22& matrix::mul( Matrix22& mtx, const Matrix22& rhs )
	{
		return mul( mtx, mtx, rhs );
	}


	TIKI_FORCE_INLINE Matrix32& matrix::mul( Matrix32& mtx, const Matrix32& rhs )
	{
		return mul( mtx, mtx, rhs );
	}

	TIKI_FORCE_INLINE Matrix33& matrix::mul( Matrix33& mtx, const Matrix33& rhs )
	{
		return mul( mtx, mtx, rhs );
	}

	TIKI_FORCE_INLINE Matrix43& matrix::mul( Matrix43& mtx, const Matrix43& rhs )
	{
		return mul( mtx, mtx, rhs );
	}

	TIKI_FORCE_INLINE Matrix44& matrix::mul( Matrix44& mtx, const Matrix44& rhs )
	{
		return mul( mtx, mtx, rhs );
	}

	TIKI_FORCE_INLINE Matrix22& matrix::mul( Matrix22& mtx, const Matrix22& lhs, const Matrix22& rhs )
	{
		Matrix22 rhsTransposed;
		matrix::transpose( rhsTransposed, rhs );

		mtx = lhs;
		matrix::transform( mtx.x, rhsTransposed );
		matrix::transform( mtx.y, rhsTransposed );

		return mtx;
	}

	TIKI_FORCE_INLINE Matrix32& matrix::mul( Matrix32& mtx, const Matrix32& lhs, const Matrix32& rhs )
	{
		Matrix22 rhsTransposed;
		matrix::transpose( rhsTransposed, rhs.rot );

		mtx = lhs;
		matrix::transform( mtx.rot.x, rhsTransposed );
		matrix::transform( mtx.rot.y, rhsTransposed );

		matrix::transform( mtx.pos, rhsTransposed );
		vector::add( mtx.pos, rhs.pos );

		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::mul( Matrix33& mtx, const Matrix33& lhs, const Matrix33& rhs )
	{
		Matrix33 rhsTransposed;
		matrix::transpose( rhsTransposed, rhs );

		mtx = lhs;
		matrix::transform( mtx.x, rhsTransposed );
		matrix::transform( mtx.y, rhsTransposed );
		matrix::transform( mtx.z, rhsTransposed );

		return mtx;
	}

	TIKI_FORCE_INLINE Matrix43& matrix::mul( Matrix43& mtx, const Matrix43& lhs, const Matrix43& rhs )
	{
		Matrix33 rhsTransposed;
		matrix::transpose( rhsTransposed, rhs.rot );

		mtx = lhs;
		matrix::transform( mtx.rot.x, rhsTransposed );
		matrix::transform( mtx.rot.y, rhsTransposed );
		matrix::transform( mtx.rot.z, rhsTransposed );
		
		matrix::transform( mtx.pos, rhsTransposed );
		vector::add( mtx.pos, rhs.pos );

		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::mul( Matrix44& mtx, const Matrix44& lhs, const Matrix44& rhs )
	{
		Matrix44 rhsTransposed;
		transpose( rhsTransposed, rhs );

		mtx = lhs;
		matrix::transform( mtx.x, rhsTransposed );
		matrix::transform( mtx.y, rhsTransposed );
		matrix::transform( mtx.z, rhsTransposed );
		matrix::transform( mtx.w, rhsTransposed );

		return mtx;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// scale

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

	//////////////////////////////////////////////////////////////////////////
	// invert

	TIKI_FORCE_INLINE bool matrix::invert( Matrix22& mtx, const Matrix22& source )
	{
		const float det = source.x.x * source.y.y - source.x.y * source.y.x;
		if( f32::isZero( det ) )
		{
			return false;
		}

		mtx.x.x = source.y.y;
		mtx.x.y = -source.x.y;
		mtx.y.x = -source.y.x;
		mtx.y.y = source.x.x;

		scale( mtx, 1.0f / det );

		return true;
	}

	TIKI_FORCE_INLINE bool matrix::invert( Matrix33& mtx, const Matrix33& source )
	{
		const float det =	source.x.x * source.y.y * source.z.z +
							source.x.y * source.y.z * source.z.x +
							source.x.z * source.y.x * source.z.y -
							source.z.x * source.y.y * source.x.z -
							source.z.y * source.y.z * source.x.x -
							source.z.z * source.y.x * source.x.y;
		if( f32::isZero( det ) )
		{
			return false;
		}

		mtx.x.x = source.y.y * source.z.z - source.y.z * source.z.y;
		mtx.x.y = source.x.z * source.z.y - source.x.y * source.z.z;
		mtx.x.z = source.x.y * source.y.z - source.x.z * source.y.y;
		mtx.y.x = source.y.z * source.z.x - source.y.x * source.z.z;
		mtx.y.y = source.x.x * source.z.z - source.x.z * source.z.x;
		mtx.y.z = source.x.z * source.y.x - source.x.x * source.y.z;
		mtx.z.x = source.y.x * source.z.y - source.y.y * source.z.x;
		mtx.z.y = source.x.y * source.z.x - source.x.x * source.z.y;
		mtx.z.z = source.x.x * source.y.y - source.x.y * source.y.x;

		scale( mtx, 1.0f / det );

		return true;
	}

	TIKI_FORCE_INLINE bool matrix::invert( Matrix44& mtx, const Matrix44& source )
	{
		// source: https://github.com/sharpdx/SharpDX/blob/master/Source/SharpDX.Mathematics/Matrix.cs

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
		if( f32::isZero( det ) )
		{
			return false;
		}

		const float inverseDet = 1.0f / det;

		const float a0 = (source.x.x * source.y.y) - (source.x.y * source.y.x);
		const float a1 = (source.x.x * source.y.z) - (source.x.z * source.y.x);
		const float a2 = (source.x.w * source.y.x) - (source.x.x * source.y.w);
		const float a3 = (source.x.y * source.y.z) - (source.x.z * source.y.y);
		const float a4 = (source.x.w * source.y.y) - (source.x.y * source.y.w);
		const float a5 = (source.x.z * source.y.w) - (source.x.w * source.y.z);

		const float d21 = source.x.y * b5 + source.x.z * b4 + source.x.w * b3;
		const float d22 = source.x.x * b5 + source.x.z * b2 + source.x.w * b1;
		const float d23 = source.x.x * -b4 + source.x.y * b2 + source.x.w * b0;
		const float d24 = source.x.x * b3 + source.x.y * -b1 + source.x.z * b0;

		const float d31 = source.w.y * a5 + source.w.z * a4 + source.w.w * a3;
		const float d32 = source.w.x * a5 + source.w.z * a2 + source.w.w * a1;
		const float d33 = source.w.x * -a4 + source.w.y * a2 + source.w.w * a0;
		const float d34 = source.w.x * a3 + source.w.y * -a1 + source.w.z * a0;

		const float d41 = source.z.y * a5 + source.z.z * a4 + source.z.w * a3;
		const float d42 = source.z.x * a5 + source.z.z * a2 + source.z.w * a1;
		const float d43 = source.z.x * -a4 + source.z.y * a2 + source.z.w * a0;
		const float d44 = source.z.x * a3 + source.z.y * -a1 + source.z.z * a0;

		mtx.x.x = +d11 * inverseDet; mtx.x.y = -d21 * inverseDet; mtx.x.z = +d31 * inverseDet; mtx.x.w = -d41 * inverseDet;
		mtx.y.x = -d12 * inverseDet; mtx.y.y = +d22 * inverseDet; mtx.y.z = -d32 * inverseDet; mtx.y.w = +d42 * inverseDet;
		mtx.z.x = +d13 * inverseDet; mtx.z.y = -d23 * inverseDet; mtx.z.z = +d33 * inverseDet; mtx.z.w = -d43 * inverseDet;
		mtx.w.x = -d14 * inverseDet; mtx.w.y = +d24 * inverseDet; mtx.w.z = -d34 * inverseDet; mtx.w.w = +d44 * inverseDet;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// lerp

	TIKI_FORCE_INLINE Matrix22& matrix::lerp( Matrix22& mtx, const Matrix22& start, const Matrix22& end, const float amount )
	{
		vector::lerp( mtx.x, start.x, end.x, amount );
		vector::lerp( mtx.y, start.y, end.y, amount );

		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::lerp( Matrix33& mtx, const Matrix33& start, const Matrix33& end, const float amount )
	{
		vector::lerp( mtx.x, start.x, end.x, amount );
		vector::lerp( mtx.y, start.y, end.y, amount );
		vector::lerp( mtx.z, start.z, end.z, amount );

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

	//////////////////////////////////////////////////////////////////////////
	// transpose

	TIKI_FORCE_INLINE Matrix22& matrix::transpose( Matrix22& mtx )
	{
		return matrix::transpose( mtx, mtx );
	}

	TIKI_FORCE_INLINE Matrix33& matrix::transpose( Matrix33& mtx )
	{
		return matrix::transpose( mtx, mtx );
	}

	TIKI_FORCE_INLINE Matrix44& matrix::transpose( Matrix44& mtx )
	{
		return matrix::transpose( mtx, mtx );
	}

	TIKI_FORCE_INLINE Matrix22& matrix::transpose( Matrix22& mtx, const Matrix22& rhs )
	{
		const Vector2 x = rhs.x;
		const Vector2 y = rhs.y;

		mtx.x.x = x.x;
		mtx.x.y = y.x;
		mtx.y.x = x.y;
		mtx.y.y = y.y;

		return mtx;
	}

	TIKI_FORCE_INLINE Matrix33& matrix::transpose( Matrix33& mtx, const Matrix33& rhs )
	{
		const Vector3 x = rhs.x;
		const Vector3 y = rhs.y;
		const Vector3 z = rhs.z;

		mtx.x.x = x.x;
		mtx.x.y = y.x;
		mtx.x.z = z.x;
		mtx.y.x = x.y;
		mtx.y.y = y.y;
		mtx.y.z = z.y;
		mtx.z.x = x.z;
		mtx.z.y = y.z;
		mtx.z.z = z.z;

		return mtx;
	}

	TIKI_FORCE_INLINE Matrix44& matrix::transpose( Matrix44& mtx, const Matrix44& rhs )
	{
		const Vector4 x = rhs.x;
		const Vector4 y = rhs.y;
		const Vector4 z = rhs.z;
		const Vector4 w = rhs.w;

		mtx.x.x = x.x;
		mtx.x.y = y.x;
		mtx.x.z = z.x;
		mtx.x.w = w.x;
		mtx.y.x = x.y;
		mtx.y.y = y.y;
		mtx.y.z = z.y;
		mtx.y.w = w.y;
		mtx.z.x = x.z;
		mtx.z.y = y.z;
		mtx.z.z = z.z;
		mtx.z.w = w.z;
		mtx.w.x = x.w;
		mtx.w.y = y.w;
		mtx.w.z = z.w;
		mtx.w.w = w.w;

		return mtx;
	}

	//////////////////////////////////////////////////////////////////////////
	// transform

	TIKI_FORCE_INLINE void matrix::transform( Vector2& vec, const Matrix22& mtx )
	{
		vector::set(
			vec,
			vector::dot( vec, mtx.x ),
			vector::dot( vec, mtx.y )
		);
	}

	TIKI_FORCE_INLINE void matrix::transform( Vector2& vec, const Matrix32& mtx )
	{
		matrix::transform( vec, mtx.rot );
		vector::add( vec, mtx.pos );
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
