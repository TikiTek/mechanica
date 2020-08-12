#pragma once
#ifndef __TIKI_PHYSICSCOLLIDER_HPP_INCLUDED__
#define __TIKI_PHYSICSCOLLIDER_HPP_INCLUDED__

#include "tiki/physics/physicscollisionobject.hpp"

#include "tiki/base/types.hpp"

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"

namespace tiki
{
	class PhysicsShape;
	struct Vector3;
	
	class PhysicsCollider : public PhysicsCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsCollider );
		friend class PhysicsWorld;

	public:

							PhysicsCollider();
		virtual				~PhysicsCollider();

		void				create( const PhysicsShape& shape, const Vector3& position );
		void				dispose();

		virtual void*		getNativeObject() const;

	private:

		btCollisionObject	m_collitionObject;

	};
}

#endif // __TIKI_PHYSICSCOLLIDER_HPP_INCLUDED__
