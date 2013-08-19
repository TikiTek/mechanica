
#include "tiki/math/quaternion.hpp"

#include "tiki/math/matrix.hpp"

#include <math.h>

namespace tiki
{
	const Quaternion	Quaternion::identity	= Quaternion( 0.0f, 0.0f, 0.0f, 1.0f );

	void Quaternion::fromMatrix( const Matrix& matrix )
	{
		Quaternion& result = *this;

		// Source: SharpDX
		float sqrt;
		float half;
		float scale = matrix.M11 + matrix.M22 + matrix.M33;

		if (scale > 0.0f)
		{
			sqrt = (float)sqrtf(scale + 1.0f);
			result.w = sqrt * 0.5f;
			sqrt = 0.5f / sqrt;

			result.x = (matrix.M23 - matrix.M32) * sqrt;
			result.y = (matrix.M31 - matrix.M13) * sqrt;
			result.z = (matrix.M12 - matrix.M21) * sqrt;
		}
		else if ((matrix.M11 >= matrix.M22) && (matrix.M11 >= matrix.M33))
		{
			sqrt = (float)sqrtf(1.0f + matrix.M11 - matrix.M22 - matrix.M33);
			half = 0.5f / sqrt;

			result.x = 0.5f * sqrt;
			result.y = (matrix.M12 + matrix.M21) * half;
			result.z = (matrix.M13 + matrix.M31) * half;
			result.w = (matrix.M23 - matrix.M32) * half;
		}
		else if (matrix.M22 > matrix.M33)
		{
			sqrt = (float)sqrtf(1.0f + matrix.M22 - matrix.M11 - matrix.M33);
			half = 0.5f / sqrt;

			result.x = (matrix.M21 + matrix.M12) * half;
			result.y = 0.5f * sqrt;
			result.z = (matrix.M32 + matrix.M23) * half;
			result.w = (matrix.M31 - matrix.M13) * half;
		}
		else
		{
			sqrt = (float)sqrtf(1.0f + matrix.M33 - matrix.M11 - matrix.M22);
			half = 0.5f / sqrt;

			result.x = (matrix.M31 + matrix.M13) * half;
			result.y = (matrix.M32 + matrix.M23) * half;
			result.z = 0.5f * sqrt;
			result.w = (matrix.M12 - matrix.M21) * half;
		}

		// :NOTE: don't use normalize because it is to inexact.
		const float len = 1.0f / sqrtf( (x * x) + (y * y) + (z * z) + (w * w) );
		x *= len;
		y *= len;
		z *= len;
		w *= len;
		TIKI_ASSERT( f32::isEquals( sqrtf( (x * x) + (y * y) + (z * z) + (w * w) ), 1.0f, 0.001f ) );
	}

	void Quaternion::fromAxisAngle( const Vector3& axis, const float radians )
	{
		const float num = radians * 0.5f;
		const float num2 = sinf( num );
		const float num3 = cosf( num );
		x = axis.x * num2;
		y = axis.y * num2;
		z = axis.z * num2;
		w = num3;
	}

	void Quaternion::fromYawPitchRoll( const float yaw, const float pitch, const float roll )
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

		x = cosroll * sinpitch * cosyaw + sinroll * cospitch * sinyaw;	//q2
		y = cosroll * cospitch * sinyaw - sinroll * sinpitch * cosyaw;	//q3
		z = sinroll * cospitch * cosyaw - cosroll * sinpitch * sinyaw;	//q1
		w = cosroll * cospitch * cosyaw + sinroll * sinpitch * sinyaw;	//q0
	}
}