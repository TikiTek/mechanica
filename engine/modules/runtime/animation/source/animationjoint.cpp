
#include "tiki/animation/animationjoint.hpp"

#include "tiki/base/functions.hpp"
#include "tiki/graphics/modelhierarchy.hpp"
#include "tiki/math/matrix.hpp"

namespace tiki
{
	void AnimationJoint::buildPoseMatrices( Matrix44* pTargetMatrix, uint targetCapacity, const AnimationJoint* pJoints, const ModelHierarchy& hierarchy )
	{
		TIKI_ASSERT( pTargetMatrix != nullptr );
		TIKI_ASSERT( pJoints != nullptr );

		const uint jointCount = TIKI_MIN( targetCapacity, hierarchy.getJointCount() );
		for (uint i = 0u; i < jointCount; ++i)
		{
			const AnimationJoint& joint = pJoints[ i ];

			matrix::compose( pTargetMatrix[ i ], joint.rotation, joint.position, joint.scale );

			const uint parentIndex = hierarchy.getParentByIndex( i );
			if ( parentIndex != ModelHierarchy::InvalidBoneIndex )
			{
				TIKI_ASSERT( parentIndex < i );
				matrix::mul( pTargetMatrix[ i ], pTargetMatrix[ parentIndex ] );
			}
		}

		for (uint i = 0u; i < jointCount; ++i)
		{
			Matrix44 mtx = pTargetMatrix[ i ];
			pTargetMatrix[ i ] = hierarchy.getSkinToBoneMatrix( i );
			matrix::mul( pTargetMatrix[ i ], mtx );
			//matrix::transpose( pTargetMatrix[ i ] );
		}
	}

	void AnimationJoint::fillJointArrayFromHierarchy( AnimationJoint* pTargetJoints, uint jointCount, const ModelHierarchy& hierarchy )
	{
		TIKI_ASSERT( pTargetJoints != nullptr );

		const vf32* pRotation	= hierarchy.getDefaultPoseRotation();
		const vf32* pPosition	= hierarchy.getDefaultPosePosition();
		const vf32* pScale		= hierarchy.getDefaultPoseScale();

		const uint count = TIKI_MIN( jointCount, hierarchy.getAlignedJointCount() );

		for (uint i = 0; i < count; ++i)
		{
			simd::get_f32( &pTargetJoints[ i ].rotation.x, pRotation[ i ] );
			simd::get_f32( &pTargetJoints[ i ].position.x, pPosition[ i ] );
			simd::get_f32( &pTargetJoints[ i ].scale.x, pScale[ i ] );
		}
	}

	void AnimationJoint::blendAnimationJoints( AnimationJoint* pTarget, const AnimationJoint* pSource0, const AnimationJoint* pSource1, uint jointCount, float blendValue )
	{
		TIKI_ASSERT( pTarget != nullptr );
		TIKI_ASSERT( pSource0 != nullptr );
		TIKI_ASSERT( pSource1 != nullptr );

		for (uint i = 0; i < jointCount; ++i)
		{
			const Quaternion rot0	= pSource0->rotation;
			const Quaternion rot1	= pSource1->rotation;
			const Vector3 pos0		= pSource0->position;
			const Vector3 pos1		= pSource1->position;
			const Vector3 scale0	= pSource0->scale;
			const Vector3 scale1	= pSource1->scale;

			Vector3 targetPos;
			vector::lerp( targetPos, pos0, pos1, blendValue );

			Quaternion targetOrientation;
			quaternion::nlerp( targetOrientation, rot0, rot1, blendValue );

			Vector3 targetScale;
			vector::lerp( targetScale, scale0, scale1, blendValue );

			pTarget->rotation	= targetOrientation;
			pTarget->position	= targetPos;
			pTarget->scale		= targetScale;

			pTarget++; 
			pSource0++; 
			pSource1++;			
		}
	}

	void AnimationJoint::blendAnimationJoints( AnimationJoint* pTarget, const AnimationJoint* pSource0, const AnimationJoint* pSource1, const float* pWeights, uint jointCount )
	{
		TIKI_ASSERT( pTarget != nullptr );
		TIKI_ASSERT( pSource0 != nullptr );
		TIKI_ASSERT( pSource1 != nullptr );
		TIKI_ASSERT( pWeights != nullptr );

		for (uint i = 0; i < jointCount; ++i)
		{
			const float blendValue = pWeights[ i ];

			const Quaternion rot0	= pSource0->rotation;
			const Quaternion rot1	= pSource1->rotation;
			const Vector3 pos0		= pSource0->position;
			const Vector3 pos1		= pSource1->position;
			const Vector3 scale0	= pSource0->scale;
			const Vector3 scale1	= pSource1->scale;

			Quaternion targetRotation;
			quaternion::nlerp( targetRotation, rot0, rot1, blendValue );

			Vector3 targetPosition;
			vector::lerp( targetPosition, pos0, pos1, blendValue );

			Vector3 targetScale;
			vector::lerp( targetScale, scale0, scale1, blendValue );

			pTarget->rotation	= targetRotation;
			pTarget->position	= targetPosition;
			pTarget->scale		= targetScale;

			pTarget++;
			pSource0++;
			pSource1++;
		}
	}
}
