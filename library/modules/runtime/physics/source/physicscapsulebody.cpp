
#include "tiki/physics/physicscapsulebody.hpp"

#include "physicsinternal.hpp"

namespace tiki
{
	PhysicsCapsuleBody::PhysicsCapsuleBody()
		: m_shape( 1.0f, 1.0f )
	{
	}

	PhysicsCapsuleBody::~PhysicsCapsuleBody()
	{
	}

	void PhysicsCapsuleBody::create( const Vector3& position, float height, float radius, float mass )
	{
		m_shape = btCapsuleShape( height, radius );

		PhysicsBodyParameters parameters;
		parameters.pCollisionShape	= &m_shape;
		parameters.transform.setIdentity();
		parameters.transform.setOrigin( toBulletVector( position ) );
		parameters.mass = mass;

		PhysicsBody::create( parameters );
	}

	void PhysicsCapsuleBody::dispose()
	{
		PhysicsBody::dispose();
	}
}