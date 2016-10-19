
#include "tiki/math/matrix.hpp"

#include "tiki/math/quaternion.hpp"

namespace tiki
{
	const Matrix22 Matrix22::zero		= { 0.0f, 0.0f, 0.0f, 0.0f };
	const Matrix22 Matrix22::one		= { 1.0f, 1.0f, 1.0f, 1.0f };
	const Matrix22 Matrix22::identity	= { 1.0f, 0.0f, 0.0f, 1.0f };
										  
	const Matrix32 Matrix32::zero		= { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	const Matrix32 Matrix32::one		= { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };
	const Matrix32 Matrix32::identity	= { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	
	const Matrix33 Matrix33::zero		= { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	const Matrix33 Matrix33::one		= { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	const Matrix33 Matrix33::identity	= { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };

	const Matrix43 Matrix43::zero		= { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	const Matrix43 Matrix43::one		= { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	const Matrix43 Matrix43::identity	= { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	const Matrix44 Matrix44::zero		= { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	const Matrix44 Matrix44::one		= { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	const Matrix44 Matrix44::identity	= { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	void matrix::compose( Matrix44& target, const Quaternion& rotation, const Vector3& position, const Vector3& scale )
	{
		Matrix43 mtx;
		mtx.pos = position;
		quaternion::toMatrix( mtx.rot, rotation );
		vector::scale( mtx.rot.x, scale.x );
		vector::scale( mtx.rot.y, scale.y );
		vector::scale( mtx.rot.z, scale.z );

		matrix::set( target, mtx );
	}

	bool matrix::decompose( Quaternion& rotation, Vector3& position, Vector3& scale, const Matrix44& source )
	{
		//Source: Unknown
		//References: http://www.gamedev.net/community/forums/topic.asp?topic_id=441695

		//Get the position.
		position.x = source.w.x;
		position.y = source.w.y;
		position.z = source.w.z;

		//Scaling is the length of the rows.
		scale.x = vector::length( source.x );
		scale.y = vector::length( source.y );
		scale.z = vector::length( source.z );

		//If any of the scaling factors are zero, than the rotation matrix can not exist.
		if ( f32::isZero( scale.x ) || f32::isZero( scale.y ) || f32::isZero( scale.z ) )
		{
			rotation = Quaternion::identity;
			return false;
		}

		//The rotation is the left over matrix after dividing out the scaling.
		Matrix33 rotationMatrix;
		rotationMatrix.x.x = source.x.x / scale.x;
		rotationMatrix.x.y = source.x.y / scale.x;
		rotationMatrix.x.z = source.x.z / scale.x;
		rotationMatrix.y.x = source.y.x / scale.y;
		rotationMatrix.y.y = source.y.y / scale.y;
		rotationMatrix.y.z = source.y.z / scale.y;
		rotationMatrix.z.x = source.z.x / scale.z;
		rotationMatrix.z.y = source.z.y / scale.z;
		rotationMatrix.z.z = source.z.z / scale.z;
		quaternion::fromMatrix( rotation, rotationMatrix );

		return true;
	}
}