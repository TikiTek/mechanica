#pragma once
#ifndef TIKI_QUATERNION_INL
#define TIKI_QUATERNION_INL

#include <math.h>

namespace tiki
{
	TIKI_FORCE_INLINE Quaternion::Quaternion():
		x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 1.0f )
	{

	}

	TIKI_FORCE_INLINE Quaternion::Quaternion( const float x, const float y, const float z, const float w ):
		x( x ), y( y ), z( z ), w( w )
	{

	}

	TIKI_FORCE_INLINE Quaternion::Quaternion( const float* arr ):
		x( arr[0] ), y( arr[1] ), z( arr[2] ), w( arr[3] )
	{

	}

	TIKI_FORCE_INLINE Quaternion::Quaternion( const Vector3& vec, const float scalar ):
		x( vec.x ), y( vec.y ), z( vec.z ), w( scalar )
	{

	}

	TIKI_FORCE_INLINE Quaternion::Quaternion( const Quaternion& quat ):
		x( quat.x ), y( quat.y ), z( quat.z ), w( quat.w )
	{

	}

	TIKI_FORCE_INLINE Quaternion::~Quaternion()
	{

	}

	TIKI_FORCE_INLINE Quaternion& Quaternion::operator=( const Quaternion& rhs )
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	TIKI_FORCE_INLINE bool Quaternion::operator==( const Quaternion& quat )
	{
		return isEquals( quat );
	}

	TIKI_FORCE_INLINE bool Quaternion::operator!=( const Quaternion& quat )
	{
		return !isEquals( quat );
	}

	TIKI_FORCE_INLINE float& Quaternion::operator[]( const int index )
	{
		TIKI_ASSERT( index >= 0 && index <= 3 );
		return arr[ index ];
	}

	TIKI_FORCE_INLINE float Quaternion::operator[]( const int index ) const
	{
		TIKI_ASSERT( index >= 0 && index <= 3 );
		return arr[ index ];
	}

	TIKI_FORCE_INLINE Quaternion::operator float*()
	{
		return arr;
	}

	TIKI_FORCE_INLINE Quaternion::operator const float*() const
	{
		return arr;
	}

	TIKI_FORCE_INLINE bool Quaternion::isZero( const float epsilon /*= f32::s_epsilon*/ ) const 
	{
		return f32::isZero( x, epsilon ) &&
			f32::isZero( y, epsilon ) &&
			f32::isZero( z, epsilon ) &&
			f32::isZero( w, epsilon );
	}

	TIKI_FORCE_INLINE bool Quaternion::isEquals( const Quaternion& other, const float epsilon /*= f32::s_epsilon*/ ) const
	{
		return f32::isEquals( x, other.x, epsilon ) &&
			f32::isEquals( y, other.y, epsilon ) &&
			f32::isEquals( z, other.z, epsilon ) &&
			f32::isEquals( w, other.w, epsilon );
	}

	TIKI_FORCE_INLINE bool Quaternion::isIdentity( const float epsilon /*= f32::s_epsilon*/ ) const
	{
		return f32::isEquals( x, 0.0f, epsilon ) &&
			f32::isEquals( y, 0.0f, epsilon ) &&
			f32::isEquals( z, 0.0f, epsilon ) &&
			f32::isEquals( w, 1.0f, epsilon );
	}

	TIKI_FORCE_INLINE float Quaternion::length() const
	{
		const float num = x * x + y * y + z * z + w * w;
		return sqrtf( num );
	}

	TIKI_FORCE_INLINE float Quaternion::lengthSq() const
	{
		return x * x + y * y + z * z + w * w;
	}

	TIKI_FORCE_INLINE void Quaternion::conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	TIKI_FORCE_INLINE void Quaternion::conjugate( const Quaternion& quat )
	{
		x = -quat.x;
		y = -quat.y;
		z = -quat.z;
	}

	TIKI_FORCE_INLINE void Quaternion::normalize()
	{
		TIKI_ASSERT( !f32::isZero( this->lengthSq() ) );
		const float tmp = math::rsqrt( this->lengthSq() );
		mul( tmp );
	}

	TIKI_FORCE_INLINE void Quaternion::normalize( const Quaternion& quat )
	{
		TIKI_ASSERT( !f32::isZero( quat.lengthSq() ) );

		const float tmp = math::rsqrt( quat.lengthSq() );
		mul( tmp );
	}

	TIKI_FORCE_INLINE void Quaternion::lookAt( const Vector3& target, const Vector3& upVec /*= Vector3::up*/ )
	{
		TIKI_ASSERT(false);
		//Vector3 forward;
		//Vector3 right;
		//Vector3 up;

		//forward.normalize( target );
		//up.normalize( upVec );

		//right.cross( up, forward );
		//up.cross( forward, right );

		////Matrix mBasis = new Matrix(
		////	vRight.X,		vRight.Y,		vRight.Z,		0.0f,
		////	vUp.X,			vUp.Y,			vUp.Z,			0.0f,
		////	vDirection.X,	vDirection.Y,	vDirection.Z,	0.0f,
		////	0.0f,			0.0f,			0.0f,			1.0f);

		////// Step 3. Build a quaternion from the matrix
		////Quaternion qrot = new Quaternion();
		////qrot.W = (float)Math.Sqrt(1.0f + mBasis.M11 + mBasis.M22 + mBasis.M33) / 2.0f;
		////double dfWScale = qrot.W * 4.0;
		////qrot.X = (float)((mBasis.M32 - mBasis.M23) / dfWScale);
		////qrot.Y = (float)((mBasis.M13 - mBasis.M31) / dfWScale);
		////qrot.Z = (float)((mBasis.M21 - mBasis.M12) / dfWScale);

		//w = sqrtf( 1.0f - right.x + up.y + forward.z ) * 0.5f;
		//const float tmp = 1.0f / ( 4.0f * w );
		////x = ( forward.y - up.z ) * tmp;
		////y = ( right.z - forward.x ) * tmp;
		////z = ( up.x - right.y ) * tmp;
	}

	TIKI_FORCE_INLINE float Quaternion::dot( const Quaternion& quat1, const Quaternion& quat2 )
	{
		return quat1.x * quat2.x + quat1.y * quat2.y + quat1.z * quat2.z + quat1.w * quat2.w;
	}

	TIKI_FORCE_INLINE void Quaternion::lerp( const Quaternion& from, const Quaternion& to, const float amount )
	{
		const float num = 1.0f - amount;
		Quaternion result;
		result.mul( from, num );
		const float num2 = Quaternion::dot( from, to );
		const float tmp = num2 >= 0.0f ? amount : -amount;

		result.x += tmp * to.x;
		result.y += tmp * to.y;
		result.z += tmp * to.z;
		result.w += tmp * to.w;

		const float rsqrt = math::rsqrt( result.lengthSq() );

		mul( result, rsqrt );
	}

	TIKI_FORCE_INLINE void Quaternion::nlerp( const Quaternion& from, const Quaternion& to, const float koeff )
	{
		const float delta = Quaternion::dot( from, to );

		Quaternion secound;
		if ( delta < 0.0f )
		{
			secound.negate( to );
		}
		else
		{
			secound = to;
		}

		Quaternion result;
		result.x = f32::lerp( from.x, secound.x, koeff );
		result.y = f32::lerp( from.y, secound.y, koeff );
		result.z = f32::lerp( from.z, secound.z, koeff );
		result.w = f32::lerp( from.w, secound.w, koeff );

		const float rsqrt = math::rsqrt( result.lengthSq() );

		mul( result, rsqrt );
	}

	TIKI_FORCE_INLINE void Quaternion::slerp( const Quaternion& from, const Quaternion& to, const float amount )
	{
		float num = Quaternion::dot( from, to );
		bool flag = false;
		if (num < 0)
		{
			flag = true;
			num = -num;
		}
		float num2;
		float num3;
		if (num > 0.999999f)
		{
			num2 = 1 - amount;
			num3 = (flag ? (-amount) : amount);
		}
		else
		{
			float num4 = acosf( num );
			float num5 = 1.0f / sinf( num4 );
			num2 = sinf( ( 1.0f - amount ) * num4 ) * num5;
			num3 = ( flag ? ( -sinf( amount * num4 ) * num5) : ( sinf( amount * num4 ) * num5 ) );
		}

		x = num2 * from.x + num3 * to.x;
		y = num2 * from.y + num3 * to.y;
		z = num2 * from.z + num3 * to.z;
		w = num2 * from.w + num3 * to.w;
	}

	TIKI_FORCE_INLINE void Quaternion::negate()
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}

	TIKI_FORCE_INLINE void Quaternion::negate( const Quaternion& quat )
	{
		x = -quat.x;
		y = -quat.y;
		z = -quat.z;
		w = -quat.w;
	}

	TIKI_FORCE_INLINE void Quaternion::add( const Quaternion& quat )
	{
		x += quat.x;
		y += quat.y;
		z += quat.z;
		w += quat.w;
	}

	TIKI_FORCE_INLINE void Quaternion::add( const Quaternion& quat1, const Quaternion& quat2 )
	{
		x = quat1.x + quat2.x;
		y = quat1.y + quat2.y;
		z = quat1.z + quat2.z;
		w = quat1.w + quat2.w;
	}

	TIKI_FORCE_INLINE void Quaternion::sub( const Quaternion& quat )
	{
		x -= quat.x;
		y -= quat.y;
		z -= quat.z;
		w -= quat.w;
	}

	TIKI_FORCE_INLINE void Quaternion::sub( const Quaternion& quat1, const Quaternion& quat2 )
	{
		x = quat1.x - quat2.x;
		y = quat1.y - quat2.y;
		z = quat1.z - quat2.z;
		w = quat1.w - quat2.w;
	}

	TIKI_FORCE_INLINE void Quaternion::mul( const Quaternion& quat )
	{
		const float num = y * quat.z - z * quat.y;
		const float num2 = z * quat.x - x * quat.z;
		const float num3 = x * quat.y - y * quat.x;
		const float num4 = x * quat.x + y * quat.y + z * quat.z;
		x = x * quat.w + quat.x * w + num;
		y = y * quat.w + quat.y * w + num2;
		z = z * quat.w + quat.z * w + num3;
		w = w * quat.w - num4;
	}

	TIKI_FORCE_INLINE void Quaternion::mul( const Quaternion& quat1, const Quaternion& quat2 )
	{
		const float num = quat1.y * quat2.z - quat1.z * quat2.y;
		const float num2 = quat1.z * quat2.x - quat1.x * quat2.z;
		const float num3 = quat1.x * quat2.y - quat1.y * quat2.x;
		const float num4 = quat1.x * quat2.x + quat1.y * quat2.y + quat1.z * quat2.z;
		x = quat1.x * quat2.w + quat2.x * quat1.w + num;
		y = quat1.y * quat2.w + quat2.y * quat1.w + num2;
		z = quat1.z * quat2.w + quat2.z * quat1.w + num3;
		w = quat1.w * quat2.w - num4;
	}

	TIKI_FORCE_INLINE void Quaternion::mul( const Quaternion& quat, const float scal )
	{
		x = quat.x * scal;
		y = quat.y * scal;
		z = quat.z * scal;
		w = quat.w * scal;
	}

	TIKI_FORCE_INLINE void Quaternion::mul( const float scalar )
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
	}

	TIKI_FORCE_INLINE void Quaternion::div( const Quaternion& quat )
	{
		const float num = Quaternion::dot( *this, quat );
		TIKI_ASSERT( !isZero( num ) );
		const float num2 = 1.0f / num;

		const float num3 = -quat.x * num2;
		const float num4 = -quat.y * num2;
		const float num5 = -quat.z * num2;
		const float num6 = quat.w * num2;

		const float num7 = y * num5 - z * num4;
		const float num8 = z * num3 - x * num5;
		const float num9 = x * num4 - y * num3;
		const float num10 = x * num3 + y * num4 + z * num5;

		x = x * num6 + num3 * w + num7;
		y = y * num6 + num4 * w + num8;
		z = z * num6 + num5 * w + num9;
		w = w * num6 - num10;
	}

	TIKI_FORCE_INLINE void Quaternion::div( const Quaternion& quat1, const Quaternion& quat2 )
	{
		const float x = quat1.x;
		const float y = quat1.y;
		const float z = quat1.z;
		const float w = quat1.w;

		const float num = Quaternion::dot( *this, quat2 );
		TIKI_ASSERT( !isZero( num ) );
		const float num2 = 1.0f / num;

		const float num3 = -quat2.x * num2;
		const float num4 = -quat2.y * num2;
		const float num5 = -quat2.z * num2;
		const float num6 = quat2.w * num2;

		const float num7 = y * num5 - z * num4;
		const float num8 = z * num3 - x * num5;
		const float num9 = x * num4 - y * num3;
		const float num10 = x * num3 + y * num4 + z * num5;

		this->x = x * num6 + num3 * w + num7;
		this->y = y * num6 + num4 * w + num8;
		this->z = z * num6 + num5 * w + num9;
		this->w = w * num6 - num10;
	}

	TIKI_FORCE_INLINE void Quaternion::div( const Quaternion& quat, const float scal )
	{
		TIKI_ASSERT( !isZero( scal ) );
		const float tmp = 1.0f / scal;
		x = quat.x * tmp;
		y = quat.y * tmp;
		z = quat.z * tmp;
		w = quat.w * tmp;
	}

	TIKI_FORCE_INLINE void Quaternion::div( const float scalar )
	{
		TIKI_ASSERT( !isZero( scalar ) );
		const float tmp = 1.0f / scalar;
		x *= tmp;
		y *= tmp;
		z *= tmp;
		w *= tmp;
	}

	TIKI_FORCE_INLINE void Quaternion::transform( Vector3& vec ) const
	{
		vec.normalize();

		Quaternion view( vec, 0 );

		view.mul( Quaternion( -x, -y, -z, w) );
		view.mul( Quaternion( x, y, z, w ), view );

		vec = Vector3( view.x, view.y, view.z );
	}

	TIKI_FORCE_INLINE void Quaternion::transform( const Vector3& vec, Vector3& result ) const
	{
		result = vec;
		result.normalize();

		Quaternion view( result, 0 );

		view.mul( Quaternion( -x, -y, -z, w) );
		view.mul( Quaternion( x, y, z, w ), view );

		result = Vector3( view.x, view.y, view.z );
	}

	TIKI_FORCE_INLINE void Quaternion::getForward( Vector3& result ) const
	{
		result.x = 2 * ( x * z + w * y );
		result.y = 2 * (y * z - w * x);
		result.z = 1 - 2 * ( x * x + y * y );
	}

	TIKI_FORCE_INLINE void Quaternion::getUp( Vector3& result ) const
	{
		result.x = 2 * (x * y - w * z);
		result.y = 1 - 2 * (x * x + z * z);
		result.z = 2 * (y * z + w * x);
	}

	TIKI_FORCE_INLINE void Quaternion::getRight( Vector3& result ) const
	{
		result.x = 1 - 2 * (y * y + z * z);
		result.y = 2 * (x * y + w * z);
		result.z = 2 * (x * z - w * y);
	}

	TIKI_FORCE_INLINE void Quaternion::getYawPitchRoll( Vector3& ypr ) const
	{
		ypr.z = atan2f( 2.0f * ( w * z + x * y ), 1.0f - 2.0f * ( z * z + x * x )); //roll
		ypr.x = asinf( 2.0f * ( w * x - y * z ));									//pitch
		ypr.y = atan2f( 2.0f * ( w * y + z * x ), 1.0f - 2.0f * ( x * x + y * y )); //yaw
	}
}

#endif // TIKI_QUATERNION_INL
