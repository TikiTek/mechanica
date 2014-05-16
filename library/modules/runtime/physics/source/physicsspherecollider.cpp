
#include "tiki/physics/physicsspherecollider.hpp"

namespace tiki
{
	PhysicsSphereCollider::PhysicsSphereCollider()
		: m_shape( 1.0f )
	{
	}

	PhysicsSphereCollider::~PhysicsSphereCollider()
	{
	}

	void PhysicsSphereCollider::create( const Vector3& position, float radius )
	{
		const btVector3 btPosition = btVector3( position.x, position.y, position.z );

		m_shape = btSphereShape( radius );

		PhysicColliderParameters parameters;
		parameters.pCollisionShape	= &m_shape;
		parameters.transform.setIdentity();
		parameters.transform.setOrigin( btPosition );

		PhysicsCollider::create( parameters );
	}

	void PhysicsSphereCollider::dispose()
	{
		PhysicsCollider::dispose();
	}
}