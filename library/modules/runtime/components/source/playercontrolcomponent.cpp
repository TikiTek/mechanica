
#include "tiki/components/playercontrolcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/physicsbodycomponent.hpp"
#include "tiki/components/playercontrolcomponent_initdata.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	struct PlayerControlComponentState : public ComponentState
	{
		TransformComponentState*	pTransform;
		PhysicsBodyComponentState*	pPhysicsBody;

		float						speed;
	};

	PlayerControlComponent::PlayerControlComponent()
	{
		m_pTransformComponent			= nullptr;
		m_pPhysicsBodyComponent			= nullptr;
		m_transformComponentTypeId		= InvalidComponentTypeId;
		m_physicsBodyComponentTypeId	= InvalidComponentTypeId;
	}

	PlayerControlComponent::~PlayerControlComponent()
	{
		TIKI_ASSERT( m_pTransformComponent			== nullptr );
		TIKI_ASSERT( m_pPhysicsBodyComponent		== nullptr );
		TIKI_ASSERT( m_transformComponentTypeId		== InvalidComponentTypeId );
		TIKI_ASSERT( m_physicsBodyComponentTypeId	== InvalidComponentTypeId );
	}

	bool PlayerControlComponent::create( const TransformComponent& transformComponent, const PhysicsBodyComponent& physicsBodyComponent )
	{
		TIKI_ASSERT( m_pTransformComponent			== nullptr );
		TIKI_ASSERT( m_pPhysicsBodyComponent		== nullptr );
		TIKI_ASSERT( m_transformComponentTypeId		== InvalidComponentTypeId );
		TIKI_ASSERT( m_physicsBodyComponentTypeId	== InvalidComponentTypeId );

		m_pTransformComponent			= &transformComponent;
		m_pPhysicsBodyComponent			= &physicsBodyComponent;

		m_transformComponentTypeId		= transformComponent.getTypeId();
		m_physicsBodyComponentTypeId	= physicsBodyComponent.getTypeId();

		vector::clear( m_inputState.leftStick );
		vector::clear( m_inputState.rightStick );

		return true;
	}

	void PlayerControlComponent::dispose()
	{
		m_pTransformComponent			= nullptr;
		m_pPhysicsBodyComponent			= nullptr;
		m_transformComponentTypeId		= InvalidComponentTypeId;
		m_physicsBodyComponentTypeId	= InvalidComponentTypeId;
	}

	void PlayerControlComponent::update( float timeStep )
	{
		Iterator componentStates = getIterator();
		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			Vector3 walkForce = { m_inputState.leftStick.x, 0.0f, m_inputState.leftStick.y };
			vector::scale( walkForce, timeStep * pState->speed );

			m_pPhysicsBodyComponent->applyForce( pState->pPhysicsBody, walkForce );
		}
	}

	bool PlayerControlComponent::processInputEvent( const InputEvent& inputEvent )
	{
		if ( inputEvent.eventType == InputEventType_Controller_StickChanged && inputEvent.deviceId == 0u )
		{
			const InputEventControllerStickData& stickData = inputEvent.data.controllerStick;
			Vector2& targetVector = ( stickData.stickIndex == 0u ? m_inputState.leftStick : m_inputState.rightStick );

			targetVector.x = stickData.xState;
			targetVector.y = stickData.yState;

			return true;
		}

		return false;
	}

	crc32 PlayerControlComponent::getTypeCrc() const
	{
		return crcString( "PlayerControlComponent" );
	}

	uint32 PlayerControlComponent::getStateSize() const
	{
		return sizeof( PlayerControlComponentState );
	}

	const char* PlayerControlComponent::getTypeName() const
	{
		return "PlayerControlComponent";
	}

	bool PlayerControlComponent::internalInitializeState( ComponentEntityIterator& componentIterator, PlayerControlComponentState* pState, const PlayerControlComponentInitData* pInitData )
	{
		pState->pTransform		= static_cast< TransformComponentState* >( static_cast< void* >( componentIterator.getFirstOfType( m_transformComponentTypeId ) ) );
		pState->pPhysicsBody	= static_cast< PhysicsBodyComponentState* >( static_cast< void* >( componentIterator.getFirstOfType( m_physicsBodyComponentTypeId ) ) );

		pState->speed = pInitData->speed;

		return true;
	}

	void PlayerControlComponent::internalDisposeState( PlayerControlComponentState* pState )
	{
		pState->pTransform		= nullptr;
		pState->pPhysicsBody	= nullptr;
	}
}