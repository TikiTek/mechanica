#pragma once
#ifndef TIKI_MATRIX_INL
#define TIKI_MATRIX_INL

namespace tiki
{
	TIKI_FORCE_INLINE Matrix::Matrix()
		: M11( 1.0f ), M21( 0.0f ), M31( 0.0f ), M41( 0.0f ),
		  M12( 0.0f ), M22( 1.0f ), M32( 0.0f ), M42( 0.0f ),
		  M13( 0.0f ), M23( 0.0f ), M33( 1.0f ), M43( 0.0f ),
		  M14( 0.0f ), M24( 0.0f ), M34( 0.0f ), M44( 1.0f )
	{
	}

	TIKI_FORCE_INLINE Matrix::Matrix ( float m11, float m21, float m31, float m41,
		float m12, float m22, float m32, float m42,
		float m13, float m23, float m33, float m43,
		float m14, float m24, float m34, float m44 )
		: M11( m11 ), M21( m21 ), M31( m31 ), M41( m41 ),
		  M12( m12 ), M22( m22 ), M32( m32 ), M42( m42 ),
		  M13( m13 ), M23( m23 ), M33( m33 ), M43( m43 ),
		  M14( m14 ), M24( m24 ), M34( m34 ), M44( m44 )
	{						   
	}

	TIKI_FORCE_INLINE Matrix::Matrix( const float *arr )
		: M11( arr[0] ),  M21( arr[1] ),  M31( arr[2] ),  M41( arr[3] ),
		  M12( arr[4] ),  M22( arr[5] ),  M32( arr[6] ),  M42( arr[7] ),
		  M13( arr[8] ),  M23( arr[9] ),  M33( arr[10] ), M43( arr[11] ),
		  M14( arr[12] ), M24( arr[13] ), M34( arr[14] ), M44( arr[15] )
	{
	}

	TIKI_FORCE_INLINE Matrix::Matrix( const Matrix& mat )
		: M11( mat.M11 ), M21( mat.M21 ), M31( mat.M31 ), M41( mat.M41 ),
		  M12( mat.M12 ), M22( mat.M22 ), M32( mat.M32 ), M42( mat.M42 ),
		  M13( mat.M13 ), M23( mat.M23 ), M33( mat.M33 ), M43( mat.M43 ),
		  M14( mat.M14 ), M24( mat.M24 ), M34( mat.M34 ), M44( mat.M44 )
	{
	}

	TIKI_FORCE_INLINE Matrix::~Matrix()
	{
	}

	TIKI_FORCE_INLINE Matrix& Matrix::operator=( const Matrix& mat )
	{
		M11 = mat.M11; M12 = mat.M12; M13 = mat.M13; M14 = mat.M14; 
		M21 = mat.M21; M22 = mat.M22; M23 = mat.M23; M24 = mat.M24; 
		M31 = mat.M31; M32 = mat.M32; M33 = mat.M33; M34 = mat.M34; 
		M41 = mat.M41; M42 = mat.M42; M43 = mat.M43; M44 = mat.M44;
		return *this;
	}

	TIKI_FORCE_INLINE bool Matrix::operator==( const Matrix& mat ) const
	{
		return isEquals( mat );
	}

	TIKI_FORCE_INLINE bool Matrix::operator!=( const Matrix& mat ) const
	{
		return !isEquals( mat );
	}

	TIKI_FORCE_INLINE float& Matrix::operator[]( const int index )
	{
		TIKI_ASSERT( index >= 0 && index <= 15 );
		return n[ index ];
	}

	TIKI_FORCE_INLINE const float Matrix::operator[]( const int index ) const
	{
		TIKI_ASSERT( index >= 0 && index <= 15 );
		return n[ index ];
	}

	TIKI_FORCE_INLINE Matrix Matrix::operator-( void ) const
	{
		return Matrix( 
			-M11, -M21, -M31, -M41,
			-M12, -M22, -M32, -M42,
			-M13, -M23, -M33, -M43,
			-M14, -M24, -M34, -M44 );
	}

	TIKI_FORCE_INLINE Matrix::operator float*( void )
	{
		return n;
	}

	TIKI_FORCE_INLINE Matrix::operator const float*( void ) const
	{
		return n;
	}

	TIKI_FORCE_INLINE bool Matrix::isZero( const float epsilon /*= f32::s_epsilon*/ ) const
	{
		return
			f32::isZero( M11, epsilon ) &&
			f32::isZero( M12, epsilon ) &&
			f32::isZero( M13, epsilon ) &&
			f32::isZero( M14, epsilon ) &&

			f32::isZero( M21, epsilon ) &&
			f32::isZero( M22, epsilon ) &&
			f32::isZero( M23, epsilon ) &&
			f32::isZero( M24, epsilon ) &&

			f32::isZero( M31, epsilon ) &&
			f32::isZero( M32, epsilon ) &&
			f32::isZero( M33, epsilon ) &&
			f32::isZero( M34, epsilon ) &&

			f32::isZero( M41, epsilon ) &&
			f32::isZero( M42, epsilon ) &&
			f32::isZero( M43, epsilon ) &&
			f32::isZero( M44, epsilon );
	}

	TIKI_FORCE_INLINE bool Matrix::isEquals( const Matrix& other, const float epsilon /*= f32::s_epsilon*/ ) const
	{
		return
			f32::isEquals( M11, other.M11, epsilon ) &&
			f32::isEquals( M12, other.M12, epsilon ) &&
			f32::isEquals( M13, other.M13, epsilon ) &&
			f32::isEquals( M14, other.M14, epsilon ) &&

			f32::isEquals( M21, other.M21, epsilon ) &&
			f32::isEquals( M22, other.M22, epsilon ) &&
			f32::isEquals( M23, other.M23, epsilon ) &&
			f32::isEquals( M24, other.M24, epsilon ) &&

			f32::isEquals( M31, other.M31, epsilon ) &&
			f32::isEquals( M32, other.M32, epsilon ) &&
			f32::isEquals( M33, other.M33, epsilon ) &&
			f32::isEquals( M34, other.M34, epsilon ) &&

			f32::isEquals( M41, other.M41, epsilon ) &&
			f32::isEquals( M42, other.M42, epsilon ) &&
			f32::isEquals( M43, other.M43, epsilon ) &&
			f32::isEquals( M44, other.M44, epsilon );
	}

	TIKI_FORCE_INLINE bool Matrix::isIdentity( const float epsilon /*= f32::s_epsilon*/ ) const
	{
		return
			f32::isEquals( M11, 1.0f, epsilon ) &&
			f32::isEquals( M12, 0.0f, epsilon ) &&
			f32::isEquals( M13, 0.0f, epsilon ) &&
			f32::isEquals( M14, 0.0f, epsilon ) &&

			f32::isEquals( M21, 0.0f, epsilon ) &&
			f32::isEquals( M22, 1.0f, epsilon ) &&
			f32::isEquals( M23, 0.0f, epsilon ) &&
			f32::isEquals( M24, 0.0f, epsilon ) &&

			f32::isEquals( M31, 0.0f, epsilon ) &&
			f32::isEquals( M32, 0.0f, epsilon ) &&
			f32::isEquals( M33, 1.0f, epsilon ) &&
			f32::isEquals( M34, 0.0f, epsilon ) &&

			f32::isEquals( M41, 0.0f, epsilon ) &&
			f32::isEquals( M42, 0.0f, epsilon ) &&
			f32::isEquals( M43, 0.0f, epsilon ) &&
			f32::isEquals( M44, 1.0f, epsilon );
	}

	TIKI_FORCE_INLINE void Matrix::negate( void )
	{
		mul( -1.0f );
	}

	TIKI_FORCE_INLINE void Matrix::negate( const Matrix& mat )
	{
		M11 = -mat.M11; M21 = -mat.M21; M31 = -mat.M31; M41 = -mat.M41;
		M12 = -mat.M12; M22 = -mat.M22; M32 = -mat.M32; M42 = -mat.M42;
		M13 = -mat.M13; M23 = -mat.M23; M33 = -mat.M33; M43 = -mat.M43;
		M14 = -mat.M14; M24 = -mat.M24; M34 = -mat.M34; M44 = -mat.M44;
	}

	TIKI_FORCE_INLINE void Matrix::makeIdentity( void )
	{
		M11 = 1.0f; M21 = 0.0f; M31 = 0.0f; M41 = 0.0f;
		M12 = 0.0f; M22 = 1.0f; M32 = 0.0f; M42 = 0.0f;
		M13 = 0.0f; M23 = 0.0f; M33 = 1.0f; M43 = 0.0f;
		M14 = 0.0f; M24 = 0.0f; M34 = 0.0f; M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::add( const Matrix& mat )
	{
		M11 += mat.M11;	M21 += mat.M21;	M31 += mat.M31;	M41 += mat.M41;
		M12 += mat.M12;	M22 += mat.M22;	M32 += mat.M32;	M42 += mat.M42;
		M13 += mat.M13;	M23 += mat.M23;	M33 += mat.M33;	M43 += mat.M43;
		M14 += mat.M14;	M24 += mat.M24;	M34 += mat.M34;	M44 += mat.M44;
	}

	TIKI_FORCE_INLINE void Matrix::add( const Matrix& mat1, const Matrix& mat2 )
	{
		M11 = mat1.M11 + mat2.M11;	M21 = mat1.M21 + mat2.M21;	M31 = mat1.M31 + mat2.M31;	M41 = mat1.M41 + mat2.M41;
		M12 = mat1.M12 + mat2.M12;	M22 = mat1.M22 + mat2.M22;	M32 = mat1.M32 + mat2.M32;	M42 = mat1.M42 + mat2.M42;
		M13 = mat1.M13 + mat2.M13;	M23 = mat1.M23 + mat2.M23;	M33 = mat1.M33 + mat2.M33;	M43 = mat1.M43 + mat2.M43;
		M14 = mat1.M14 + mat2.M14;	M24 = mat1.M24 + mat2.M24;	M34 = mat1.M34 + mat2.M34;	M44 = mat1.M44 + mat2.M44;
	}

	TIKI_FORCE_INLINE void Matrix::sub( const Matrix& mat )
	{
		M11 -= mat.M11;	M21 -= mat.M21;	M31 -= mat.M31;	M41 -= mat.M41;
		M12 -= mat.M12;	M22 -= mat.M22;	M32 -= mat.M32;	M42 -= mat.M42;
		M13 -= mat.M13;	M23 -= mat.M23;	M33 -= mat.M33;	M43 -= mat.M43;
		M14 -= mat.M14;	M24 -= mat.M24;	M34 -= mat.M34;	M44 -= mat.M44;
	}

	TIKI_FORCE_INLINE void Matrix::sub( const Matrix& mat1, const Matrix& mat2 )
	{
		M11 = mat1.M11 - mat2.M11;	M21 = mat1.M21 - mat2.M21;	M31 = mat1.M31 - mat2.M31;	M41 = mat1.M41 - mat2.M41;
		M12 = mat1.M12 - mat2.M12;	M22 = mat1.M22 - mat2.M22;	M32 = mat1.M32 - mat2.M32;	M42 = mat1.M42 - mat2.M42;
		M13 = mat1.M13 - mat2.M13;	M23 = mat1.M23 - mat2.M23;	M33 = mat1.M33 - mat2.M33;	M43 = mat1.M43 - mat2.M43;
		M14 = mat1.M14 - mat2.M14;	M24 = mat1.M24 - mat2.M24;	M34 = mat1.M34 - mat2.M34;	M44 = mat1.M44 - mat2.M44;
	}

	TIKI_FORCE_INLINE void Matrix::mul( const Matrix& mat )
	{
		const float tmpM11 = M11, tmpM12 = M12, tmpM13 = M13, tmpM14 = M14;
		const float tmpM21 = M21, tmpM22 = M22, tmpM23 = M23, tmpM24 = M24;
		const float tmpM31 = M31, tmpM32 = M32, tmpM33 = M33, tmpM34 = M34;
		const float tmpM41 = M41, tmpM42 = M42, tmpM43 = M43, tmpM44 = M44;

		M11 = tmpM11 * mat.M11 + tmpM12 * mat.M21 + tmpM13 * mat.M31 + tmpM14 * mat.M41;
		M12 = tmpM11 * mat.M12 + tmpM12 * mat.M22 + tmpM13 * mat.M32 + tmpM14 * mat.M42;
		M13 = tmpM11 * mat.M13 + tmpM12 * mat.M23 + tmpM13 * mat.M33 + tmpM14 * mat.M43;
		M14 = tmpM11 * mat.M14 + tmpM12 * mat.M24 + tmpM13 * mat.M34 + tmpM14 * mat.M44;
		M21 = tmpM21 * mat.M11 + tmpM22 * mat.M21 + tmpM23 * mat.M31 + tmpM24 * mat.M41;
		M22 = tmpM21 * mat.M12 + tmpM22 * mat.M22 + tmpM23 * mat.M32 + tmpM24 * mat.M42;
		M23 = tmpM21 * mat.M13 + tmpM22 * mat.M23 + tmpM23 * mat.M33 + tmpM24 * mat.M43;
		M24 = tmpM21 * mat.M14 + tmpM22 * mat.M24 + tmpM23 * mat.M34 + tmpM24 * mat.M44;
		M31 = tmpM31 * mat.M11 + tmpM32 * mat.M21 + tmpM33 * mat.M31 + tmpM34 * mat.M41;
		M32 = tmpM31 * mat.M12 + tmpM32 * mat.M22 + tmpM33 * mat.M32 + tmpM34 * mat.M42;
		M33 = tmpM31 * mat.M13 + tmpM32 * mat.M23 + tmpM33 * mat.M33 + tmpM34 * mat.M43;
		M34 = tmpM31 * mat.M14 + tmpM32 * mat.M24 + tmpM33 * mat.M34 + tmpM34 * mat.M44;
		M41 = tmpM41 * mat.M11 + tmpM42 * mat.M21 + tmpM43 * mat.M31 + tmpM44 * mat.M41;
		M42 = tmpM41 * mat.M12 + tmpM42 * mat.M22 + tmpM43 * mat.M32 + tmpM44 * mat.M42;
		M43 = tmpM41 * mat.M13 + tmpM42 * mat.M23 + tmpM43 * mat.M33 + tmpM44 * mat.M43;
		M44 = tmpM41 * mat.M14 + tmpM42 * mat.M24 + tmpM43 * mat.M34 + tmpM44 * mat.M44;
	}

	TIKI_FORCE_INLINE void Matrix::mul( const Matrix& mat1, const Matrix& mat2 )
	{
		TIKI_ASSERT( this != &mat1 );
		TIKI_ASSERT( this != &mat2 );

		M11 = mat1.M11 * mat2.M11 + mat1.M12 * mat2.M21 + mat1.M13 * mat2.M31 + mat1.M14 * mat2.M41;
		M12 = mat1.M11 * mat2.M12 + mat1.M12 * mat2.M22 + mat1.M13 * mat2.M32 + mat1.M14 * mat2.M42;
		M13 = mat1.M11 * mat2.M13 + mat1.M12 * mat2.M23 + mat1.M13 * mat2.M33 + mat1.M14 * mat2.M43;
		M14 = mat1.M11 * mat2.M14 + mat1.M12 * mat2.M24 + mat1.M13 * mat2.M34 + mat1.M14 * mat2.M44;
		M21 = mat1.M21 * mat2.M11 + mat1.M22 * mat2.M21 + mat1.M23 * mat2.M31 + mat1.M24 * mat2.M41;
		M22 = mat1.M21 * mat2.M12 + mat1.M22 * mat2.M22 + mat1.M23 * mat2.M32 + mat1.M24 * mat2.M42;
		M23 = mat1.M21 * mat2.M13 + mat1.M22 * mat2.M23 + mat1.M23 * mat2.M33 + mat1.M24 * mat2.M43;
		M24 = mat1.M21 * mat2.M14 + mat1.M22 * mat2.M24 + mat1.M23 * mat2.M34 + mat1.M24 * mat2.M44;
		M31 = mat1.M31 * mat2.M11 + mat1.M32 * mat2.M21 + mat1.M33 * mat2.M31 + mat1.M34 * mat2.M41;
		M32 = mat1.M31 * mat2.M12 + mat1.M32 * mat2.M22 + mat1.M33 * mat2.M32 + mat1.M34 * mat2.M42;
		M33 = mat1.M31 * mat2.M13 + mat1.M32 * mat2.M23 + mat1.M33 * mat2.M33 + mat1.M34 * mat2.M43;
		M34 = mat1.M31 * mat2.M14 + mat1.M32 * mat2.M24 + mat1.M33 * mat2.M34 + mat1.M34 * mat2.M44;
		M41 = mat1.M41 * mat2.M11 + mat1.M42 * mat2.M21 + mat1.M43 * mat2.M31 + mat1.M44 * mat2.M41;
		M42 = mat1.M41 * mat2.M12 + mat1.M42 * mat2.M22 + mat1.M43 * mat2.M32 + mat1.M44 * mat2.M42;
		M43 = mat1.M41 * mat2.M13 + mat1.M42 * mat2.M23 + mat1.M43 * mat2.M33 + mat1.M44 * mat2.M43;
		M44 = mat1.M41 * mat2.M14 + mat1.M42 * mat2.M24 + mat1.M43 * mat2.M34 + mat1.M44 * mat2.M44;
	}

	TIKI_FORCE_INLINE void Matrix::mul( const Matrix& mat, const float scale )
	{
		M11 = mat.M11 * scale;	M21 = mat.M21 * scale;	M31 = mat.M31 * scale;	M41 = mat.M41 * scale;
		M12 = mat.M12 * scale;	M22 = mat.M22 * scale;	M32 = mat.M32 * scale;	M42 = mat.M42 * scale;
		M13 = mat.M13 * scale;	M23 = mat.M23 * scale;	M33 = mat.M33 * scale;	M43 = mat.M43 * scale;
		M14 = mat.M14 * scale;	M24 = mat.M24 * scale;	M34 = mat.M34 * scale;	M44 = mat.M44 * scale;
	}

	TIKI_FORCE_INLINE void Matrix::mul( const float scale )
	{
		M11 *= scale; M21 *= scale; M31 *= scale; M41 *= scale;
		M12 *= scale; M22 *= scale; M32 *= scale; M42 *= scale;
		M13 *= scale; M23 *= scale; M33 *= scale; M43 *= scale;
		M14 *= scale; M24 *= scale; M34 *= scale; M44 *= scale;
	}

	TIKI_FORCE_INLINE void Matrix::div( const Matrix& mat )
	{
		TIKI_ASSERT( !isZero( mat.M11 ) && !isZero( mat.M21 ) && !isZero( mat.M31 ) && !isZero( mat.M41 ) );
		TIKI_ASSERT( !isZero( mat.M12 ) && !isZero( mat.M22 ) && !isZero( mat.M32 ) && !isZero( mat.M42 ) );
		TIKI_ASSERT( !isZero( mat.M13 ) && !isZero( mat.M23 ) && !isZero( mat.M33 ) && !isZero( mat.M43 ) );
		TIKI_ASSERT( !isZero( mat.M14 ) && !isZero( mat.M24 ) && !isZero( mat.M34 ) && !isZero( mat.M44 ) );

		M11 /= mat.M11;	M21 /= mat.M21;	M31 /= mat.M31;	M41 /= mat.M41;
		M12 /= mat.M12;	M22 /= mat.M22;	M32 /= mat.M32;	M42 /= mat.M42;
		M13 /= mat.M13;	M23 /= mat.M23;	M33 /= mat.M33;	M43 /= mat.M43;
		M14 /= mat.M14;	M24 /= mat.M24;	M34 /= mat.M34;	M44 /= mat.M44;
	}

	TIKI_FORCE_INLINE void Matrix::div( const Matrix& mat1, const Matrix& mat2 )
	{
		TIKI_ASSERT( !f32::isZero( mat2.M11 ) && !f32::isZero( mat2.M21 ) && !f32::isZero( mat2.M31 ) && !f32::isZero( mat2.M41 ) );
		TIKI_ASSERT( !f32::isZero( mat2.M12 ) && !f32::isZero( mat2.M22 ) && !f32::isZero( mat2.M32 ) && !f32::isZero( mat2.M42 ) );
		TIKI_ASSERT( !f32::isZero( mat2.M13 ) && !f32::isZero( mat2.M23 ) && !f32::isZero( mat2.M33 ) && !f32::isZero( mat2.M43 ) );
		TIKI_ASSERT( !f32::isZero( mat2.M14 ) && !f32::isZero( mat2.M24 ) && !f32::isZero( mat2.M34 ) && !f32::isZero( mat2.M44 ) );

		M11 = mat1.M11 / mat2.M11;	M21 = mat1.M21 / mat2.M21;	M31 = mat1.M31 / mat2.M31;	M41 = mat1.M41 / mat2.M41;
		M12 = mat1.M12 / mat2.M12;	M22 = mat1.M22 / mat2.M22;	M32 = mat1.M32 / mat2.M32;	M42 = mat1.M42 / mat2.M42;
		M13 = mat1.M13 / mat2.M13;	M23 = mat1.M23 / mat2.M23;	M33 = mat1.M33 / mat2.M33;	M43 = mat1.M43 / mat2.M43;
		M14 = mat1.M14 / mat2.M14;	M24 = mat1.M24 / mat2.M24;	M34 = mat1.M34 / mat2.M34;	M44 = mat1.M44 / mat2.M44;
	}

	TIKI_FORCE_INLINE void Matrix::div( const Matrix& mat, const float scal )
	{
		TIKI_ASSERT( !f32::isZero( scal ) );
		const float tmp = 1.0f / scal;

		M11 = mat.M11 * tmp;	M21 = mat.M21 * tmp;	M31 = mat.M31 * tmp;	M41 = mat.M41 * tmp;
		M12 = mat.M12 * tmp;	M22 = mat.M22 * tmp;	M32 = mat.M32 * tmp;	M42 = mat.M42 * tmp;
		M13 = mat.M13 * tmp;	M23 = mat.M23 * tmp;	M33 = mat.M33 * tmp;	M43 = mat.M43 * tmp;
		M14 = mat.M14 * tmp;	M24 = mat.M24 * tmp;	M34 = mat.M34 * tmp;	M44 = mat.M44 * tmp;
	}

	TIKI_FORCE_INLINE void Matrix::div( const float scal )
	{
		TIKI_ASSERT( !f32::isZero( scal ) );
		const float tmp = 1.0f / scal;

		M11 *= tmp; M21 *= tmp; M31 *= tmp; M41 *= tmp;
		M12 *= tmp; M22 *= tmp; M32 *= tmp; M42 *= tmp;
		M13 *= tmp; M23 *= tmp; M33 *= tmp; M43 *= tmp;
		M14 *= tmp; M24 *= tmp; M34 *= tmp; M44 *= tmp;
	}

	TIKI_FORCE_INLINE void Matrix::createIdentity()
	{
		M11 = 1.0f;
		M12 = 0.0f;
		M13 = 0.0f;
		M14 = 0.0f;

		M21 = 0.0f;
		M22 = 1.0f;
		M23 = 0.0f;
		M24 = 0.0f;

		M31 = 0.0f;
		M32 = 0.0f;
		M33 = 1.0f;
		M34 = 0.0f;

		M41 = 0.0f;
		M42 = 0.0f;
		M43 = 0.0f;
		M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::createRotationX( const float radians )
	{
		float num   = cosf( radians );
		float num2	= sinf( radians );

		M11 = 1.0f; M12 = 0.0f;  M13 = 0.0f; M14 = 0.0f;
		M21 = 0.0f; M22 = num;   M23 = num2; M24 = 0.0f;
		M31 = 0.0f; M32 = -num2; M33 = num;  M34 = 0.0f;
		M41 = 0.0f; M42 = 0.0f;  M43 = 0.0f; M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::createRotationY( const float radians )
	{
		const float num = cosf( radians );
		const float num2 = sinf( radians );

		M11 = num;	M12 = 0.0f;	M13 = -num2; M14 = 0.0f;
		M21 = 0.0f;	M22 = 1.0f;	M23 = 0.0f;	 M24 = 0.0f;
		M31 = num2;	M32 = 0.0f;	M33 = num;	 M34 = 0.0f;
		M41 = 0.0f;	M42 = 0.0f;	M43 = 0.0f;	 M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::createRotationZ( const float radians )
	{
		float num = cosf( radians );
		float num2 = sinf( radians );

		M11 = num;	M12 = 0.0f; M13 = -num2; M14 = 0.0f;
		M21 = 0.0f; M22 = 1.0f; M23 = 0.0f;	 M24 = 0.0f;
		M31 = num2; M32 = 0.0f; M33 = num;   M34 = 0.0f;
		M41 = 0.0f; M42 = 0.0f; M43 = 0.0f;  M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::createAxisAngle( const Vector3& axis, float radians )
	{
		const float x = axis.x;
		const float y = axis.y;
		const float z = axis.z;
		const float num = sinf( radians );
		const float num2 = cosf( radians );
		const float num3 = x * x;
		const float num4 = y * y;
		const float num5 = z * z;
		const float num6 = x * y;
		const float num7 = x * z;
		const float num8 = y * z;

		M11 = num3 + num2 * (1.0f - num3);
		M12 = num6 - num2 * num6 + num * z;
		M13 = num7 - num2 * num7 - num * y;
		M14 = 0.0f;
		M21 = num6 - num2 * num6 - num * z;
		M22 = num4 + num2 * (1.0f - num4);
		M23 = num8 - num2 * num8 + num * x;
		M24 = 0.0f;
		M31 = num7 - num2 * num7 + num * y;
		M32 = num8 - num2 * num8 - num * x;
		M33 = num5 + num2 * (1.0f - num5);
		M34 = 0.0f;
		M41 = 0.0f;
		M42 = 0.0f;
		M43 = 0.0f;
		M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::createTranslation( const Vector3& vec )
	{
		M11 = 1.0f;	M21 = 0.0f;	M31 = 0.0f;	M41 = vec.x;
		M12 = 0.0f;	M22 = 1.0f;	M32 = 0.0f;	M42 = vec.y;
		M13 = 0.0f;	M23 = 0.0f;	M33 = 1.0f;	M43 = vec.z;
		M14 = 0.0f;	M24 = 0.0f;	M34 = 0.0f;	M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::createTranslation( const float x, const float y, const float z )
	{
		M11 = 1.0f;	M21 = 0.0f;	M31 = 0.0f;	M41 = x;
		M12 = 0.0f;	M22 = 1.0f;	M32 = 0.0f;	M42 = y;
		M13 = 0.0f;	M23 = 0.0f;	M33 = 1.0f;	M43 = z;
		M14 = 0.0f;	M24 = 0.0f;	M34 = 0.0f;	M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::createScale( const Vector3& vec )
	{
		M11 = vec.x;	M21 = 0.0f;		M31 = 0.0f;		M41 = 0.0f;
		M12 = 0.0f;		M22 = vec.y;	M32 = 0.0f;		M42 = 0.0f;
		M13 = 0.0f;		M23 = 0.0f;		M33 = vec.z;	M43 = 0.0f;
		M14 = 0.0f;		M24 = 0.0f;		M34 = 0.0f;		M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::createScale( const float x, const float y, const float z )
	{
		M11 = x;	M21 = 0.0f;	M31 = 0.0f;	M41 = 0.0f;
		M12 = 0.0f;	M22 = y;	M32 = 0.0f;	M42 = 0.0f;
		M13 = 0.0f;	M23 = 0.0f;	M33 = z;	M43 = 0.0f;
		M14 = 0.0f;	M24 = 0.0f;	M34 = 0.0f;	M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::createScale( const float all )
	{
		M11 = all;	M21 = 0.0f;	M31 = 0.0f;	M41 = 0.0f;
		M12 = 0.0f;	M22 = all;	M32 = 0.0f;	M42 = 0.0f;
		M13 = 0.0f;	M23 = 0.0f;	M33 = all;	M43 = 0.0f;
		M14 = 0.0f;	M24 = 0.0f;	M34 = 0.0f;	M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::createWorld( const Vector3& pos, const Vector3& forward, const Vector3& up )
	{
		Vector3 zaxis;
		zaxis.negate( forward );
		zaxis.normalize();

		Vector3 xaxis;
		xaxis.cross( up, zaxis );
		xaxis.normalize();

		Vector3 yaxis;
		yaxis.cross( zaxis, xaxis );

		M11 = xaxis.x;	M21 = yaxis.x;	M31 = zaxis.x;	M41 = pos.x;
		M12 = xaxis.y;	M22 = yaxis.y;	M32 = zaxis.y;	M42 = pos.y;
		M13 = xaxis.z;	M23 = yaxis.z;	M33 = zaxis.z;	M43 = pos.z;
		M14 = 0.0f;		M24 = 0.0f;		M34 = 0.0f;		M44 = 1.0f;

	}

	TIKI_FORCE_INLINE void Matrix::createFromQuaternion( const Quaternion& rotation )
	{
		const float xx = rotation.x * rotation.x;
		const float yy = rotation.y * rotation.y;
		const float zz = rotation.z * rotation.z;
		const float xy = rotation.x * rotation.y;
		const float zw = rotation.z * rotation.w;
		const float zx = rotation.z * rotation.x;
		const float yw = rotation.y * rotation.w;
		const float yz = rotation.y * rotation.z;
		const float xw = rotation.x * rotation.w;

		createIdentity();
		M11 = 1.0f - (2.0f * (yy + zz));
		M12 = 2.0f * (xy + zw);
		M13 = 2.0f * (zx - yw);
		M21 = 2.0f * (xy - zw);
		M22 = 1.0f - (2.0f * (zz + xx));
		M23 = 2.0f * (yz + xw);
		M31 = 2.0f * (zx + yw);
		M32 = 2.0f * (yz - xw);
		M33 = 1.0f - (2.0f * (yy + xx));
	}

	TIKI_FORCE_INLINE void Matrix::createFromYawPitchRoll( const float yaw, const float pitch, const float roll )
	{
		Quaternion quat;
		quat.fromYawPitchRoll( yaw, pitch, roll );
		createFromQuaternion( quat );
	}

	TIKI_FORCE_INLINE void Matrix::createLookAtLH( const Vector3& camera, const Vector3& target, const Vector3& up )
	{
		Vector3 zaxis;
		zaxis.sub( target, camera );
		zaxis.normalize();

		Vector3 xaxis;
		xaxis.cross( up, zaxis );
		xaxis.normalize();

		Vector3 yaxis;
		yaxis.cross( zaxis, xaxis );

		M11 = xaxis.x; M12 = yaxis.x; M13 = zaxis.x; M14 = 0.0f;
		M21 = xaxis.y; M22 = yaxis.y; M23 = zaxis.y; M24 = 0.0f;
		M31 = xaxis.z; M32 = yaxis.z; M33 = zaxis.z; M34 = 0.0f;
		M41 = -Vector3::dot( xaxis, camera );
		M42 = -Vector3::dot( yaxis, camera );
		M43 = -Vector3::dot( zaxis, camera );
		M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::createLookAtRH( const Vector3& camera, const Vector3& target, const Vector3& up )
	{
		Vector3 zaxis;
		zaxis.sub( camera, target );
		zaxis.normalize();

		Vector3 xaxis;
		xaxis.cross( up, zaxis );
		xaxis.normalize();

		Vector3 yaxis;
		yaxis.cross( zaxis, xaxis );

		M11 = xaxis.x; M12 = yaxis.x; M13 = zaxis.x; M14 = 0.0f;
		M21 = xaxis.y; M22 = yaxis.y; M23 = zaxis.y; M24 = 0.0f;
		M31 = xaxis.z; M32 = yaxis.z; M33 = zaxis.z; M34 = 0.0f;
		M41 = -Vector3::dot( xaxis, camera );
		M42 = -Vector3::dot( yaxis, camera );
		M43 = -Vector3::dot( zaxis, camera );
		M44 = 1.0f;
	}

	TIKI_FORCE_INLINE void Matrix::createPerspectiveFieldOfViewLH( 
		const float fieldOfView, const float aspectRatio, 
		const float nearPlane, const float farPlane )
	{
		TIKI_ASSERT( fieldOfView > 0.0f && fieldOfView < f32::s_pi );
		TIKI_ASSERT( nearPlane > 0.0f );
		TIKI_ASSERT( farPlane > nearPlane );

		const float h = 1.0f / tanf( fieldOfView * 0.5f );
		const float w = h / aspectRatio;

		M11 = w;		M12 = 0.0f;		M13 = 0.0f;		M14 = 0.0f;
		M21 = 0.0f;		M22 = h;		M23 = 0.0f;		M24 = 0.0f;
		M31 = 0.0f;		M32 = 0.0f;						M34 = 1.0f;
		M41 = 0.0f;		M42 = 0.0f;						M44 = 0.0f;

		M33 = farPlane / ( farPlane - nearPlane );
		M43 = -nearPlane * farPlane / ( farPlane - nearPlane );
	}

	TIKI_FORCE_INLINE void Matrix::createPerspectiveFieldOfViewRH( 
		const float fieldOfView, const float aspectRatio, 
		const float nearPlane, const float farPlane )
	{
		TIKI_ASSERT( fieldOfView > 0.0f && fieldOfView < f32::s_pi );
		TIKI_ASSERT( nearPlane > 0.0f );
		TIKI_ASSERT( farPlane > nearPlane );

		const float h = 1.0f / tanf( fieldOfView * 0.5f );
		const float w = h / aspectRatio;

		M11 = w;		M12 = 0.0f;		M13 = 0.0f;		M14 = 0.0f;
		M21 = 0.0f;		M22 = h;		M23 = 0.0f;		M24 = 0.0f;
		M31 = 0.0f;		M32 = 0.0f;						M34 = -1.0f;
		M41 = 0.0f;		M42 = 0.0f;						M44 = 0.0f;

		M33 = farPlane / ( nearPlane - farPlane );
		M43 = nearPlane * farPlane / ( nearPlane - farPlane );
	}

	TIKI_FORCE_INLINE void Matrix::transpose( void )
	{
		tikiSwap( M12, M21 );
		tikiSwap( M13, M31 ); tikiSwap( M23, M32 );
		tikiSwap( M14, M41 ); tikiSwap( M24, M42 ); tikiSwap( M34, M43 ); 
	}

	TIKI_FORCE_INLINE void Matrix::transpose( const Matrix& mat )
	{
		TIKI_ASSERT( &*this != &mat );

		M11 = mat.M11; M21 = mat.M12; M31 = mat.M13; M41 = mat.M14;
		M12 = mat.M21; M22 = mat.M22; M32 = mat.M23; M42 = mat.M24;
		M13 = mat.M31; M23 = mat.M32; M33 = mat.M33; M43 = mat.M34;
		M14 = mat.M41; M24 = mat.M42; M34 = mat.M43; M44 = mat.M44;
	}

	TIKI_FORCE_INLINE void Matrix::invert( void )
	{
		invert( *this );
	}

	TIKI_FORCE_INLINE void Matrix::invert( const Matrix& mat )
	{
		const float m   = mat.M11,	m2  = mat.M12,	m3  = mat.M13,	m4  = mat.M14;
		const float m5  = mat.M21,	m6  = mat.M22,	m7  = mat.M23,	m8  = mat.M24;
		const float m9  = mat.M31,	m10 = mat.M32,	m11 = mat.M33,	m12 = mat.M34;
		const float m13 = mat.M41,	m14 = mat.M42,	m15 = mat.M43,	m16 = mat.M44;

		const float num = m11 * m16 - m12 * m15;
		const float num2 = m10 * m16 - m12 * m14;
		const float num3 = m10 * m15 - m11 * m14;
		const float num4 = m9 * m16 - m12 * m13;
		const float num5 = m9 * m15 - m11 * m13;
		const float num6 = m9 * m14 - m10 * m13;
		const float num7 = m6 * num - m7 * num2 + m8 * num3;
		const float num8 = -(m5 * num - m7 * num4 + m8 * num5);
		const float num9 = m5 * num2 - m6 * num4 + m8 * num6;
		const float num10 = -(m5 * num3 - m6 * num5 + m7 * num6);
#if TIKI_BUILD_DEBUG
		const float tmp = (m * num7 + m2 * num8 + m3 * num9 + m4 * num10);
		TIKI_ASSERT( !f32::isZero( tmp ) );
		const float num11 = 1.0f / tmp;
#else
		const float num11 = 1.0f / (m * num7 + m2 * num8 + m3 * num9 + m4 * num10);;
#endif
		const float num12 = m7 * m16 - m8 * m15;
		const float num13 = m6 * m16 - m8 * m14;
		const float num14 = m6 * m15 - m7 * m14;
		const float num15 = m5 * m16 - m8 * m13;
		const float num16 = m5 * m15 - m7 * m13;
		const float num17 = m5 * m14 - m6 * m13;
		const float num18 = m7 * m12 - m8 * m11;
		const float num19 = m6 * m12 - m8 * m10;
		const float num20 = m6 * m11 - m7 * m10;
		const float num21 = m5 * m12 - m8 * m9;
		const float num22 = m5 * m11 - m7 * m9;
		const float num23 = m5 * m10 - m6 * m9;

		M11 = num7 * num11;
		M21 = num8 * num11;
		M31 = num9 * num11;
		M41 = num10 * num11;
		M12 = -(m2 * num - m3 * num2 + m4 * num3) * num11;
		M22 = (m * num - m3 * num4 + m4 * num5) * num11;
		M32 = -(m * num2 - m2 * num4 + m4 * num6) * num11;
		M42 = (m * num3 - m2 * num5 + m3 * num6) * num11;
		M13 = (m2 * num12 - m3 * num13 + m4 * num14) * num11;
		M23 = -(m * num12 - m3 * num15 + m4 * num16) * num11;
		M33 = (m * num13 - m2 * num15 + m4 * num17) * num11;
		M43 = -(m * num14 - m2 * num16 + m3 * num17) * num11;
		M14 = -(m2 * num18 - m3 * num19 + m4 * num20) * num11;
		M24 = (m * num18 - m3 * num21 + m4 * num22) * num11;
		M34 = -(m * num19 - m2 * num21 + m4 * num23) * num11;
		M44 = (m * num20 - m2 * num22 + m3 * num23) * num11;
	}

	TIKI_FORCE_INLINE void Matrix::lerp( const Matrix& mat, const float amount )
	{
		M11 = tiki::lerp( M11, mat.M11, amount );
		M12 = tiki::lerp( M12, mat.M12, amount );
		M13 = tiki::lerp( M13, mat.M13, amount );
		M14 = tiki::lerp( M14, mat.M14, amount );
		M21 = tiki::lerp( M21, mat.M21, amount );
		M22 = tiki::lerp( M22, mat.M22, amount );
		M23 = tiki::lerp( M23, mat.M23, amount );
		M24 = tiki::lerp( M24, mat.M24, amount );
		M31 = tiki::lerp( M31, mat.M31, amount );
		M32 = tiki::lerp( M32, mat.M32, amount );
		M33 = tiki::lerp( M33, mat.M33, amount );
		M34 = tiki::lerp( M34, mat.M34, amount );
		M41 = tiki::lerp( M41, mat.M41, amount );
		M42 = tiki::lerp( M42, mat.M42, amount );
		M43 = tiki::lerp( M43, mat.M43, amount );
		M44 = tiki::lerp( M44, mat.M44, amount );
	}

	TIKI_FORCE_INLINE void Matrix::lerp( const Matrix& mat1, const Matrix& mat2, const float amount )
	{
		M11 = tiki::lerp( mat1.M11, mat2.M11, amount );
		M12 = tiki::lerp( mat1.M12, mat2.M12, amount );
		M13 = tiki::lerp( mat1.M13, mat2.M13, amount );
		M14 = tiki::lerp( mat1.M14, mat2.M14, amount );
		M21 = tiki::lerp( mat1.M21, mat2.M21, amount );
		M22 = tiki::lerp( mat1.M22, mat2.M22, amount );
		M23 = tiki::lerp( mat1.M23, mat2.M23, amount );
		M24 = tiki::lerp( mat1.M24, mat2.M24, amount );
		M31 = tiki::lerp( mat1.M31, mat2.M31, amount );
		M32 = tiki::lerp( mat1.M32, mat2.M32, amount );
		M33 = tiki::lerp( mat1.M33, mat2.M33, amount );
		M34 = tiki::lerp( mat1.M34, mat2.M34, amount );
		M41 = tiki::lerp( mat1.M41, mat2.M41, amount );
		M42 = tiki::lerp( mat1.M42, mat2.M42, amount );
		M43 = tiki::lerp( mat1.M43, mat2.M43, amount );
		M44 = tiki::lerp( mat1.M44, mat2.M44, amount );
	}

	TIKI_FORCE_INLINE void Matrix::scale( const Vector3& scale )
	{
		M11 *= scale.x; M21 *= scale.x; M31 *= scale.x; M41 *= scale.x;
		M12 *= scale.y; M22 *= scale.y; M32 *= scale.y; M42 *= scale.y;
		M13 *= scale.z; M23 *= scale.z; M33 *= scale.z; M43 *= scale.z;
	}

	TIKI_FORCE_INLINE void Matrix::forward( Vector3& vec )
	{
		vec.x = -M31;
		vec.y = -M32;
		vec.z = -M33;
	}

	TIKI_FORCE_INLINE void Matrix::backward( Vector3& vec )
	{
		vec.x = M31;
		vec.y = M32;
		vec.z = M33;
	}

	TIKI_FORCE_INLINE void Matrix::up( Vector3& vec )
	{
		vec.x = M21;
		vec.y = M22;
		vec.z = M23;
	}

	TIKI_FORCE_INLINE void Matrix::down( Vector3& vec )
	{
		vec.x = -M21;
		vec.y = -M22;
		vec.z = -M23;
	}

	TIKI_FORCE_INLINE void Matrix::right( Vector3& vec )
	{
		vec.x = M11;
		vec.y = M12;
		vec.z = M13;
	}

	TIKI_FORCE_INLINE void Matrix::left( Vector3& vec )
	{
		vec.x = -M11;
		vec.y = -M12;
		vec.z = -M13;
	}

	TIKI_FORCE_INLINE void Matrix::getTranslation( Vector3& vec )
	{
		vec.x = M41;
		vec.y = M42;
		vec.z = M43;
	}

	TIKI_FORCE_INLINE void Matrix::setTranslation( const Vector3& vec )
	{
		M41 = vec.x;
		M42 = vec.y;
		M43 = vec.z;
	}

	TIKI_FORCE_INLINE void Matrix::transform( Vector2& vec ) const
	{
		const float x = vec.x * M11 + vec.y * M21 + M41;
		const float y = vec.x * M12 + vec.y * M22 + M42;
		vec.x = x;
		vec.y = y;
	}

	TIKI_FORCE_INLINE void Matrix::transform( const Vector2& vec, Vector2& result ) const
	{
		result.x = vec.x * M11 + vec.y * M21 + M41;
		result.y = vec.x * M12 + vec.y * M22 + M42;
	}

	TIKI_FORCE_INLINE void Matrix::transform( Vector3& vec ) const
	{
		const float x = vec.x * M11 + vec.y * M21 + vec.z * M31 + M41;
		const float y = vec.x * M12 + vec.y * M22 + vec.z * M32 + M42;
		const float z = vec.x * M13 + vec.y * M23 + vec.z * M33 + M43;
		vec.x = x;
		vec.y = y;
		vec.z = z;
	}

	TIKI_FORCE_INLINE void Matrix::transform( const Vector3& vec, Vector3& result ) const
	{
		result.x = vec.x * M11 + vec.y * M21 + vec.z * M31 + M41;
		result.y = vec.x * M12 + vec.y * M22 + vec.z * M32 + M42;
		result.z = vec.x * M13 + vec.y * M23 + vec.z * M33 + M43;
	}

	TIKI_FORCE_INLINE void Matrix::transform( Vector4& vec ) const
	{
		const float x = vec.x * M11 + vec.y * M21 + M41;
		const float y = vec.x * M12 + vec.y * M22 + M42;
		const float z = vec.x * M13 + vec.y * M23 + M43;
		const float w = vec.x * M14 + vec.y * M24 + M44;
		vec.x = x;
		vec.y = y;
		vec.z = z;
		vec.w = w;
	}

	TIKI_FORCE_INLINE void Matrix::transform( const Vector4& vec, Vector4& result ) const
	{
		result.x = vec.x * M11 + vec.y * M21 + M41;
		result.y = vec.x * M12 + vec.y * M22 + M42;
		result.z = vec.x * M13 + vec.y * M23 + M43;
		result.w = vec.x * M14 + vec.y * M24 + M44;
	}

	TIKI_FORCE_INLINE void Matrix::transformAndClipToViewport( Vector3& vec, const Vector2& viewport ) const
	{
		const float w = 1.0f / ((vec.x * M14) + (vec.y * M24) + (vec.z * M34) + M44);
		transform( vec );
		vec.mul( w );

		vec.x = ( 1.0f + vec.x ) * 0.5f * viewport.x;
		vec.y = ( 1.0f - vec.y ) * 0.5f * viewport.y;
	}
}

#endif // TIKI_MATRIX_INL
