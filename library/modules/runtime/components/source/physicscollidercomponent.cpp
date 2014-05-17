
#include "tiki/components/physicscollidercomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/physicscomponents_initdata.hpp"
#include "tiki/physics/physicsboxshape.hpp"
#include "tiki/physics/physicscapsuleshape.hpp"
#include "tiki/physics/physicscollider.hpp"
#include "tiki/physics/physicssphereshape.hpp"
#include "tiki/physics/physicsworld.hpp"

namespace tiki
{
	struct PhysicsColliderComponentState : public ComponentState
	{
		PhysicsShapeType	shapeType;
		PhysicsBoxShape		boxShape;
		PhysicsCapsuleShape	capsuleShape;
		PhysicsSphereShape	sphereShape;

		PhysicsCollider		collider;
	};

	PhysicsColliderComponent::PhysicsColliderComponent()
	{
		m_pWorld = nullptr;
	}

	PhysicsColliderComponent::~PhysicsColliderComponent()
	{
		TIKI_ASSERT( m_pWorld == nullptr );
	}

	bool PhysicsColliderComponent::create( PhysicsWorld& physicsWorld )
	{
		m_pWorld = &physicsWorld;

		return true;
	}

	void PhysicsColliderComponent::dispose()
	{
		m_pWorld = nullptr;
	}

	crc32 PhysicsColliderComponent::getTypeCrc() const
	{
		return crcString( "PhysicsColliderComponent" );
	}

	uint32 PhysicsColliderComponent::getStateSize() const
	{
		return sizeof( PhysicsColliderComponentState );
	}

	const char* PhysicsColliderComponent::getTypeName() const
	{
		return "PhysicsColliderComponent";
	}

	bool PhysicsColliderComponent::internalInitializeState( ComponentEntityIterator& componentIterator, PhysicsColliderComponentState* pState, const PhysicsColliderComponentInitData* pInitData )
	{
		TIKI_ASSERT( m_pWorld != nullptr );

		pState = new( pState ) PhysicsColliderComponentState;
		pState->shapeType = pInitData->shape.shapeType;

		PhysicsShape* pShape = nullptr;
		switch ( pState->shapeType )
		{
		case ShapeType_Box:
			pState->boxShape.create( vector::set( Vector3(), pInitData->shape.shapeBoxSize ) );
			pShape = &pState->boxShape;
			break;

		case ShapeType_Capsule:
			pState->capsuleShape.create( pInitData->shape.shapeCapsuleHeight, pInitData->shape.shapeCapsuleRadius );
			pShape = &pState->capsuleShape;
			break;

		case ShapeType_Sphere:
			pState->sphereShape.create( pInitData->shape.shapeSphereRadius );
			pShape = &pState->sphereShape;
			break;

		default:
			TIKI_TRACE_ERROR( "[PhysicsColliderComponent] Could not initilize State, because of unsupported ShapeType.\n" );
			return false;
		}

		pState->collider.create( *pShape, vector::set( Vector3(), pInitData->position ) );
		m_pWorld->addCollider( pState->collider );

		return true;
	}

	void PhysicsColliderComponent::internalDisposeState( PhysicsColliderComponentState* pState )
	{
		TIKI_ASSERT( m_pWorld != nullptr );

		m_pWorld->removeCollider( pState->collider );
		pState->collider.dispose();

		switch ( pState->shapeType )
		{
		case ShapeType_Box:
			pState->boxShape.dispose();
			break;

		case ShapeType_Capsule:
			pState->capsuleShape.dispose();
			break;

		case ShapeType_Sphere:
			pState->sphereShape.dispose();
			break;

		default:
			TIKI_TRACE_ERROR( "[PhysicsColliderComponent] Could not dispose State, because of unsupported ShapeType.\n" );
			break;
		}

		pState->~PhysicsColliderComponentState();
	}
}