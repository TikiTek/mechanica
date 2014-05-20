
#include "tiki/components/physicsbodycomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/physicscomponents_initdata.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics/physicsbody.hpp"
#include "tiki/physics/physicsboxshape.hpp"
#include "tiki/physics/physicscapsuleshape.hpp"
#include "tiki/physics/physicssphereshape.hpp"
#include "tiki/physics/physicsworld.hpp"

#include "physicscomponents_shared.hpp"

namespace tiki
{
	struct PhysicsBodyComponentState : public ComponentState
	{
		TransformComponentState*	pTransform;

		PhysicsComponentShape		shape;

		PhysicsBody					body;
	};

	PhysicsBodyComponent::PhysicsBodyComponent()
	{
		m_pPhysicsWorld				= nullptr;

		m_pTranformComponent		= nullptr;
		m_transformComponentTypeId	= InvalidComponentTypeId;
	}

	PhysicsBodyComponent::~PhysicsBodyComponent()
	{
		TIKI_ASSERT( m_pPhysicsWorld			== nullptr );
		TIKI_ASSERT( m_pTranformComponent		== nullptr );
		TIKI_ASSERT( m_transformComponentTypeId	== InvalidComponentTypeId );
	}

	bool PhysicsBodyComponent::create( PhysicsWorld& physicsWorld, const TransformComponent& transformComponent )
	{
		m_pPhysicsWorld			= &physicsWorld;

		m_pTranformComponent		= &transformComponent;
		m_transformComponentTypeId	= transformComponent.getTypeId();
		if ( m_transformComponentTypeId == InvalidComponentTypeId )
		{
			return false;
		}

		return true;
	}

	void PhysicsBodyComponent::dispose()
	{
		m_pPhysicsWorld				= nullptr;

		m_pTranformComponent		= nullptr;
		m_transformComponentTypeId	= InvalidComponentTypeId;
	}

	void PhysicsBodyComponent::update()
	{
		ConstIterator componentStates = getConstIterator();

		const State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			Vector3 position;
			Quaternion rotation;			
			pState->body.getPosition( position );
			pState->body.getRotation( rotation );

			m_pTranformComponent->setPosition( pState->pTransform, position );
			m_pTranformComponent->setRotation( pState->pTransform, rotation );
		}
	}

	void PhysicsBodyComponent::applyForce( PhysicsBodyComponentState* pState, const Vector3& force ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->body.applyForce( force );
	}

	void PhysicsBodyComponent::getPosition( Vector3& targetPosition, const PhysicsBodyComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->body.getPosition( targetPosition );
	}

	void PhysicsBodyComponent::getRotation( Quaternion& targetRotation, const PhysicsBodyComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->body.getRotation( targetRotation );
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
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		pState = new( pState ) PhysicsBodyComponentState;
		pState->pTransform	= static_cast< TransformComponentState* >( static_cast< void* >( componentIterator.getFirstOfType( m_transformComponentTypeId ) ) );

		PhysicsShape* pShape = createPhysicsComponentShape( pState->shape, pInitData->shape );
		if ( pShape == nullptr )
		{
			return false;
		}

		pState->body.create( *pShape, vector::set( Vector3(), pInitData->position ), pInitData->mass, pInitData->freeRotation );
		m_pPhysicsWorld->addBody( pState->body );

		return true;
	}

	void PhysicsBodyComponent::internalDisposeState( PhysicsBodyComponentState* pState )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		pState->pTransform = nullptr;

		m_pPhysicsWorld->removeBody( pState->body );
		pState->body.dispose();

		disposePhysicsComponentShape( pState->shape );

		pState->~PhysicsBodyComponentState();
	}
}