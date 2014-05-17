
#include "tiki/physics/physicsboxbody.hpp"

#include "physicsinternal.hpp"

namespace tiki
{
	PhysicsBoxBody::PhysicsBoxBody()
		: m_shape( btVector3( 1.0f, 1.0f, 1.0f ) )
	{
	}

	PhysicsBoxBody::~PhysicsBoxBody()
	{
	}

	void PhysicsBoxBody::create( const Vector3& position, const Vector3& size, float mass )
	{
		m_shape = btBoxShape( toBulletVector( size ) );

		PhysicsBodyParameters parameters;
		parameters.pCollisionShape = &m_shape;
		parameters.transform.setIdentity();
		parameters.transform.setOrigin( toBulletVector( position ) );
		parameters.mass = mass;

		PhysicsBody::create( parameters );
	}

	void PhysicsBoxBody::dispose()
	{
		PhysicsBody::dispose();
	}
}