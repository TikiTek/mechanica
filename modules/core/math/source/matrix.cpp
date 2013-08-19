
#include "tiki/math/matrix.hpp"

namespace tiki
{
	const Matrix Matrix::identity = Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f );

	void Matrix::compose( const Quaternion& rotation, const Vector3& translation, const Vector3& scale )
	{
		createFromQuaternion( rotation );

		M11 *= scale.x;
		M12 *= scale.x;
		M13 *= scale.x;

		M21 *= scale.y;
		M22 *= scale.y;
		M23 *= scale.y;

		M31 *= scale.z;
		M32 *= scale.z;
		M33 *= scale.z;

		M41 = translation.x;
		M42 = translation.y;
		M43 = translation.z;
	}

	bool Matrix::decompose( Quaternion& rotation, Vector3& translation, Vector3& scale ) const
	{
		//Source: Unknown
		//References: http://www.gamedev.net/community/forums/topic.asp?topic_id=441695

		//Get the translation.
		translation.x = M41;
		translation.y = M42;
		translation.z = M43;

		//Scaling is the length of the rows.
		scale.x = (float)sqrtf((M11 * M11) + (M12 * M12) + (M13 * M13));
		scale.y = (float)sqrtf((M21 * M21) + (M22 * M22) + (M23 * M23));
		scale.z = (float)sqrtf((M31 * M31) + (M32 * M32) + (M33 * M33));

		//If any of the scaling factors are zero, than the rotation matrix can not exist.
		if ( f32::isZero( scale.x ) || f32::isZero( scale.y ) || f32::isZero( scale.z ) )
		{
			rotation = Quaternion::identity;
			return false;
		}

		//The rotation is the left over matrix after dividing out the scaling.
		Matrix rotationmatrix;
		rotationmatrix.M11 = M11 / scale.x;
		rotationmatrix.M12 = M12 / scale.x;
		rotationmatrix.M13 = M13 / scale.x;

		rotationmatrix.M21 = M21 / scale.y;
		rotationmatrix.M22 = M22 / scale.y;
		rotationmatrix.M23 = M23 / scale.y;

		rotationmatrix.M31 = M31 / scale.z;
		rotationmatrix.M32 = M32 / scale.z;
		rotationmatrix.M33 = M33 / scale.z;

		rotationmatrix.M44 = 1.0f;

		rotation.fromMatrix( rotationmatrix );

		return true;
	}
}