#pragma once
#ifndef __TIKI_PHYSICSCHARACTERCONTROLLER_HPP_INCLUDED__
#define __TIKI_PHYSICSCHARACTERCONTROLLER_HPP_INCLUDED__

#include "tiki/physics/physicscollisionobject.hpp"

#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

namespace tiki
{
	class PhysicsShape;
	struct Quaternion;
	struct Vector3;

	class PhysicsCharacterController : public PhysicsCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsCharacterController );
		friend class PhysicsWorld;

	public:

						PhysicsCharacterController();
		virtual			~PhysicsCharacterController();

		void			create( const PhysicsShape& shape, const Vector3& position, float gravity );
		void			dispose();

		void			move( const Vector3& direction );
		void			jump();

		void			getPosition( Vector3& targetPosition ) const;
		void			getRotation( Quaternion& targetRotation ) const;

		void			setRotation( const Quaternion& rotation );

		virtual void*	getNativeObject() const;

	private:
		
		btPairCachingGhostObject		m_ghostObject;
		btKinematicCharacterController	m_controller;

	};
}

#endif // __TIKI_PHYSICSCHARACTERCONTROLLER_HPP_INCLUDED__
