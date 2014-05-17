
#include "tiki/physics/physicsboxcollider.hpp"

#include "tiki/math/vector.hpp"

#include "physicsinternal.hpp"

namespace tiki
{
	PhysicsBoxCollider::PhysicsBoxCollider()
		: m_shape( btVector3( 1.0f, 1.0f, 1.0f ) )
	{
	}

	PhysicsBoxCollider::~PhysicsBoxCollider()
	{
	}
	
	void PhysicsBoxCollider::create( const Vector3& position, const Vector3& size )
	{
		m_shape = btBoxShape( toBulletVector( size ) );

		PhysicColliderParameters parameters;
		parameters.pCollisionShape	= &m_shape;
		parameters.transform.setIdentity();
		parameters.transform.setOrigin( toBulletVector( position ) );

		PhysicsCollider::create( parameters );
	}

	void PhysicsBoxCollider::dispose()
	{
		PhysicsCollider::dispose();
	}
}