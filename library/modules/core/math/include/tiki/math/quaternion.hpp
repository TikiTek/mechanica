#ifndef TIKI_QUATERNION_HPP__
#define TIKI_QUATERNION_HPP__

#include "tiki/math/vector3.hpp"

#include <math.h>

namespace tiki
{
	struct Matrix;

	TIKI_PRE_ALIGN( 16 ) struct Quaternion
	{
		union 
		{
			struct  
			{
				float x, y, z, w;
			};
			float arr[4];
		};

		static const Quaternion			identity;
		
		TIKI_FORCE_INLINE				Quaternion();
		TIKI_FORCE_INLINE				Quaternion( const float x, const float y, const float z, const float w );
		TIKI_FORCE_INLINE				Quaternion( const float* arr );
		TIKI_FORCE_INLINE				Quaternion( const Vector3& vec, const float scalar );
		TIKI_FORCE_INLINE				Quaternion( const Quaternion& quat );
		TIKI_FORCE_INLINE				~Quaternion();

		TIKI_FORCE_INLINE Quaternion&	operator=( const Quaternion& rhs );
		TIKI_FORCE_INLINE bool			operator==( const Quaternion& quat );
		TIKI_FORCE_INLINE bool			operator!=( const Quaternion& quat );
		TIKI_FORCE_INLINE float&		operator[]( const int index );
		TIKI_FORCE_INLINE float			operator[]( const int index ) const;

		TIKI_FORCE_INLINE operator		float*();
		TIKI_FORCE_INLINE operator		const float*() const;

		void							fromMatrix( const Matrix& matrix );
		void							fromAxisAngle( const Vector3& axis, const float radians );
		void							fromYawPitchRoll( const float yaw, const float pitch, const float roll );

		TIKI_FORCE_INLINE bool			isEquals( const Quaternion& other, const float epsilon = f32::s_epsilon ) const;
		TIKI_FORCE_INLINE bool			isIdentity( const float epsilon = f32::s_epsilon ) const;
		TIKI_FORCE_INLINE bool			isZero( const float epsilon = f32::s_epsilon ) const;

		TIKI_FORCE_INLINE float			length() const;
		TIKI_FORCE_INLINE float			lengthSq() const;

		TIKI_FORCE_INLINE void			conjugate();
		TIKI_FORCE_INLINE void			conjugate( const Quaternion& quat );

		TIKI_FORCE_INLINE void			normalize();
		TIKI_FORCE_INLINE void			normalize( const Quaternion& quat );

		TIKI_FORCE_INLINE void			lookAt( const Vector3& target, const Vector3& up = Vector3::up );

		TIKI_FORCE_INLINE static float	dot( const Quaternion& quat1, const Quaternion& quat2 );

		TIKI_FORCE_INLINE void			lerp( const Quaternion& from, const Quaternion& to, const float amount );
		TIKI_FORCE_INLINE void			nlerp( const Quaternion& from, const Quaternion& to, const float amount );
		TIKI_FORCE_INLINE void			slerp( const Quaternion& from, const Quaternion& to, const float amount );

		TIKI_FORCE_INLINE void			negate();
		TIKI_FORCE_INLINE void			negate( const Quaternion& quat );

		TIKI_FORCE_INLINE void			add( const Quaternion& quat );
		TIKI_FORCE_INLINE void			add( const Quaternion& quat1, const Quaternion& quat2 );

		TIKI_FORCE_INLINE void			sub( const Quaternion& quat );
		TIKI_FORCE_INLINE void			sub( const Quaternion& quat1, const Quaternion& quat2 );

		TIKI_FORCE_INLINE void			mul( const Quaternion& quat );
		TIKI_FORCE_INLINE void			mul( const Quaternion& quat1, const Quaternion& quat2 );
		TIKI_FORCE_INLINE void			mul( const Quaternion& quat, const float scal );
		TIKI_FORCE_INLINE void			mul( const float scalar );

		TIKI_FORCE_INLINE void			div( const Quaternion& quat );
		TIKI_FORCE_INLINE void			div( const Quaternion& quat1, const Quaternion& quat2 );
		TIKI_FORCE_INLINE void			div( const Quaternion& quat, const float scal );
		TIKI_FORCE_INLINE void			div( const float scalar );

		TIKI_FORCE_INLINE void			transform( Vector3& vec ) const;
		TIKI_FORCE_INLINE void			transform( const Vector3& vec, Vector3& result ) const;

		TIKI_FORCE_INLINE void			getForward( Vector3& result ) const;
		TIKI_FORCE_INLINE void			getUp( Vector3& result ) const;
		TIKI_FORCE_INLINE void			getRight( Vector3& result ) const;

		TIKI_FORCE_INLINE void			getYawPitchRoll( Vector3& ypr ) const;

	} TIKI_POST_ALIGN( 16 );
}

#include "../../../source/quaternion.inl"

#endif // TIKI_QUATERNION_HPP__
