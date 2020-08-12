#pragma once
#ifndef __TIKI_PHYSICSBODY_HPP_INCLUDED__
#define __TIKI_PHYSICSBODY_HPP_INCLUDED__

#include "tiki/physics/physicscollisionobject.hpp"

#include "tiki/base/types.hpp"

#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace tiki
{
	class PhysicsShape;
	struct Quaternion;
	struct Vector3;
	
	class PhysicsBody : public PhysicsCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsBody );
		friend class PhysicsWorld;

	public:

							PhysicsBody();
		virtual				~PhysicsBody();

		void				create( const PhysicsShape& shape, const Vector3& position, float mass, bool freeRotation );
		void				dispose();

		void				applyForce( const Vector3& direction );

		void				getPosition( Vector3& position ) const;
		void				getRotation( Quaternion& rotation ) const;

		virtual void*		getNativeObject() const;
		
	private:

		btRigidBody			m_ridgidBody;

	};
}

#endif // __TIKI_PHYSICSBODY_HPP_INCLUDED__
