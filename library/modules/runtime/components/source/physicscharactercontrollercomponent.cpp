
#include "tiki/components/physicscharactercontrollercomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/physicscomponents_initdata.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics/physicsboxshape.hpp"
#include "tiki/physics/physicscapsuleshape.hpp"
#include "tiki/physics/physicscharactercontroller.hpp"
#include "tiki/physics/physicssphereshape.hpp"
#include "tiki/physics/physicsworld.hpp"

namespace tiki
{
	struct PhysicsCharacterControllerComponentState : public ComponentState
	{
		TransformComponentState*	pTransform;

		PhysicsComponentShape		shape;

		PhysicsCharacterController	controller;
	};

	PhysicsCharacterControllerComponent::PhysicsCharacterControllerComponent()
	{
		m_pPhysicsWorld				= nullptr;

		m_pTranformComponent		= nullptr;
	}

	PhysicsCharacterControllerComponent::~PhysicsCharacterControllerComponent()
	{
		TIKI_ASSERT( m_pPhysicsWorld		== nullptr );
		TIKI_ASSERT( m_pTranformComponent	== nullptr );
	}

	bool PhysicsCharacterControllerComponent::create( PhysicsWorld& physicsWorld, const TransformComponent& transformComponent )
	{
		m_pPhysicsWorld			= &physicsWorld;

		m_pTranformComponent	= &transformComponent;

		return true;
	}

	void PhysicsCharacterControllerComponent::dispose()
	{
		m_pPhysicsWorld			= nullptr;

		m_pTranformComponent	= nullptr;
	}

	void PhysicsCharacterControllerComponent::update()
	{
		ConstIterator componentStates = getConstIterator();

		const State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			Vector3 position;
			Quaternion rotation;			
			pState->controller.getPosition( position );
			pState->controller.getRotation( rotation );

			m_pTranformComponent->setPosition( pState->pTransform, position );
			m_pTranformComponent->setRotation( pState->pTransform, rotation );
		}
	}

	void PhysicsCharacterControllerComponent::move( PhysicsCharacterControllerComponentState* pState, const Vector3& direction ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->controller.move( direction );
	}

	void PhysicsCharacterControllerComponent::jump( PhysicsCharacterControllerComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->controller.jump();
	}

	void PhysicsCharacterControllerComponent::getPosition( Vector3& targetPosition, const PhysicsCharacterControllerComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->controller.getPosition( targetPosition );
	}

	void PhysicsCharacterControllerComponent::getRotation( Quaternion& targetRotation, const PhysicsCharacterControllerComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->controller.getRotation( targetRotation );
	}

	void PhysicsCharacterControllerComponent::setRotation( PhysicsCharacterControllerComponentState* pState, const Quaternion& rotation ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->controller.setRotation( rotation );
	}

	crc32 PhysicsCharacterControllerComponent::getTypeCrc() const
	{
		return crcString( "PhysicsCharacterControllerComponent" );
	}

	uint32 PhysicsCharacterControllerComponent::getStateSize() const
	{
		return sizeof( PhysicsCharacterControllerComponentState );
	}

	const char* PhysicsCharacterControllerComponent::getTypeName() const
	{
		return "PhysicsCharacterControllerComponent";
	}

	bool PhysicsCharacterControllerComponent::internalInitializeState( ComponentEntityIterator& componentIterator, PhysicsCharacterControllerComponentState* pState, const PhysicsCharacterControllerComponentInitData* pInitData )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		pState = new( pState ) PhysicsCharacterControllerComponentState;
		pState->pTransform	= (TransformComponentState*)componentIterator.getFirstOfType( m_pTranformComponent->getTypeId() ) ;

		PhysicsShape* pShape = createPhysicsComponentShape( pState->shape, pInitData->shape );
		if ( pShape == nullptr )
		{
			return false;
		}

		pState->controller.create( *pShape, vector::set( Vector3(), pInitData->position ) );
		m_pPhysicsWorld->addCharacterController( pState->controller );

		return true;
	}

	void PhysicsCharacterControllerComponent::internalDisposeState( PhysicsCharacterControllerComponentState* pState )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		pState->pTransform = nullptr;

		m_pPhysicsWorld->removeCharacterController( pState->controller );
		pState->controller.dispose();

		disposePhysicsComponentShape( pState->shape );

		pState->~PhysicsCharacterControllerComponentState();
	}
}