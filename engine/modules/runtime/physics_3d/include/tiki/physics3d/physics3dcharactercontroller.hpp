#pragma once
#ifndef TIKI_PHYSICS3DCHARACTERCONTROLLER_HPP_INCLUDED
#define TIKI_PHYSICS3DCHARACTERCONTROLLER_HPP_INCLUDED

#include "tiki/physics3d/physics3dcollisionobject.hpp"

#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

namespace tiki
{
	class Physics3dShape;
	struct Quaternion;
	struct Vector3;

	class Physics3dCharacterController : public Physics3dCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( Physics3dCharacterController );
		friend class Physics3dWorld;

	public:

						Physics3dCharacterController();
		virtual			~Physics3dCharacterController();

		void			create( const Physics3dShape& shape, const Vector3& position, float gravity );
		void			dispose();

		void			move( const Vector3& direction );
		void			jump();

		void			getPosition( Vector3& targetPosition ) const;
		void			getRotation( Quaternion& targetRotation ) const;

		void			setRotation( const Quaternion& rotation );

		virtual void*	getNativeObject() const TIKI_OVERRIDE TIKI_FINAL;

	private:
		
		btPairCachingGhostObject		m_ghostObject;
		btKinematicCharacterController	m_controller;

	};
}

#endif // TIKI_PHYSICS3DCHARACTERCONTROLLER_HPP_INCLUDED
