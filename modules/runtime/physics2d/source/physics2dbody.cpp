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

	void PhysicsBody::create( const PhysicsShape& shape, const Vector3& position, float mass, bool freeRotation )
	{
		dispose();

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin( toBulletVector( position ) );

		btCollisionShape* pShape = static_cast< btCollisionShape* >( shape.getNativeShape() );
		btVector3 localInertia;
		if ( freeRotation )
		{
			pShape->calculateLocalInertia( mass, localInertia );
		}
		else
		{
			localInertia = toBulletVector( Vector3::zero );
		}

		m_ridgidBody = btRigidBody( mass, nullptr, pShape, localInertia );
		m_ridgidBody.setCenterOfMassTransform( transform );
		m_ridgidBody.setUserPointer( this );
		m_ridgidBody.setActivationState( 1u );
	}

	void PhysicsBody::dispose()
	{
		m_ridgidBody.setUserPointer( nullptr );

		if ( m_ridgidBody.getMotionState() != nullptr )
		{
			delete( m_ridgidBody.getMotionState() );
		}
	}

	void PhysicsBody::applyForce( const Vector3& force )
	{
		m_ridgidBody.applyCentralImpulse( toBulletVector( force ) );
	}

	void PhysicsBody::getPosition( Vector3& position ) const
	{
		position = toTikiVector( m_ridgidBody.getWorldTransform().getOrigin() );
	}

	void PhysicsBody::getRotation( Quaternion& rotation ) const
	{
		rotation = toTikiQuaternion( m_ridgidBody.getWorldTransform().getRotation() );
	}

	void* PhysicsBody::getNativeObject() const
	{
		return (void*)&m_ridgidBody;
	}
}