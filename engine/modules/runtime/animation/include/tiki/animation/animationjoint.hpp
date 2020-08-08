#pragma once

#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	struct Matrix44;
	struct ModelHierarchy;

	struct AnimationJoint
	{
		Quaternion		rotation;
		Vector3			position;
		Vector3			scale;

		static void		buildPoseMatrices( Matrix44* pTargetMatrix, uintreg targetCapacity, const AnimationJoint* pJoints, const ModelHierarchy& hierarchy );
		static void		fillJointArrayFromHierarchy( AnimationJoint* pTargetJoints, uintreg jointCount, const ModelHierarchy& hierarchy );

		static void		blendAnimationJoints( AnimationJoint* pTargetJoints, const AnimationJoint* pLeftJoints, const AnimationJoint* pRightJoints, uintreg jointCount, float weight );
		static void		blendAnimationJoints( AnimationJoint* pTargetJoints, const AnimationJoint* pLeftJoints, const AnimationJoint* pRightJoints, const float* pWeights, uintreg jointCount );
	};
}
