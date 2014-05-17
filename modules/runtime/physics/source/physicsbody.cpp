
#include "tiki/physics/physicsbody.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/physics/physicsshape.hpp"

#include "BulletCollision/CollisionShapes/btCollisionShape.h"

#include "physicsinternal.hpp"

namespace tiki
{
	PhysicsBody::PhysicsBody()
		: m_ridgidBody( 1.0f, nullptr, nullptr )
	{
	}

	PhysicsBody::~PhysicsBody()
	{
		TIKI_ASSERT( m_ridgidBody.getUserPointer() == nullptr );
	}

	void PhysicsBody::create( PhysicsShape& shape, const Vector3& position, float mass )
	{
		dispose();

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin( toBulletVector( position ) );

		btCollisionShape* pShape = static_cast< btCollisionShape* >( shape.getNativeShape() );
		btVector3 localInertia;
		pShape->calculateLocalInertia( mass, localInertia );

		m_ridgidBody = btRigidBody( mass, nullptr, pShape, localInertia );
		m_ridgidBody.setCenterOfMassTransform( transform );
		m_ridgidBody.setUserPointer( this );
	}

	void PhysicsBody::dispose()
	{
		m_ridgidBody.setUserPointer( nullptr );

		if ( m_ridgidBody.getMotionState() != nullptr )
		{
			delete( m_ridgidBody.getMotionState() );
		}
	}

	void PhysicsBody::getPosition( Vector3& position ) const
	{
		position = toTikiVector( m_ridgidBody.getWorldTransform().getOrigin() );
	}

	void PhysicsBody::getRotation( Quaternion& rotation ) const
	{
		rotation = toTikiQuaternion( m_ridgidBody.getWorldTransform().getRotation() );
	}
}