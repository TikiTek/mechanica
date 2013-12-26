#pragma once
#ifndef __TIKI_MATRIX_INL_INCLUDED__
#define __TIKI_MATRIX_INL_INCLUDED__

#include "tiki/base/float32.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"

#include "tiki/math/matrix.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE bool matrix::isEquals( const Matrix33& lhs, const Matrix33& rhs, float epsilon /*= f32::s_epsilon*/ )
	{
		return vector::isEquals( lhs.x, rhs.x, epsilon ) && vector::isEquals( lhs.y, rhs.y, epsilon ) && vector::isEquals( lhs.z, rhs.z, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isEquals( const Matrix43& lhs, const Matrix43& rhs, float epsilon /*= f32::s_epsilon*/ )
	{
		return isEquals( lhs.rot, rhs.rot, epsilon ) && vector::isEquals( lhs.pos, rhs.pos, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isEquals( const Matrix44& lhs, const Matrix44& rhs, float epsilon /*= f32::s_epsilon*/ )
	{
		return vector::isEquals( lhs.x, rhs.x, epsilon ) && vector::isEquals( lhs.y, rhs.y, epsilon ) && vector::isEquals( lhs.z, rhs.z, epsilon ) && vector::isEquals( lhs.w, rhs.w, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isIdentity( const Matrix33& mtx, float epsilon /*= f32::s_epsilon*/ )
	{

	}

	TIKI_FORCE_INLINE bool matrix::isIdentity( const Matrix43& mtx, float epsilon /*= f32::s_epsilon*/ )
	{

	}

	TIKI_FORCE_INLINE bool matrix::isIdentity( const Matrix44& mtx, float epsilon /*= f32::s_epsilon*/ )
	{

	}

	TIKI_FORCE_INLINE bool matrix::isZero( const Matrix33& mtx, float epsilon /*= f32::s_epsilon*/ )
	{
		return vector::isZero( mtx.x, epsilon ) && vector::isZero( mtx.y, epsilon ) && vector::isZero( mtx.z, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isZero( const Matrix43& mtx, float epsilon /*= f32::s_epsilon*/ )
	{
		return isZero( mtx.rot, epsilon ) && vector::isZero( mtx.pos, epsilon );
	}

	TIKI_FORCE_INLINE bool matrix::isZero( const Matrix44& mtx, float epsilon /*= f32::s_epsilon*/ )
	{
		return vector::isZero( mtx.x, epsilon ) && vector::isZero( mtx.y, epsilon ) && vector::isZero( mtx.z, epsilon ) && vector::isZero( mtx.w, epsilon );
	}

	TIKI_FORCE_INLINE void matrix::clear( Matrix33& mtx )
	{
		vector::clear( mtx.x );
		vector::clear( mtx.y );
		vector::clear( mtx.z );
	}

	TIKI_FORCE_INLINE void matrix::clear( Matrix43& mtx )
	{
		matrix::clear( mtx.rot );
		vector::clear( mtx.pos );
	}

	TIKI_FORCE_INLINE void matrix::clear( Matrix44& mtx )
	{
		vector::clear( mtx.x );
		vector::clear( mtx.y );
		vector::clear( mtx.z );
		vector::clear( mtx.w );
	}

	TIKI_FORCE_INLINE void matrix::createIdentity( Matrix33& mtx )
	{

	}

	TIKI_FORCE_INLINE void matrix::createIdentity( Matrix43& mtx )
	{

	}

	TIKI_FORCE_INLINE void matrix::createIdentity( Matrix44& mtx )
	{

	}

	TIKI_FORCE_INLINE void matrix::set( Matrix33& mtx, const Vector3& x, const Vector3& y, const Vector3& z )
	{

	}

	TIKI_FORCE_INLINE void matrix::set( Matrix43& mtx, const Matrix33& rot, const Vector3& pos )
	{

	}

	TIKI_FORCE_INLINE void matrix::set( Matrix44& mtx, const Matrix43& rotPos )
	{

	}

	TIKI_FORCE_INLINE void matrix::set( Matrix44& mtx, const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w )
	{

	}

	TIKI_FORCE_INLINE void matrix::add( Matrix33& mtx, const Matrix33& rhs )
	{

	}

	TIKI_FORCE_INLINE void matrix::add( Matrix43& mtx, const Matrix43& rhs )
	{

	}

	TIKI_FORCE_INLINE void matrix::add( Matrix44& mtx, const Matrix44& rhs )
	{

	}

	TIKI_FORCE_INLINE void matrix::sub( Matrix33& mtx, const Matrix33& rhs )
	{

	}

	TIKI_FORCE_INLINE void matrix::sub( Matrix43& mtx, const Matrix43& rhs )
	{

	}

	TIKI_FORCE_INLINE void matrix::sub( Matrix44& mtx, const Matrix44& rhs )
	{

	}

	TIKI_FORCE_INLINE void matrix::mul( Matrix33& mtx, const Matrix33& rhs )
	{

	}

	TIKI_FORCE_INLINE void matrix::mul( Matrix43& mtx, const Matrix43& rhs )
	{

	}

	TIKI_FORCE_INLINE void matrix::mul( Matrix44& mtx, const Matrix44& rhs )
	{

	}

	TIKI_FORCE_INLINE void matrix::div( Matrix33& mtx, const Matrix33& rhs )
	{

	}

	TIKI_FORCE_INLINE void matrix::div( Matrix43& mtx, const Matrix43& rhs )
	{

	}

	TIKI_FORCE_INLINE void matrix::div( Matrix44& mtx, const Matrix44& rhs )
	{

	}

	TIKI_FORCE_INLINE void matrix::scale( Matrix33& mtx, float val )
	{

	}

	TIKI_FORCE_INLINE void matrix::scale( Matrix43& mtx, float val )
	{

	}

	TIKI_FORCE_INLINE void matrix::scale( Matrix44& mtx, float val )
	{

	}

	TIKI_FORCE_INLINE void matrix::invert( Matrix33& mtx, const Matrix33& source )
	{

	}

	TIKI_FORCE_INLINE void matrix::invert( Matrix43& mtx, const Matrix43& source )
	{

	}

	TIKI_FORCE_INLINE void matrix::invert( Matrix44& mtx, const Matrix44& source )
	{

	}

	TIKI_FORCE_INLINE void matrix::transpose( Matrix33& mtx )
	{

	}

	TIKI_FORCE_INLINE void matrix::transpose( Matrix44& mtx )
	{

	}

	TIKI_FORCE_INLINE void matrix::transform( Vector3& vec, const Matrix33& mtx )
	{

	}

	TIKI_FORCE_INLINE void matrix::transform( Vector3& vec, const Matrix43& mtx )
	{

	}

	TIKI_FORCE_INLINE void matrix::transform( Vector3& vec, const Matrix44& mtx )
	{

	}

	TIKI_FORCE_INLINE void matrix::transform( Vector4& vec, const Matrix44& mtx )
	{

	}

	TIKI_FORCE_INLINE void matrix::lerp( Matrix33& mtx, const Matrix33& start, const Matrix33& end, const float amount )
	{

	}

	TIKI_FORCE_INLINE void matrix::lerp( Matrix43& mtx, const Matrix43& start, const Matrix43& end, const float amount )
	{

	}

	TIKI_FORCE_INLINE void matrix::lerp( Matrix44& mtx, const Matrix44& start, const Matrix44& end, const float amount )
	{

	}

}

#endif // __TIKI_MATRIX_INL_INCLUDED__
