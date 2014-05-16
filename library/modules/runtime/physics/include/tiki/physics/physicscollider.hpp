#pragma once
#ifndef __TIKI_PHYSICSCOLLIDER_HPP_INCLUDED__
#define __TIKI_PHYSICSCOLLIDER_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"

namespace tiki
{
	struct PhysicColliderParameters;
	
	class PhysicsCollider
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( PhysicsCollider );
		friend class PhysicsWorld;

	public:

		void				create( const PhysicColliderParameters& parameters );
		void				dispose();

	private:

		btCollisionObject	m_collitionObject;

	};
}

#endif // __TIKI_PHYSICSCOLLIDER_HPP_INCLUDED__
