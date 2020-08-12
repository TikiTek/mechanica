#pragma once
#ifndef __TIKI_PHYSICSINTERNAL_HPP_INCLUDED__
#define __TIKI_PHYSICSINTERNAL_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/math/quaternion.hpp"
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

	TIKI_FORCE_INLINE Vector3 toTikiVector( const btVector3& vec )
	{
		return vector::create( vec.x(), vec.y(), vec.z() );
	}

	TIKI_FORCE_INLINE btQuaternion toBulletQuaternion( const Quaternion& quat )
	{
		return btQuaternion( quat.x, quat.y, quat.z, quat.w );
	}

	TIKI_FORCE_INLINE Quaternion toTikiQuaternion( const btQuaternion& quat )
	{
		return quaternion::create( quat.x(), quat.y(), quat.z(), quat.w() );
	}
}

#endif // __TIKI_PHYSICSINTERNAL_HPP_INCLUDED__
