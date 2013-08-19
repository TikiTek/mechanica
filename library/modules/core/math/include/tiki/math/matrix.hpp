#ifndef TIKI_MATRIX_HPP__
#define TIKI_MATRIX_HPP__

#include "tiki/math/vector2.hpp"
#include "tiki/math/vector3.hpp"
#include "tiki/math/vector4.hpp"
#include "tiki/math/quaternion.hpp"

#include <math.h>

namespace tiki
{
	TIKI_PRE_ALIGN( 16 ) struct Matrix
	{
		union
		{
			struct  
			{
				float M11, M21, M31, M41;
				float M12, M22, M32, M42;
				float M13, M23, M33, M43;
				float M14, M24, M34, M44;
			};
			float m[ 4u ][ 4u ];
			float n[ 16u ];
		};

		static const	Matrix identity;

		TIKI_FORCE_INLINE Matrix( void );
		TIKI_FORCE_INLINE Matrix( float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44 );

		TIKI_FORCE_INLINE Matrix( const float* arr );
		TIKI_FORCE_INLINE Matrix( const Matrix& mat );
		TIKI_FORCE_INLINE ~Matrix( void );

		TIKI_FORCE_INLINE Matrix&			operator=( const Matrix& mat );
		TIKI_FORCE_INLINE bool				operator==( const Matrix& mat ) const;
		TIKI_FORCE_INLINE bool				operator!=( const Matrix& mat ) const;

		TIKI_FORCE_INLINE float&			operator[]( const int index );
		TIKI_FORCE_INLINE const float		operator[]( const int index ) const;
		TIKI_FORCE_INLINE Matrix			operator-( void ) const;

		TIKI_FORCE_INLINE operator			float*( void );
		TIKI_FORCE_INLINE operator const	float*( void ) const;

		TIKI_FORCE_INLINE bool				isEquals( const Matrix& other, const float epsilon = f32::s_epsilon ) const;
		TIKI_FORCE_INLINE bool				isIdentity( const float epsilon = f32::s_epsilon ) const;
		TIKI_FORCE_INLINE bool				isZero( const float epsilon = f32::s_epsilon ) const;

		TIKI_FORCE_INLINE void				negate( void );
		TIKI_FORCE_INLINE void				negate( const Matrix& mat );
		TIKI_FORCE_INLINE void				makeIdentity( void );

		TIKI_FORCE_INLINE void				add( const Matrix& mat );
		TIKI_FORCE_INLINE void				add( const Matrix& mat1, const Matrix& mat2 );

		TIKI_FORCE_INLINE void				sub( const Matrix& mat );
		TIKI_FORCE_INLINE void				sub( const Matrix& mat1, const Matrix& mat2 );	

		TIKI_FORCE_INLINE void				mul( const Matrix& mat );
		TIKI_FORCE_INLINE void				mul( const Matrix& mat1, const Matrix& mat2 );
		TIKI_FORCE_INLINE void				mul( const Matrix& mat, const float scal );
		TIKI_FORCE_INLINE void				mul( const float scal );

		TIKI_FORCE_INLINE void				div( const Matrix& mat );
		TIKI_FORCE_INLINE void				div( const Matrix& mat1, const Matrix& mat2 );
		TIKI_FORCE_INLINE void				div( const Matrix& mat, const float scal );
		TIKI_FORCE_INLINE void				div( const float scal );

		TIKI_FORCE_INLINE void				scale( const Vector3& scale );

		TIKI_FORCE_INLINE void				createIdentity();

		TIKI_FORCE_INLINE void				createRotationX( const float rad );
		TIKI_FORCE_INLINE void				createRotationY( const float rad );
		TIKI_FORCE_INLINE void				createRotationZ( const float rad );
		TIKI_FORCE_INLINE void				createAxisAngle( const Vector3& axis, float radians );

		TIKI_FORCE_INLINE void				createTranslation( const Vector3& vec );
		TIKI_FORCE_INLINE void				createTranslation( const float x, const float y, const float z );

		TIKI_FORCE_INLINE void				createScale( const Vector3& vec );
		TIKI_FORCE_INLINE void				createScale( const float x, const float y, const float z );
		TIKI_FORCE_INLINE void				createScale( const float all );

		TIKI_FORCE_INLINE void				createWorld( const Vector3& pos, const Vector3& forward, const Vector3& up );

		TIKI_FORCE_INLINE void				createFromQuaternion( const Quaternion& quat );
		TIKI_FORCE_INLINE void				createFromYawPitchRoll( const float yaw, const float pitch, const float roll );

		TIKI_FORCE_INLINE void				createLookAtLH( const Vector3& camera, const Vector3& target, const Vector3& up );
		TIKI_FORCE_INLINE void				createLookAtRH( const Vector3& camera, const Vector3& target, const Vector3& up );

		TIKI_FORCE_INLINE void				createPerspectiveFieldOfViewLH( const float fieldOfView, const float aspectRatio, 
																			const float nearPlanee, const float farPlane );
		TIKI_FORCE_INLINE void				createPerspectiveFieldOfViewRH( const float fieldOfView, const float aspectRatio, 
																			const float nearPlane, const float farPlane );

		void								compose( const Quaternion& rotation, const Vector3& translation, const Vector3& scale );
		bool								decompose( Quaternion& rotation, Vector3& translation, Vector3& scale ) const;

		TIKI_FORCE_INLINE void				transform( Vector2& vec ) const;
		TIKI_FORCE_INLINE void				transform( const Vector2& vec, Vector2& result ) const;
		TIKI_FORCE_INLINE void				transform( Vector3& vec ) const;
		TIKI_FORCE_INLINE void				transform( const Vector3& vec, Vector3& result ) const;
		TIKI_FORCE_INLINE void				transform( Vector4& vec ) const;
		TIKI_FORCE_INLINE void				transform( const Vector4& vec, Vector4& result ) const;

		TIKI_FORCE_INLINE void				transformAndClipToViewport( Vector3& vec, const Vector2& viewport ) const;

		TIKI_FORCE_INLINE void				transpose( void );
		TIKI_FORCE_INLINE void				transpose( const Matrix& mat );

		TIKI_FORCE_INLINE void				invert( void );
		TIKI_FORCE_INLINE void				invert( const Matrix& mat );

		TIKI_FORCE_INLINE void				lerp( const Matrix& mat, const float amount );
		TIKI_FORCE_INLINE void				lerp( const Matrix& mat1, const Matrix& mat2, const float amount );

		TIKI_FORCE_INLINE void				forward( Vector3& vec );
		TIKI_FORCE_INLINE void				backward( Vector3& vec );
		TIKI_FORCE_INLINE void				up( Vector3& vec );
		TIKI_FORCE_INLINE void				down( Vector3& vec );
		TIKI_FORCE_INLINE void				right( Vector3& vec );
		TIKI_FORCE_INLINE void				left( Vector3& vec );

		TIKI_FORCE_INLINE void				getTranslation( Vector3& vec );
		TIKI_FORCE_INLINE void				setTranslation( const Vector3& vec );

	} TIKI_POST_ALIGN( 16 );
}

#include "matrix.inl"

#endif // TIKI_MATRIX_HPP__
