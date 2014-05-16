
#include "tiki/physics/physicscollider.hpp"

#include "physicsinternal.hpp"

namespace tiki
{
	void PhysicsCollider::create( const PhysicColliderParameters& parameters )
	{
		m_collitionObject.setCollisionShape( parameters.pCollisionShape );
		m_collitionObject.setWorldTransform( parameters.transform );
		m_collitionObject.setUserPointer( this );
	}

	void PhysicsCollider::dispose()
	{
		m_collitionObject.setUserPointer( nullptr );
	}
}