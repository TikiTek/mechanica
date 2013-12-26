
#include "tiki/math/matrix.hpp"

#include "tiki/math/quaternion.hpp"

namespace tiki
{
	const Matrix33 Matrix33::zero		= { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	const Matrix33 Matrix33::one		= { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	const Matrix33 Matrix33::identity	= { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };

	const Matrix43 Matrix43::zero		= { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	const Matrix43 Matrix43::one		= { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	const Matrix43 Matrix43::identity	= { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	const Matrix44 Matrix44::zero		= { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	const Matrix44 Matrix44::one		= { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	const Matrix44 Matrix44::identity	= { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	void matrix::compose( Matrix44& target, const Quaternion& rotation, const Vector3& translation, const Vector3& scale )
	{
		Matrix43 mtx;
		mtx.pos = translation;
		quaternion::toMatrix( mtx.rot, rotation );
		vector::scale( mtx.rot.x, scale.x );
		vector::scale( mtx.rot.y, scale.y );
		vector::scale( mtx.rot.z, scale.z );

		matrix::set( target, mtx );
	}

	bool matrix::decompose( Quaternion& rotation, Vector3& translation, Vector3& scale, const Matrix44& source )
	{
		//Source: Unknown
		//References: http://www.gamedev.net/community/forums/topic.asp?topic_id=441695

		//Get the translation.
		translation.x = source.x.w;
		translation.y = source.y.w;
		translation.z = source.z.w;

		//Scaling is the length of the rows.
		scale.x = sqrtf( source.x.x * source.x.x + source.x.y * source.x.y + source.x.z * source.x.z );
		scale.y = sqrtf( source.y.x * source.y.x + source.y.y * source.y.y + source.y.z * source.y.z );
		scale.z = sqrtf( source.z.x * source.z.x + source.z.y * source.z.y + source.z.z * source.z.z );

		//If any of the scaling factors are zero, than the rotation matrix can not exist.
		if ( f32::isZero( scale.x ) || f32::isZero( scale.y ) || f32::isZero( scale.z ) )
		{
			rotation = Quaternion::identity;
			return false;
		}

		//The rotation is the left over matrix after dividing out the scaling.
		Matrix33 rotationmatrix;
		rotationmatrix.x.x = source.x.x / scale.x;
		rotationmatrix.x.y = source.x.y / scale.x;
		rotationmatrix.x.z = source.x.z / scale.x;
		rotationmatrix.y.x = source.y.x / scale.y;
		rotationmatrix.y.y = source.y.y / scale.y;
		rotationmatrix.y.z = source.y.z / scale.y;
		rotationmatrix.z.x = source.z.x / scale.z;
		rotationmatrix.z.y = source.z.y / scale.z;
		rotationmatrix.z.z = source.z.z / scale.z;

		quaternion::fromMatrix( rotation, rotationmatrix );

		return true;
	}
}