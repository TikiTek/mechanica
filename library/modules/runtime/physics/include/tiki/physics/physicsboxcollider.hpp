#pragma once
#ifndef __TIKI_PHYSICSBOXCOLLIDER_HPP_INCLUDED__
#define __TIKI_PHYSICSBOXCOLLIDER_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/physics/physicscollider.hpp"

#include "BulletCollision/CollisionShapes/btBoxShape.h"

namespace tiki
{
	struct Vector3;

	class PhysicsBoxCollider : public PhysicsCollider
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsBoxCollider );

	public:

					PhysicsBoxCollider();
					~PhysicsBoxCollider();

		void		create( const Vector3& position, const Vector3& size );
		void		dispose();

	private:

		btBoxShape	m_shape;

	};
}

#endif // __TIKI_PHYSICSBOXCOLLIDER_HPP_INCLUDED__
