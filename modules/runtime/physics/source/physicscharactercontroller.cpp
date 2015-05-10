
#include "tiki/physics/physicscharactercontroller.hpp"

#include "tiki/physics/physicsshape.hpp"

#include "physicsinternal.hpp"

#include "BulletCollision/CollisionShapes/btConvexShape.h"

namespace tiki
{	
	PhysicsCharacterController::PhysicsCharacterController()
		: m_controller( nullptr, nullptr, 1.0f )
	{
	}

	PhysicsCharacterController::~PhysicsCharacterController()
	{
	}

	void PhysicsCharacterController::create( const PhysicsShape& shape, const Vector3& position, float gravity )
	{
		btConvexShape* pShape = static_cast< btConvexShape* >( shape.getNativeShape() );

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin( toBulletVector( position ) );
		
		m_ghostObject.setWorldTransform( transform );
		m_ghostObject.setCollisionShape( pShape );
		m_ghostObject.setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );

		m_controller = btKinematicCharacterController( &m_ghostObject, pShape, 0.35f );
		m_controller.setGravity( gravity );
	}

	void PhysicsCharacterController::dispose()
	{
	}

	void PhysicsCharacterController::move( const Vector3& direction )
	{
		m_controller.setWalkDirection( toBulletVector( direction ) );
	}

	void PhysicsCharacterController::jump()
	{
		m_controller.jump();
	}

	void PhysicsCharacterController::getPosition( Vector3& targetPosition ) const
	{
		targetPosition = toTikiVector( m_ghostObject.getWorldTransform().getOrigin() );
	}

	void PhysicsCharacterController::getRotation( Quaternion& targetRotation ) const
	{
		targetRotation = toTikiQuaternion( m_ghostObject.getWorldTransform().getRotation() );
	}

	void PhysicsCharacterController::setRotation( const Quaternion& rotation )
	{
		m_ghostObject.getWorldTransform().setRotation( toBulletQuaternion( rotation ) );
	}

	void* PhysicsCharacterController::getNativeObject() const
	{
		return (void*)&m_ghostObject;
	}
}