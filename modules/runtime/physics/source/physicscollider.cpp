
#include "tiki/physics/physicscollider.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/physics/physicsshape.hpp"

#include "physicsinternal.hpp"

namespace tiki
{
	PhysicsCollider::PhysicsCollider()
	{
	}

	PhysicsCollider::~PhysicsCollider()
	{
		TIKI_ASSERT( m_collitionObject.getUserPointer() == nullptr );
	}

	void PhysicsCollider::create( const PhysicsShape& shape, const Vector3& position )
	{
		dispose();

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin( toBulletVector( position ) );

		m_collitionObject.setCollisionShape( static_cast< btCollisionShape* >( shape.getNativeShape() ) );
		m_collitionObject.setWorldTransform( transform );
		m_collitionObject.setUserPointer( this );
	}

	void PhysicsCollider::dispose()
	{
		m_collitionObject.setUserPointer( nullptr );
	}

	void* PhysicsCollider::getNativeObject() const
	{
		return (void*)&m_collitionObject;
	}
}