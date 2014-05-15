#ifndef TIKI_QUATERNION_HPP__
#define TIKI_QUATERNION_HPP__

#include "tiki/math/vector.hpp"
#include "tiki/math/matrix.hpp"

namespace tiki
{
	TIKI_PRE_ALIGN( 16 ) struct Quaternion
	{
		float x;
		float y;
		float z;
		float w;

		static const Quaternion zero;
		static const Quaternion one;
		static const Quaternion identity;
	}
	TIKI_POST_ALIGN( 16 );

	namespace quaternion
	{
		TIKI_FORCE_INLINE bool			isEquals( const Quaternion& lhs, const Quaternion& rhs, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool			isIdentity( const Quaternion& quat, float epsilon = f32::epsilon );
		TIKI_FORCE_INLINE bool			isZero( const Quaternion& quat, float epsilon = f32::epsilon );

		TIKI_FORCE_INLINE Quaternion&	clear( Quaternion& quat );
		TIKI_FORCE_INLINE Quaternion&	createIdentity( Quaternion& quat );

		TIKI_FORCE_INLINE Quaternion&	set( Quaternion& quat, const float4& source );
		TIKI_FORCE_INLINE Quaternion&	set( Quaternion& quat, float x, float y, float z, float w );

		TIKI_FORCE_INLINE Quaternion&	add( Quaternion& quat, const Quaternion& rhs );
		TIKI_FORCE_INLINE Quaternion&	sub( Quaternion& quat, const Quaternion& rhs );
		TIKI_FORCE_INLINE Quaternion&	mul( Quaternion& quat, const Quaternion& rhs );
		TIKI_FORCE_INLINE Quaternion&	div( Quaternion& quat, const Quaternion& rhs );
		TIKI_FORCE_INLINE Quaternion&	scale( Quaternion& quat, float val );

		TIKI_FORCE_INLINE float			length( const Quaternion& quat );
		TIKI_FORCE_INLINE float			lengthSquared( const Quaternion& quat );
		TIKI_FORCE_INLINE float			dot( const Quaternion& quat1, const Quaternion& quat2 );

		TIKI_FORCE_INLINE Quaternion&	negate( Quaternion& quat );
		TIKI_FORCE_INLINE Quaternion&	conjugate( Quaternion& quat );
		TIKI_FORCE_INLINE Quaternion&	normalize( Quaternion& quat );
					
		TIKI_FORCE_INLINE Quaternion&	nlerp( Quaternion& quat, const Quaternion& start, const Quaternion& end, const float amount );
		TIKI_FORCE_INLINE Quaternion&	slerp( Quaternion& quat, const Quaternion& start, const Quaternion& end, const float amount );

		TIKI_FORCE_INLINE void			createLookAt( Quaternion& quat, const Vector3& position, const Vector3& target, const Vector3& up = Vector3::unitY );

		TIKI_FORCE_INLINE void			fromMatrix( Quaternion& quat, const Matrix33& matrix );
		TIKI_FORCE_INLINE void			toMatrix( Matrix33& mtx, const Quaternion& quat );

		TIKI_FORCE_INLINE void			fromYawPitchRoll( Quaternion& quat, float yaw, float pitch, float roll );
		TIKI_FORCE_INLINE void			toYawPitchRoll( Vector3& yawPitchRoll, const Quaternion& quat );
		
		TIKI_FORCE_INLINE void			transform( Vector3& vec, const Quaternion& quat );

		TIKI_FORCE_INLINE void			getForward( Vector3& result, const Quaternion& quat );
		TIKI_FORCE_INLINE void			getBackward( Vector3& result, const Quaternion& quat );
		TIKI_FORCE_INLINE void			getLeft( Vector3& result, const Quaternion& quat );
		TIKI_FORCE_INLINE void			getRight( Vector3& result, const Quaternion& quat );
		TIKI_FORCE_INLINE void			getUp( Vector3& result, const Quaternion& quat );
		TIKI_FORCE_INLINE void			getDown( Vector3& result, const Quaternion& quat );
	};
}

#include "../../../source/quaternion.inl"

#endif // TIKI_QUATERNION_HPP__
