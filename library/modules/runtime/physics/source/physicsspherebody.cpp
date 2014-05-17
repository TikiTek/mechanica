
#include "tiki/physics/physicsspherebody.hpp"

namespace tiki
{
	PhysicsSphereBody::PhysicsSphereBody()
		: m_shape( 1.0f )
	{
	}

	PhysicsSphereBody::~PhysicsSphereBody()
	{
	}

	void PhysicsSphereBody::create( const Vector3& position, float radius, float mass )
	{
		m_shape = btSphereShape( radius );

		PhysicsBodyParameters parameters;
		parameters.pCollisionShape = &m_shape;
		parameters.transform.setIdentity();
		parameters.transform.setOrigin( toBulletVector( position ) );
		parameters.mass = mass;

		PhysicsBody::create( parameters );
	}

	void PhysicsSphereBody::dispose()
	{
		PhysicsBody::dispose();
	}
}