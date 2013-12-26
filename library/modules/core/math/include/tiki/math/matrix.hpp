#pragma once
#ifndef __TIKI_MATRIX_HPP_INCLUDED__
#define __TIKI_MATRIX_HPP_INCLUDED__

#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Quaternion;

	TIKI_PRE_ALIGN( 16 ) struct Matrix33
	{
		Vector3 x;
		Vector3 y;
		Vector3 z;

		static const Matrix33 zero;
		static const Matrix33 one;
		static const Matrix33 identity;
	}
	TIKI_POST_ALIGN( 16);

	TIKI_PRE_ALIGN( 16 ) struct Matrix43
	{
		Matrix33	rot;
		Vector3		pos;

		static const Matrix43 zero;
		static const Matrix43 one;
		static const Matrix43 identity;
	}
	TIKI_POST_ALIGN( 16);

	TIKI_PRE_ALIGN( 16 ) struct Matrix44
	{
		Vector4 x;
		Vector4 y;
		Vector4 z;
		Vector4 w;

		static const Matrix44 zero;
		static const Matrix44 one;
		static const Matrix44 identity;
	}
	TIKI_POST_ALIGN( 16);

	namespace matrix
	{
		TIKI_FORCE_INLINE bool		isEquals( const Matrix33& lhs, const Matrix33& rhs, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool		isEquals( const Matrix43& lhs, const Matrix43& rhs, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool		isEquals( const Matrix44& lhs, const Matrix44& rhs, float epsilon = f32::epsilon );
		
		TIKI_FORCE_INLINE bool		isIdentity( const Matrix33& mtx, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool		isIdentity( const Matrix43& mtx, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool		isIdentity( const Matrix44& mtx, float epsilon = f32::epsilon );

		TIKI_FORCE_INLINE bool		isZero( const Matrix33& mtx, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool		isZero( const Matrix43& mtx, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool		isZero( const Matrix44& mtx, float epsilon = f32::epsilon );

		TIKI_FORCE_INLINE Matrix33&	clear( Matrix33& mtx );
		TIKI_FORCE_INLINE Matrix43&	clear( Matrix43& mtx );
		TIKI_FORCE_INLINE Matrix44&	clear( Matrix44& mtx );
		TIKI_FORCE_INLINE Matrix33&	createIdentity( Matrix33& mtx );
		TIKI_FORCE_INLINE Matrix43&	createIdentity( Matrix43& mtx );
		TIKI_FORCE_INLINE Matrix44&	createIdentity( Matrix44& mtx );
		TIKI_FORCE_INLINE Matrix33&	createScale( Matrix33& mtx, const Vector3& scale );
		TIKI_FORCE_INLINE Matrix33&	createRotationX( Matrix33& mtx, float angle );
		TIKI_FORCE_INLINE Matrix33&	createRotationY( Matrix33& mtx, float angle );
		TIKI_FORCE_INLINE Matrix33&	createRotationZ( Matrix33& mtx, float angle );

		TIKI_FORCE_INLINE Matrix33&	set( Matrix33& mtx, const Vector3& x, const Vector3& y, const Vector3& z );
		TIKI_FORCE_INLINE Matrix43&	set( Matrix43& mtx, const Matrix33& rot, const Vector3& pos );
		TIKI_FORCE_INLINE Matrix44&	set( Matrix44& mtx, const Matrix43& rotPos );
		TIKI_FORCE_INLINE Matrix44&	set( Matrix44& mtx, const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w );

		TIKI_FORCE_INLINE Matrix33&	add( Matrix33& mtx, const Matrix33& rhs );
		TIKI_FORCE_INLINE Matrix43&	add( Matrix43& mtx, const Matrix43& rhs );
		TIKI_FORCE_INLINE Matrix44&	add( Matrix44& mtx, const Matrix44& rhs );

		TIKI_FORCE_INLINE Matrix33&	sub( Matrix33& mtx, const Matrix33& rhs );
		TIKI_FORCE_INLINE Matrix43&	sub( Matrix43& mtx, const Matrix43& rhs );
		TIKI_FORCE_INLINE Matrix44&	sub( Matrix44& mtx, const Matrix44& rhs );

		TIKI_FORCE_INLINE Matrix33&	mul( Matrix33& mtx, const Matrix33& rhs );
		TIKI_FORCE_INLINE Matrix43&	mul( Matrix43& mtx, const Matrix43& rhs );
		TIKI_FORCE_INLINE Matrix44&	mul( Matrix44& mtx, const Matrix44& rhs );

		TIKI_FORCE_INLINE Matrix33&	div( Matrix33& mtx, const Matrix33& rhs );
		TIKI_FORCE_INLINE Matrix43&	div( Matrix43& mtx, const Matrix43& rhs );
		TIKI_FORCE_INLINE Matrix44&	div( Matrix44& mtx, const Matrix44& rhs );

		TIKI_FORCE_INLINE Matrix33&	scale( Matrix33& mtx, float val );
		TIKI_FORCE_INLINE Matrix43&	scale( Matrix43& mtx, float val );
		TIKI_FORCE_INLINE Matrix44&	scale( Matrix44& mtx, float val );

		TIKI_FORCE_INLINE Matrix33&	invert( Matrix33& mtx, const Matrix33& source );
		TIKI_FORCE_INLINE Matrix43&	invert( Matrix43& mtx, const Matrix43& source );
		TIKI_FORCE_INLINE Matrix44&	invert( Matrix44& mtx, const Matrix44& source );

		TIKI_FORCE_INLINE Matrix33&	lerp( Matrix33& mtx, const Matrix33& start, const Matrix33& end, const float amount );
		TIKI_FORCE_INLINE Matrix43&	lerp( Matrix43& mtx, const Matrix43& start, const Matrix43& end, const float amount );
		TIKI_FORCE_INLINE Matrix44&	lerp( Matrix44& mtx, const Matrix44& start, const Matrix44& end, const float amount );

		TIKI_FORCE_INLINE Matrix33&	transpose( Matrix33& mtx );
		TIKI_FORCE_INLINE Matrix44&	transpose( Matrix44& mtx );

		TIKI_FORCE_INLINE void		transform( Vector3& vec, const Matrix33& mtx );
		TIKI_FORCE_INLINE void		transform( Vector3& vec, const Matrix43& mtx );
		TIKI_FORCE_INLINE void		transform( Vector3& vec, const Matrix44& mtx );
		TIKI_FORCE_INLINE void		transform( Vector4& vec, const Matrix44& mtx );

		void						compose( Matrix44& target, const Quaternion& rotation, const Vector3& translation, const Vector3& scale );
		bool						decompose( Quaternion& rotation, Vector3& translation, Vector3& scale, const Matrix44& source );
	}
}

#include "../../../source/matrix.inl"

#endif // __TIKI_MATRIX_HPP_INCLUDED__
