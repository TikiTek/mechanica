#pragma once
#ifndef TIKI_ANIMATIONJOINT_HPP__INCLUDED
#define TIKI_ANIMATIONJOINT_HPP__INCLUDED

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

		static void		buildPoseMatrices( Matrix44* pTargetMatrix, uint targetCapacity, const AnimationJoint* pJoints, const ModelHierarchy& hierarchy );
		static void		fillJointArrayFromHierarchy( AnimationJoint* pTargetJoints, size_t jointCount, const ModelHierarchy& hierarchy );

		static void		blendAnimationJoints( AnimationJoint* pTargetJoints, const AnimationJoint* pLeftJoints, const AnimationJoint* pRightJoints, size_t jointCount, float weight );
		static void		blendAnimationJoints( AnimationJoint* pTargetJoints, const AnimationJoint* pLeftJoints, const AnimationJoint* pRightJoints, const float* pWeights, size_t jointCount );
	};
}

#endif // TIKI_ANIMATIONJOINT_HPP__INCLUDED