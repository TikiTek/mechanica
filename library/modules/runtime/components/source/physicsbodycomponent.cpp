
#include "tiki/components/physicsbodycomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/physicscomponents_initdata.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/physics/physicsbody.hpp"
#include "tiki/physics/physicsboxshape.hpp"
#include "tiki/physics/physicscapsuleshape.hpp"
#include "tiki/physics/physicssphereshape.hpp"
#include "tiki/physics/physicsworld.hpp"

namespace tiki
{
	struct PhysicsBodyComponentState : public ComponentState
	{
		TransformComponentState*	pTransform;

		PhysicsShapeType			shapeType;
		PhysicsBoxShape				boxShape;
		PhysicsCapsuleShape			capsuleShape;
		PhysicsSphereShape			sphereShape;

		PhysicsBody					body;
	};

	PhysicsBodyComponent::PhysicsBodyComponent()
	{
		m_pWorld			= nullptr;

		m_transformTypeId	= InvalidComponentTypeId;
	}

	PhysicsBodyComponent::~PhysicsBodyComponent()
	{
		TIKI_ASSERT( m_pWorld == nullptr );
		TIKI_ASSERT( m_transformTypeId == InvalidComponentTypeId );
	}

	bool PhysicsBodyComponent::create( PhysicsWorld& physicsWorld, const TransformComponent& transformComponent )
	{
		m_pWorld = &physicsWorld;

		m_transformTypeId = transformComponent.getTypeId();
		if ( m_transformTypeId == InvalidComponentTypeId )
		{
			return false;
		}

		return true;
	}

	void PhysicsBodyComponent::dispose()
	{
		m_pWorld			= nullptr;

		m_transformTypeId	= InvalidComponentTypeId;
	}

	void PhysicsBodyComponent::update()
	{
		ConstIterator componentStates = getConstIterator();

		const State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			pState->body.getPosition( pState->pTransform->position );
			pState->body.getRotation( pState->pTransform->rotation );
		}
	}

	crc32 PhysicsBodyComponent::getTypeCrc() const
	{
		return crcString( "PhysicsBodyComponent" );
	}

	uint32 PhysicsBodyComponent::getStateSize() const
	{
		return sizeof( PhysicsBodyComponentState );
	}

	const char* PhysicsBodyComponent::getTypeName() const
	{
		return "PhysicsBodyComponent";
	}

	bool PhysicsBodyComponent::internalInitializeState( ComponentEntityIterator& componentIterator, PhysicsBodyComponentState* pState, const PhysicsBodyComponentInitData* pInitData )
	{
		TIKI_ASSERT( m_pWorld != nullptr );

		pState = new( pState ) PhysicsBodyComponentState;
		pState->shapeType	= pInitData->shape.shapeType;
		pState->pTransform	= static_cast< TransformComponentState* >( componentIterator.getFirstOfType( m_transformTypeId ) );

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
			TIKI_TRACE_ERROR( "[PhysicsBodyComponent] Could not initilize State, because of unsupported ShapeType.\n" );
			return false;
		}

		pState->body.create( *pShape, vector::set( Vector3(), pInitData->position ), pInitData->mass );
		m_pWorld->addBody( pState->body );

		return true;
	}

	void PhysicsBodyComponent::internalDisposeState( PhysicsBodyComponentState* pState )
	{
		TIKI_ASSERT( m_pWorld != nullptr );

		pState->pTransform = nullptr;

		m_pWorld->removeBody( pState->body );
		pState->body.dispose();

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
			TIKI_TRACE_ERROR( "[PhysicsBodyComponent] Could not dispose State, because of unsupported ShapeType.\n" );
			break;
		}	

		pState->~PhysicsBodyComponentState();
	}
}