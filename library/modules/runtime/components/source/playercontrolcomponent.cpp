
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
		TransformComponentState*					pTransform;
		PhysicsCharacterControllerComponentState*	pPhysicsController;

		float										speed;
		Vector2										rotation;
		Quaternion									positionRotation;
	};

	PlayerControlComponent::PlayerControlComponent()
	{
		m_pTransformComponent						= nullptr;
		m_pPhysicsCharacterControllerComponent		= nullptr;
		m_transformComponentTypeId					= InvalidComponentTypeId;
		m_physicsCharacterControllerComponentTypeId	= InvalidComponentTypeId;
	}

	PlayerControlComponent::~PlayerControlComponent()
	{
		TIKI_ASSERT( m_pTransformComponent							== nullptr );
		TIKI_ASSERT( m_pPhysicsCharacterControllerComponent			== nullptr );
		TIKI_ASSERT( m_transformComponentTypeId						== InvalidComponentTypeId );
		TIKI_ASSERT( m_physicsCharacterControllerComponentTypeId	== InvalidComponentTypeId );
	}

	bool PlayerControlComponent::create( const TransformComponent& transformComponent, const PhysicsCharacterControllerComponent& physicsCharacterControllerComponent )
	{
		TIKI_ASSERT( m_pTransformComponent							== nullptr );
		TIKI_ASSERT( m_pPhysicsCharacterControllerComponent			== nullptr );
		TIKI_ASSERT( m_transformComponentTypeId						== InvalidComponentTypeId );
		TIKI_ASSERT( m_physicsCharacterControllerComponentTypeId	== InvalidComponentTypeId );

		m_pTransformComponent					= &transformComponent;
		m_pPhysicsCharacterControllerComponent	= &physicsCharacterControllerComponent;

		m_transformComponentTypeId					= transformComponent.getTypeId();
		m_physicsCharacterControllerComponentTypeId	= physicsCharacterControllerComponent.getTypeId();

		vector::clear( m_inputState.leftStick );
		vector::clear( m_inputState.rightStick );

		return true;
	}

	void PlayerControlComponent::dispose()
	{
		m_pTransformComponent						= nullptr;
		m_pPhysicsCharacterControllerComponent		= nullptr;
		m_transformComponentTypeId					= InvalidComponentTypeId;
		m_physicsCharacterControllerComponentTypeId	= InvalidComponentTypeId;
	}

	void PlayerControlComponent::update( float timeDelta )
	{
		Iterator componentStates = getIterator();
		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			Vector2 rotationOffset = m_inputState.rightStick;
			vector::scale( rotationOffset, timeDelta );
			vector::add( pState->rotation, rotationOffset );
			pState->rotation.y = f32::clamp( pState->rotation.y, -f32::piOver2, f32::piOver2 );
			quaternion::fromYawPitchRoll( pState->positionRotation, pState->rotation.x, 0.0f, 0.0f );

			Vector3 walkForce = { m_inputState.leftStick.x, 0.0f, m_inputState.leftStick.y };
			vector::scale( walkForce, -pState->speed );
			quaternion::transform( walkForce, pState->positionRotation );

			m_pPhysicsCharacterControllerComponent->move( pState->pPhysicsController, walkForce );

			if ( m_inputState.jump )
			{
				m_pPhysicsCharacterControllerComponent->jump( pState->pPhysicsController );
				m_inputState.jump = false;
			}
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
		else if ( inputEvent.eventType == InputEventType_Controller_ButtonDown && inputEvent.data.controllerButton.button == ControllerButton_A && inputEvent.deviceId == 0u )
		{
			m_inputState.jump = true;
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
		pState->pTransform			= static_cast< TransformComponentState* >( static_cast< void* >( componentIterator.getFirstOfType( m_transformComponentTypeId ) ) );
		pState->pPhysicsController	= static_cast< PhysicsCharacterControllerComponentState* >( static_cast< void* >( componentIterator.getFirstOfType( m_physicsCharacterControllerComponentTypeId ) ) );

		pState->speed = pInitData->speed;
		vector::clear( pState->rotation );

		return true;
	}

	void PlayerControlComponent::internalDisposeState( PlayerControlComponentState* pState )
	{
		pState->pTransform			= nullptr;
		pState->pPhysicsController	= nullptr;
	}

	void PlayerControlComponent::getPlayerViewState( PlayerViewState& rTargetState, const PlayerControlComponentState* pState ) const
	{
		TIKI_ASSERT( pState != nullptr );

		quaternion::fromYawPitchRoll( rTargetState.eyeDirection, pState->rotation.x, pState->rotation.y, 0.0f );

		Vector3 basePosition = { 0.0f, 0.25f, -0.15f };		
		quaternion::transform( basePosition, pState->positionRotation );
		m_pTransformComponent->getPosition( rTargetState.eyePosition, pState->pTransform );
		vector::add( rTargetState.eyePosition, basePosition );
	}
}