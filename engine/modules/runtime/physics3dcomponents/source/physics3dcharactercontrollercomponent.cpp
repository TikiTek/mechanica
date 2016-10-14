#include "tiki/physics3dcomponents/physics3dcharactercontrollercomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components3d/transform3dcomponent.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics3d/physics3dboxshape.hpp"
#include "tiki/physics3d/physics3dcapsuleshape.hpp"
#include "tiki/physics3d/physics3dcharactercontroller.hpp"
#include "tiki/physics3d/physics3dsphereshape.hpp"
#include "tiki/physics3d/physics3dworld.hpp"

#include "physics3dcomponents_shared.hpp"

#include "components.hpp"

namespace tiki
{
	struct Physics3dCharacterControllerComponentState : public ComponentState
	{
		Transform3dComponentState*		pTransform;

		Physics3dComponentShape			shape;
		Physics3dCharacterController	controller;
	};

	Physics3dCharacterControllerComponent::Physics3dCharacterControllerComponent()
	{
		m_pPhysicsWorld				= nullptr;

		m_pTranformComponent		= nullptr;
	}

	Physics3dCharacterControllerComponent::~Physics3dCharacterControllerComponent()
	{
		TIKI_ASSERT( m_pPhysicsWorld		== nullptr );
		TIKI_ASSERT( m_pTranformComponent	== nullptr );
	}

	bool Physics3dCharacterControllerComponent::create( Physics3dWorld& physicsWorld, const Transform3dComponent& transformComponent )
	{
		m_pPhysicsWorld			= &physicsWorld;

		m_pTranformComponent	= &transformComponent;

		return true;
	}

	void Physics3dCharacterControllerComponent::dispose()
	{
		m_pPhysicsWorld			= nullptr;

		m_pTranformComponent	= nullptr;
	}

	void Physics3dCharacterControllerComponent::update()
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

	void Physics3dCharacterControllerComponent::move( Physics3dCharacterControllerComponentState* pState, const Vector3& direction ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->controller.move( direction );
	}

	void Physics3dCharacterControllerComponent::jump( Physics3dCharacterControllerComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->controller.jump();
	}

	void Physics3dCharacterControllerComponent::getPosition( Vector3& targetPosition, const Physics3dCharacterControllerComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->controller.getPosition( targetPosition );
	}

	void Physics3dCharacterControllerComponent::getRotation( Quaternion& targetRotation, const Physics3dCharacterControllerComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->controller.getRotation( targetRotation );
	}

	void Physics3dCharacterControllerComponent::setRotation( Physics3dCharacterControllerComponentState* pState, const Quaternion& rotation ) const
	{
		TIKI_ASSERT( pState != nullptr );

		pState->controller.setRotation( rotation );
	}

	const Physics3dCollisionObject& Physics3dCharacterControllerComponent::getPhysicsObject( const Physics3dCharacterControllerComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		return pState->controller;
	}

	crc32 Physics3dCharacterControllerComponent::getTypeCrc() const
	{
		return crcString( "Physics3dCharacterControllerComponent" );
	}

	uint32 Physics3dCharacterControllerComponent::getStateSize() const
	{
		return sizeof( Physics3dCharacterControllerComponentState );
	}

	const char* Physics3dCharacterControllerComponent::getTypeName() const
	{
		return "Physics3dCharacterControllerComponent";
	}

	bool Physics3dCharacterControllerComponent::internalInitializeState( ComponentEntityIterator& componentIterator, Physics3dCharacterControllerComponentState* pState, const Physics3dCharacterControllerComponentInitData* pInitData )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		pState = new( pState ) Physics3dCharacterControllerComponentState;
		pState->pTransform	= (Transform3dComponentState*)componentIterator.getFirstOfType( m_pTranformComponent->getTypeId() ) ;

		Physics3dShape* pShape = createPhysics3dComponentShape( pState->shape, pInitData->shape );
		if ( pShape == nullptr )
		{
			return false;
		}

		pState->controller.create( *pShape, vector::create( pInitData->position ), pInitData->gravity );
		m_pPhysicsWorld->addCharacterController( pState->controller );

		return true;
	}

	void Physics3dCharacterControllerComponent::internalDisposeState( Physics3dCharacterControllerComponentState* pState )
	{
		TIKI_ASSERT( m_pPhysicsWorld != nullptr );

		pState->pTransform = nullptr;

		m_pPhysicsWorld->removeCharacterController( pState->controller );
		pState->controller.dispose();

		disposePhysics3dComponentShape( pState->shape );

		pState->~Physics3dCharacterControllerComponentState();
	}
}
