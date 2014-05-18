#pragma once
#ifndef __TIKI_PHYSICSBODY_HPP_INCLUDED__
#define __TIKI_PHYSICSBODY_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace tiki
{
	class PhysicsShape;
	struct Quaternion;
	struct Vector3;
	
	class PhysicsBody
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsBody );
		friend class PhysicsWorld;

	public:

							PhysicsBody();
							~PhysicsBody();

		void				create( PhysicsShape& shape, const Vector3& position, float mass );
		void				dispose();

		void				applyForce( const Vector3& direction );

		void				getPosition( Vector3& position ) const;
		void				getRotation( Quaternion& rotation ) const;
		
	private:

		btRigidBody			m_ridgidBody;

	};
}

#endif // __TIKI_PHYSICSBODY_HPP_INCLUDED__
