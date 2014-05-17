#pragma once
#ifndef __TIKI_PHYSICSINTERNAL_HPP_INCLUDED__
#define __TIKI_PHYSICSINTERNAL_HPP_INCLUDED__

#include "tiki/base/inline.hpp"
#include "tiki/math/vector.hpp"

#include "LinearMath/btTransform.h"

namespace tiki
{
	struct PhysicColliderParameters
	{
		btCollisionShape*	pCollisionShape;
		btTransform			transform;
	};	

	struct PhysicsBodyParameters
	{
		btCollisionShape*	pCollisionShape;
		btTransform			transform;
		float				mass;
	};	

	TIKI_FORCE_INLINE btVector3 toBulletVector( const Vector3& vec )
	{
		return btVector3( vec.x, vec.y, vec.z );
	}
}

#endif // __TIKI_PHYSICSINTERNAL_HPP_INCLUDED__
