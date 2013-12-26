#pragma once
#ifndef __TIKI_MATRIX_HPP_INCLUDED__
#define __TIKI_MATRIX_HPP_INCLUDED__

#include "tiki/math/vector.hpp"

namespace tiki
{
	TIKI_PRE_ALIGN( 16 ) struct Matrix33
	{
		Vector3 x;
		Vector3 y;
		Vector3 z;
	}
	TIKI_POST_ALIGN( 16);

	TIKI_PRE_ALIGN( 16 ) struct Matrix43
	{
		Matrix33	rot;
		Vector3		pos;
	}
	TIKI_POST_ALIGN( 16);

	TIKI_PRE_ALIGN( 16 ) struct Matrix44
	{
		Vector4 x;
		Vector4 y;
		Vector4 z;
		Vector4 w;
	}
	TIKI_POST_ALIGN( 16);

	namespace matrix
	{
		TIKI_FORCE_INLINE bool	isEquals( const Matrix33& lhs, const Matrix33& rhs, float epsilon = f32::s_epsilon );
		TIKI_FORCE_INLINE bool	isEquals( const Matrix43& lhs, const Matrix43& rhs, float epsilon = f32::s_epsilon );
		TIKI_FORCE_INLINE bool	isEquals( const Matrix44& lhs, const Matrix44& rhs, float epsilon = f32::s_epsilon );
		
		TIKI_FORCE_INLINE bool	isIdentity( const Matrix33& mtx, float epsilon = f32::s_epsilon );
		TIKI_FORCE_INLINE bool	isIdentity( const Matrix43& mtx, float epsilon = f32::s_epsilon );
		TIKI_FORCE_INLINE bool	isIdentity( const Matrix44& mtx, float epsilon = f32::s_epsilon );

		TIKI_FORCE_INLINE bool	isZero( const Matrix33& mtx, float epsilon = f32::s_epsilon );
		TIKI_FORCE_INLINE bool	isZero( const Matrix43& mtx, float epsilon = f32::s_epsilon );
		TIKI_FORCE_INLINE bool	isZero( const Matrix44& mtx, float epsilon = f32::s_epsilon );

		TIKI_FORCE_INLINE void	clear( Matrix33& mtx );
		TIKI_FORCE_INLINE void	clear( Matrix43& mtx );
		TIKI_FORCE_INLINE void	clear( Matrix44& mtx );
		TIKI_FORCE_INLINE void	createIdentity( Matrix33& mtx );
		TIKI_FORCE_INLINE void	createIdentity( Matrix43& mtx );
		TIKI_FORCE_INLINE void	createIdentity( Matrix44& mtx );

		TIKI_FORCE_INLINE void	set( Matrix33& mtx, const Vector3& x, const Vector3& y, const Vector3& z );
		TIKI_FORCE_INLINE void	set( Matrix43& mtx, const Matrix33& rot, const Vector3& pos );
		TIKI_FORCE_INLINE void	set( Matrix44& mtx, const Matrix43& rotPos );
		TIKI_FORCE_INLINE void	set( Matrix44& mtx, const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w );

		TIKI_FORCE_INLINE void	add( Matrix33& mtx, const Matrix33& rhs );
		TIKI_FORCE_INLINE void	add( Matrix43& mtx, const Matrix43& rhs );
		TIKI_FORCE_INLINE void	add( Matrix44& mtx, const Matrix44& rhs );

		TIKI_FORCE_INLINE void	sub( Matrix33& mtx, const Matrix33& rhs );
		TIKI_FORCE_INLINE void	sub( Matrix43& mtx, const Matrix43& rhs );
		TIKI_FORCE_INLINE void	sub( Matrix44& mtx, const Matrix44& rhs );

		TIKI_FORCE_INLINE void	mul( Matrix33& mtx, const Matrix33& rhs );
		TIKI_FORCE_INLINE void	mul( Matrix43& mtx, const Matrix43& rhs );
		TIKI_FORCE_INLINE void	mul( Matrix44& mtx, const Matrix44& rhs );

		TIKI_FORCE_INLINE void	div( Matrix33& mtx, const Matrix33& rhs );
		TIKI_FORCE_INLINE void	div( Matrix43& mtx, const Matrix43& rhs );
		TIKI_FORCE_INLINE void	div( Matrix44& mtx, const Matrix44& rhs );

		TIKI_FORCE_INLINE void	scale( Matrix33& mtx, float val );
		TIKI_FORCE_INLINE void	scale( Matrix43& mtx, float val );
		TIKI_FORCE_INLINE void	scale( Matrix44& mtx, float val );

		TIKI_FORCE_INLINE void	invert( Matrix33& mtx, const Matrix33& source );
		TIKI_FORCE_INLINE void	invert( Matrix43& mtx, const Matrix43& source );
		TIKI_FORCE_INLINE void	invert( Matrix44& mtx, const Matrix44& source );

		TIKI_FORCE_INLINE void	transpose( Matrix33& mtx );
		TIKI_FORCE_INLINE void	transpose( Matrix44& mtx );

		TIKI_FORCE_INLINE void	transform( Vector3& vec, const Matrix33& mtx );
		TIKI_FORCE_INLINE void	transform( Vector3& vec, const Matrix43& mtx );
		TIKI_FORCE_INLINE void	transform( Vector3& vec, const Matrix44& mtx );
		TIKI_FORCE_INLINE void	transform( Vector4& vec, const Matrix44& mtx );

		TIKI_FORCE_INLINE void	lerp( Matrix33& mtx, const Matrix33& start, const Matrix33& end, const float amount );
		TIKI_FORCE_INLINE void	lerp( Matrix43& mtx, const Matrix43& start, const Matrix43& end, const float amount );
		TIKI_FORCE_INLINE void	lerp( Matrix44& mtx, const Matrix44& start, const Matrix44& end, const float amount );
	}
}

#include "matrix.inl"

#endif // __TIKI_MATRIX_HPP_INCLUDED__
