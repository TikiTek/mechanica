#pragma once
#ifndef __TIKI_PHYSICSSPHERECOLLIDER_HPP_INCLUDED__
#define __TIKI_PHYSICSSPHERECOLLIDER_HPP_INCLUDED__

#include "tiki/physics/physicscollider.hpp"

#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"

namespace tiki
{
	class PhysicsSphereCollider : public PhysicsCollider
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsSphereCollider );

	public:

						PhysicsSphereCollider();
						~PhysicsSphereCollider();

		void			create( const Vector3& position, float radius );
		void			dispose();

	private:

		btSphereShape	m_shape;

	};
}

#endif // __TIKI_PHYSICSSPHERECOLLIDER_HPP_INCLUDED__
