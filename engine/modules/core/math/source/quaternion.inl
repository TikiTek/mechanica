#ifndef TIKI_QUATERNION_INL
#define TIKI_QUATERNION_INL



namespace tiki
{
	TIKI_FORCE_INLINE bool quaternion::isZero( const Quaternion& quat, float epsilon /* = f32::epsilon */ )
	{
		return f32::isZero( quat.x, epsilon ) && f32::isZero( quat.y, epsilon ) && f32::isZero( quat.z, epsilon ) && f32::isZero( quat.w, epsilon );
	}

	TIKI_FORCE_INLINE bool quaternion::isEquals( const Quaternion& lhs, const Quaternion& rhs, float epsilon /* = f32::epsilon */ )
	{
		return f32::isEquals( lhs.x, rhs.x, epsilon ) && f32::isEquals( lhs.y, rhs.y, epsilon ) && f32::isEquals( lhs.z, rhs.z, epsilon ) && f32::isEquals( lhs.w, rhs.w, epsilon );
	}

	TIKI_FORCE_INLINE bool quaternion::isIdentity( const Quaternion& quat, float epsilon /* = f32::epsilon */ )
	{
		return quaternion::isEquals( quat, Quaternion::identity, epsilon );
	}

	TIKI_FORCE_INLINE Quaternion quaternion::create( float x, float y, float z, float w )
	{
		const Quaternion quat = { x, y, z, w };
		return quat;
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::clear( Quaternion& quat )
	{
		quat.x = 0.0f;
		quat.y = 0.0f;
		quat.z = 0.0f;
		quat.w = 0.0f;
		return quat;
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::createIdentity( Quaternion& quat )
	{
		quat = Quaternion::identity;
		return quat;
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::set( Quaternion& quat, const float4& source )
	{
		quat.x = source.x;
		quat.y = source.y;
		quat.z = source.z;
		quat.w = source.w;
		return quat;
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::set( Quaternion& quat, float x, float y, float z, float w )
	{
		quat.x = x;
		quat.y = y;
		quat.z = z;
		quat.w = w;
		return quat;
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::add( Quaternion& quat, const Quaternion& rhs )
	{
		quat.x += rhs.x;
		quat.y += rhs.y;
		quat.z += rhs.z;
		quat.w += rhs.w;
		return quat;
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::sub( Quaternion& quat, const Quaternion& rhs )
	{
		quat.x -= rhs.x;
		quat.y -= rhs.y;
		quat.z -= rhs.z;
		quat.w -= rhs.w;
		return quat;
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::mul( Quaternion& quat, const Quaternion& rhs )
	{
		const float lx = quat.x;
		const float ly = quat.y;
		const float lz = quat.z;
		const float lw = quat.w;

		const float rx = rhs.x;
		const float ry = rhs.y;
		const float rz = rhs.z;
		const float rw = rhs.w;

		quat.x = ( lx * rw + rx * lw ) + ( ly * rz - lz * ry );
		quat.y = ( ly * rw + ry * lw ) + ( lz * rx - lx * rz );
		quat.z = ( lz * rw + rz * lw ) + ( lx * ry - ly * rx );
		quat.w = ( lw * rw ) - ( lx * rx + ly * ry + lz * rz );

		return quat;
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::div( Quaternion& quat, const Quaternion& rhs )
	{
		const float x = quat.x;
		const float y = quat.y;
		const float z = quat.z;
		const float w = quat.w;
		const float dotVal = quaternion::dot( quat, rhs );
		TIKI_ASSERT( f32::isZero( dotVal ) == false );
		const float inverseDotVal = 1.0f / dotVal;
		const float raX = -rhs.x * inverseDotVal;
		const float raY = -rhs.y * inverseDotVal;
		const float raZ = -rhs.z * inverseDotVal;
		const float raW = rhs.w * inverseDotVal;
		const float rbX = y * raZ - z * raY;
		const float rbY = z * raX - x * raZ;
		const float rbZ = x * raY - y * raX;
		const float rbW = x * raX + y * raY + z * raZ;
		quat.x = x * raW + raX * w + rbX;
		quat.y = y * raW + raY * w + rbY;
		quat.z = z * raW + raZ * w + rbZ;
		quat.w = w * raW - rbW;
		return quat;
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::scale( Quaternion& quat, float val )
	{
		quat.x *= val;
		quat.y *= val;
		quat.z *= val;
		quat.w *= val;
		return quat;			 
	}

	TIKI_FORCE_INLINE float quaternion::length( const Quaternion& quat )
	{
		return f32::sqrt( quaternion::lengthSquared( quat ) );
	}

	TIKI_FORCE_INLINE float quaternion::lengthSquared( const Quaternion& quat )
	{
		return dot( quat, quat );
	}

	TIKI_FORCE_INLINE float quaternion::dot( const Quaternion& quat1, const Quaternion& quat2 )
	{
		return quat1.x * quat2.x + quat1.y * quat2.y + quat1.z * quat2.z + quat1.w * quat2.w;
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::negate( Quaternion& quat )	
	{
		quat.x = -quat.x;
		quat.y = -quat.y;
		quat.z = -quat.z;
		quat.w = -quat.w;
		return quat;
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::conjugate( Quaternion& quat )
	{
		quat.x = -quat.x;
		quat.y = -quat.y;
		quat.z = -quat.z;
		return quat;
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::normalize( Quaternion& quat )
	{
		TIKI_ASSERT( f32::isZero( quaternion::lengthSquared( quat ) ) == false );
		return quaternion::scale(
			quat,
			f32::rsqrt( quaternion::lengthSquared( quat ) )
		);
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::nlerp( Quaternion& quat, const Quaternion& start, const Quaternion& end, const float amount )
	{
		const float delta = quaternion::dot( start, end );
		Quaternion secound = end;
		if ( delta < 0.0f )
		{
			quaternion::negate( secound );
		}
		quat.x = f32::lerp( start.x, secound.x, amount );
		quat.y = f32::lerp( start.y, secound.y, amount );
		quat.z = f32::lerp( start.z, secound.z, amount );
		quat.w = f32::lerp( start.w, secound.w, amount );
		return normalize( quat );
	}

	TIKI_FORCE_INLINE Quaternion& quaternion::slerp( Quaternion& quat, const Quaternion& start, const Quaternion& end, const float amount )
	{
		float opposite;
		float inverse;
		const float dotVal = quaternion::dot( start, end );
		const float dotAbs = f32::abs( dotVal );

		if ( dotVal >= 0.999999f )
		{
			inverse = 1.0f - amount;
			opposite = amount * f32::sign( dotVal );
		}
		else
		{
			const float acos = acosf( dotAbs );
			const float invSin = 1.0f / sinf( acos );

			inverse = sinf( ( 1.0f - amount ) * acos ) * invSin;
			opposite = sinf( amount * acos ) * invSin * f32::sign( dotVal );
		}

		quat.x = inverse * start.x + opposite * end.x;
		quat.y = inverse * start.y + opposite * end.y;
		quat.z = inverse * start.z + opposite * end.z;
		quat.w = inverse * start.w + opposite * end.w;
		return quaternion::normalize( quat );
	}

	TIKI_FORCE_INLINE void quaternion::createLookAt( Quaternion& quat, const Vector3& position, const Vector3& target, const Vector3& up /*= Vector3::unitY*/ )
	{		
		Vector3 forward = target;
		vector::normalize( vector::sub( forward, position ) );

		const float dot = vector::dot( Vector3::unitZ, forward );

		if (f32::abs( dot + 1.0f ) < f32::epsilon)
		{
			quat.x = up.x;
			quat.x = up.y;
			quat.x = up.z;
			quat.x = f32::pi;

			return;
		}
		if (f32::abs( dot - 1.0f ) < f32::epsilon)
		{
			quat = Quaternion::identity;

			return;
		}

		const float rotAngle = f32::acos( dot );
		Vector3 rotAxis;
		vector::normalize( vector::cross( rotAxis, Vector3::unitZ, forward ) );

		fromAxisAngle( quat, rotAxis, rotAngle );
	}

	TIKI_FORCE_INLINE void quaternion::fromMatrix( Quaternion& result, const Matrix33& mtx )
	{
		// Source: SharpDX
		const float scale = mtx.x.x + mtx.y.y + mtx.z.z;
		if ( scale > 0.0f )
		{
			const float sqrt1 = sqrtf( scale + 1.0f );
			const float sqrt2 = 0.5f / sqrt1;
			result.w = sqrt1 * 0.5f;

			result.x = ( mtx.y.z - mtx.z.y ) * sqrt2;
			result.y = ( mtx.z.x - mtx.x.z ) * sqrt2;
			result.z = ( mtx.x.y - mtx.y.x ) * sqrt2;
		}
		else if ( mtx.x.x >= mtx.y.y && mtx.x.x >= mtx.z.z )
		{
			const float sqrt = sqrtf( 1.0f + mtx.x.x - mtx.y.y - mtx.z.z );
			const float half = 0.5f / sqrt;

			result.x = 0.5f * sqrt;
			result.y = ( mtx.x.y + mtx.y.x ) * half;
			result.z = ( mtx.x.z + mtx.z.x ) * half;
			result.w = ( mtx.y.z - mtx.z.y ) * half;
		}
		else if ( mtx.y.y > mtx.z.z )
		{
			const float sqrt = sqrtf( 1.0f + mtx.y.y - mtx.x.x - mtx.z.z );
			const float half = 0.5f / sqrt;

			result.x = ( mtx.y.x + mtx.x.y ) * half;
			result.y = 0.5f * sqrt;
			result.z = ( mtx.z.y + mtx.y.z ) * half;
			result.w = ( mtx.z.x - mtx.x.z ) * half;
		}
		else
		{
			const float sqrt = sqrtf( 1.0f + mtx.z.z - mtx.x.x - mtx.y.y );
			const float half = 0.5f / sqrt;

			result.x = ( mtx.z.x + mtx.x.z ) * half;
			result.y = ( mtx.z.y + mtx.y.z ) * half;
			result.z = 0.5f * sqrt;
			result.w = ( mtx.x.y - mtx.y.x ) * half;
		}

		quaternion::normalize( result );
	}

	TIKI_FORCE_INLINE void quaternion::toMatrix( Matrix33& mtx, const Quaternion& quat )
	{
		const float xx = quat.x * quat.x;
		const float yy = quat.y * quat.y;
		const float zz = quat.z * quat.z;
		const float xy = quat.x * quat.y;
		const float zw = quat.z * quat.w;
		const float zx = quat.z * quat.x;
		const float yw = quat.y * quat.w;
		const float yz = quat.y * quat.z;
		const float xw = quat.x * quat.w;

		matrix::createIdentity( mtx );
		mtx.x.x = 1.0f - ( 2.0f * ( yy + zz ) );
		mtx.x.y = 2.0f * ( xy + zw );
		mtx.x.z = 2.0f * ( zx - yw );
		mtx.y.x = 2.0f * ( xy - zw );
		mtx.y.y = 1.0f - ( 2.0f * ( zz + xx ) );
		mtx.y.z = 2.0f * ( yz + xw );
		mtx.z.x = 2.0f * ( zx + yw );
		mtx.z.y = 2.0f * ( yz - xw );
		mtx.z.z = 1.0f - ( 2.0f * ( yy + xx ) );
	}
	
	TIKI_FORCE_INLINE void quaternion::toMatrix( Matrix44& mtx, const Quaternion& quat )
	{
		const float xx = quat.x * quat.x;
		const float yy = quat.y * quat.y;
		const float zz = quat.z * quat.z;
		const float xy = quat.x * quat.y;
		const float zw = quat.z * quat.w;
		const float zx = quat.z * quat.x;
		const float yw = quat.y * quat.w;
		const float yz = quat.y * quat.z;
		const float xw = quat.x * quat.w;

		matrix::createIdentity( mtx );
		mtx.x.x = 1.0f - (2.0f * (yy + zz));
		mtx.x.y = 2.0f * (xy + zw);
		mtx.x.z = 2.0f * (zx - yw);
		mtx.y.x = 2.0f * (xy - zw);
		mtx.y.y = 1.0f - (2.0f * (zz + xx));
		mtx.y.z = 2.0f * (yz + xw);
		mtx.z.x = 2.0f * (zx + yw);
		mtx.z.y = 2.0f * (yz - xw);
		mtx.z.z = 1.0f - (2.0f * (yy + xx));
	}

	TIKI_FORCE_INLINE void quaternion::fromYawPitchRoll( Quaternion& quat, float yaw, float pitch, float roll )
	{
		const float halfroll	= roll * 0.5f;
		const float sinroll		= sinf( halfroll );
		const float cosroll		= cosf( halfroll );

		const float halfpitch	= pitch * 0.5f;
		const float sinpitch	= sinf( halfpitch );
		const float cospitch	= cosf( halfpitch );

		const float halfyaw		= yaw * 0.5f;
		const float sinyaw		= sinf( halfyaw );
		const float cosyaw		= cosf( halfyaw );

		quat.x = cosroll * sinpitch * cosyaw + sinroll * cospitch * sinyaw;
		quat.y = cosroll * cospitch * sinyaw - sinroll * sinpitch * cosyaw;
		quat.z = sinroll * cospitch * cosyaw - cosroll * sinpitch * sinyaw;
		quat.w = cosroll * cospitch * cosyaw + sinroll * sinpitch * sinyaw;
	}

	TIKI_FORCE_INLINE void quaternion::toYawPitchRoll( Vector3& yawPitchRoll, const Quaternion& quat )
	{
		yawPitchRoll.z = atan2f( 2.0f * ( quat.w * quat.z + quat.x * quat.y ), 1.0f - 2.0f * ( quat.z * quat.z + quat.x * quat.x ));	// roll
		yawPitchRoll.x = asinf( 2.0f * ( quat.w * quat.x - quat.y * quat.z ));															// pitch
		yawPitchRoll.y = atan2f( 2.0f * ( quat.w * quat.y + quat.z * quat.x ), 1.0f - 2.0f * ( quat.x * quat.x + quat.y * quat.y ));	// yaw
	}

	TIKI_FORCE_INLINE void quaternion::fromAxisAngle( Quaternion& quat, const Vector3& axis, float angle )
	{
		Vector3 normalized = axis;
		vector::normalize( normalized );

		float half = angle * 0.5f;
		float sin = f32::sin( half );
		float cos = f32::cos( half );

		quat.x = normalized.x * sin;
		quat.y = normalized.y * sin;
		quat.z = normalized.z * sin;
		quat.w = cos;
	}

	TIKI_FORCE_INLINE void quaternion::transform( Vector3& vec, const Quaternion& quat )
	{
		const float x = quat.x + quat.x;
		const float y = quat.y + quat.y;
		const float z = quat.z + quat.z;

		const float wx = quat.w * x;
		const float wy = quat.w * y;
		const float wz = quat.w * z;
		const float xx = quat.x * x;
		const float xy = quat.x * y;
		const float xz = quat.x * z;
		const float yy = quat.y * y;
		const float yz = quat.y * z;
		const float zz = quat.z * z;

		const float rx = ( ( vec.x * ( ( 1.0f - yy ) - zz ) ) + ( vec.y * ( xy - wz ) ) ) + ( vec.z * ( xz + wy ) );
		const float ry = ( ( vec.x * ( xy + wz ) ) + ( vec.y * ( ( 1.0f - xx ) - zz ) ) ) + ( vec.z * ( yz - wx ) );
		const float rz = ( ( vec.x * ( xz - wy ) ) + ( vec.y * ( yz + wx ) ) ) + ( vec.z * ( ( 1.0f - xx ) - yy ) );

		vector::set( vec, rx, ry, rz );
	}

	TIKI_FORCE_INLINE void quaternion::getForward( Vector3& result, const Quaternion& quat )
	{
		quaternion::getBackward( result, quat );
		vector::negate( result );
	}

	TIKI_FORCE_INLINE void quaternion::getBackward( Vector3& result, const Quaternion& quat )
	{
		result.x = 2.0f * ( quat.x * quat.z + quat.w * quat.y );
		result.y = 2.0f * ( quat.y * quat.z - quat.w * quat.x );
		result.z = 1.0f - 2.0f * ( quat.x * quat.x + quat.y * quat.y );
	}

	TIKI_FORCE_INLINE void quaternion::getLeft( Vector3& result, const Quaternion& quat )
	{
		result.x = 1.0f - 2.0f * ( quat.y * quat.y + quat.z * quat.z );
		result.y = 2.0f * ( quat.x * quat.y + quat.w * quat.z );
		result.z = 2.0f * ( quat.x * quat.z - quat.w * quat.y );
	}

	TIKI_FORCE_INLINE void quaternion::getRight( Vector3& result, const Quaternion& quat )
	{
		quaternion::getLeft( result, quat );
		vector::negate( result );
	}

	TIKI_FORCE_INLINE void quaternion::getUp( Vector3& result, const Quaternion& quat )
	{
		quaternion::getDown( result, quat );
		vector::negate( result );
	}

	TIKI_FORCE_INLINE void quaternion::getDown( Vector3& result, const Quaternion& quat )
	{
		result.x = 2.0f * ( quat.x * quat.y - quat.w * quat.z );
		result.y = 1.0f - 2.0f * ( quat.x * quat.x + quat.z * quat.z );
		result.z = 2.0f * ( quat.y * quat.z + quat.w * quat.x );
	}
}

#endif // TIKI_QUATERNION_INL
