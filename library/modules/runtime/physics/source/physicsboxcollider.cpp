
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
		const btVector3 btPosition = btVector3( position.x, position.y, position.z );
		const btVector3 btSize = btVector3( size.x, size.y, size.z );

		m_shape = btBoxShape( btSize );

		PhysicColliderParameters parameters;
		parameters.pCollisionShape	= &m_shape;
		parameters.transform.setIdentity();
		parameters.transform.setOrigin( btPosition );

		PhysicsCollider::create( parameters );
	}

	void PhysicsBoxCollider::dispose()
	{
		PhysicsCollider::dispose();
	}
}