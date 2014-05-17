
#include "tiki/physics/physicsbody.hpp"

#include "physicsinternal.hpp"

namespace tiki
{
	PhysicsBody::PhysicsBody()
		: m_ridgidBody( 1.0f, nullptr, nullptr )
	{
	}

	PhysicsBody::~PhysicsBody()
	{
	}

	void PhysicsBody::create( const PhysicsBodyParameters& parameters )
	{
		m_ridgidBody.setCollisionShape( parameters.pCollisionShape );
		m_ridgidBody.setWorldTransform( parameters.transform );
		m_ridgidBody.setMassProps( parameters.mass, toBulletVector( Vector3::zero ) );
	}

	void PhysicsBody::dispose()
	{
		if ( m_ridgidBody.getMotionState() != nullptr )
		{
			delete( m_ridgidBody.getMotionState() );
		}
	}
}