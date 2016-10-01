#include "tiki/physics3d/physics3dcharactercontroller.hpp"

#include "tiki/physics3d/physics3dshape.hpp"

#include "physics3dinternal.hpp"

#include "BulletCollision/CollisionShapes/btConvexShape.h"

namespace tiki
{	
	Physics3dCharacterController::Physics3dCharacterController()
		: m_controller( nullptr, nullptr, 1.0f )
	{
	}

	Physics3dCharacterController::~Physics3dCharacterController()
	{
	}

	void Physics3dCharacterController::create( const Physics3dShape& shape, const Vector3& position, float gravity )
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

	void Physics3dCharacterController::dispose()
	{
	}

	void Physics3dCharacterController::move( const Vector3& direction )
	{
		m_controller.setWalkDirection( toBulletVector( direction ) );
	}

	void Physics3dCharacterController::jump()
	{
		m_controller.jump();
	}

	void Physics3dCharacterController::getPosition( Vector3& targetPosition ) const
	{
		targetPosition = toTikiVector( m_ghostObject.getWorldTransform().getOrigin() );
	}

	void Physics3dCharacterController::getRotation( Quaternion& targetRotation ) const
	{
		targetRotation = toTikiQuaternion( m_ghostObject.getWorldTransform().getRotation() );
	}

	void Physics3dCharacterController::setRotation( const Quaternion& rotation )
	{
		m_ghostObject.getWorldTransform().setRotation( toBulletQuaternion( rotation ) );
	}

	void* Physics3dCharacterController::getNativeObject() const
	{
		return (void*)&m_ghostObject;
	}
}