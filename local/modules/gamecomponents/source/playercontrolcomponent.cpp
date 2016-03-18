
#include "tiki/gamecomponents/playercontrolcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/physicsbodycomponent.hpp"
#include "tiki/components/physicscharactercontrollercomponent.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/debugrenderer/debugrenderer.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/vector.hpp"

#include "gamecomponents.hpp"

namespace tiki
{
    struct PhysicsCharacterControllerComponentState;

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
	}

	PlayerControlComponent::~PlayerControlComponent()
	{
		TIKI_ASSERT( m_pTransformComponent							== nullptr );
		TIKI_ASSERT( m_pPhysicsCharacterControllerComponent			== nullptr );
	}

	bool PlayerControlComponent::create( const TransformComponent& transformComponent, const PhysicsCharacterControllerComponent& physicsCharacterControllerComponent )
	{
		TIKI_ASSERT( m_pTransformComponent							== nullptr );
		TIKI_ASSERT( m_pPhysicsCharacterControllerComponent			== nullptr );

		m_pTransformComponent					= &transformComponent;
		m_pPhysicsCharacterControllerComponent	= &physicsCharacterControllerComponent;

		vector::clear( m_inputState.leftStick );
		vector::clear( m_inputState.rightStick );

		return true;
	}

	void PlayerControlComponent::dispose()
	{
		m_pTransformComponent						= nullptr;
		m_pPhysicsCharacterControllerComponent		= nullptr;
	}

	void PlayerControlComponent::update( const GameCamera& gameCamera, float timeDelta )
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		debugrenderer::drawLine( vector::create( 0.0f, 5.0f, -100.0f ), vector::create( 0.0f, 5.0f, 100.0f ), TIKI_COLOR_RED );
#endif

		Iterator componentStates = getIterator();
		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			Vector3 walkForce = { m_inputState.leftStick.x, 0.0f, m_inputState.leftStick.y };
			vector::scale( walkForce, -pState->speed );
			
			const float rotationFactor = f32::piOver2 + (m_inputState.leftStick.x / 3.0f);
			Quaternion rotation;
			quaternion::fromYawPitchRoll( rotation, rotationFactor, 0.0f, 0.0f );
			
			m_pPhysicsCharacterControllerComponent->move( pState->pPhysicsController, walkForce );
			m_pTransformComponent->setRotation( pState->pTransform, rotation );
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
		pState->pTransform			= (TransformComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );
		pState->pPhysicsController	= (PhysicsCharacterControllerComponentState*)componentIterator.getFirstOfType( m_pPhysicsCharacterControllerComponent->getTypeId() );

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
