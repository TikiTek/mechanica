#include "tiki/physics3d/physics3dbody.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/physics3d/physics3dshape.hpp"

#include "BulletCollision/CollisionShapes/btCollisionShape.h"

#include "physics3dinternal.hpp"

namespace tiki
{
	Physics3dBody::Physics3dBody()
		: m_ridgidBody( 1.0f, nullptr, nullptr )
	{
	}

	Physics3dBody::~Physics3dBody()
	{
		TIKI_ASSERT( m_ridgidBody.getUserPointer() == nullptr );
	}

	void Physics3dBody::create( const Physics3dShape& shape, const Vector3& position, float mass, bool freeRotation )
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

	void Physics3dBody::dispose()
	{
		m_ridgidBody.setUserPointer( nullptr );

		if ( m_ridgidBody.getMotionState() != nullptr )
		{
			delete( m_ridgidBody.getMotionState() );
		}
	}

	void Physics3dBody::applyForce( const Vector3& force )
	{
		m_ridgidBody.applyCentralImpulse( toBulletVector( force ) );
	}

	void Physics3dBody::getPosition( Vector3& position ) const
	{
		position = toTikiVector( m_ridgidBody.getWorldTransform().getOrigin() );
	}

	void Physics3dBody::getRotation( Quaternion& rotation ) const
	{
		rotation = toTikiQuaternion( m_ridgidBody.getWorldTransform().getRotation() );
	}

	void* Physics3dBody::getNativeObject() const
	{
		return (void*)&m_ridgidBody;
	}
}