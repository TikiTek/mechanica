#include "tiki/mechanica_components/playercontrolcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/debugprop.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components2d/transform2dcomponent.hpp"
#include "tiki/debugrenderer/debugrenderer.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/intersection.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/ray.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics2d/physics2dbody.hpp"

#include "mechanica_components.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_FLOAT( s_controlBorder, "GameComponents/PlayerControlBorder", 0.0f, 0.0f, 300.0f );

	struct PlayerInputState
	{
		Vector2		leftStick;
		Vector2		rightStick;

		bool		jump;
	};

	struct PlayerControlComponentState : public ComponentState
	{
		Transform2dComponentState*	pTransform;

		float						speed;
		Vector2						rotation;
		Quaternion					positionRotation;

		PlayerInputState			inputState;
	};

	PlayerControlComponent::PlayerControlComponent()
	{
		m_pTransformComponent	= nullptr;
		m_pPhysicsWorld			= nullptr;
	}

	PlayerControlComponent::~PlayerControlComponent()
	{
		TIKI_ASSERT( m_pTransformComponent	== nullptr );
		TIKI_ASSERT( m_pPhysicsWorld		== nullptr );
	}

	bool PlayerControlComponent::create( Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent )
	{
		TIKI_ASSERT( m_pTransformComponent	== nullptr );
		TIKI_ASSERT( m_pPhysicsWorld		== nullptr );

		m_pPhysicsWorld			= &physicsWorld;
		m_pTransformComponent	= &transformComponent;


		return true;
	}

	void PlayerControlComponent::dispose()
	{
		m_pTransformComponent	= nullptr;
		m_pPhysicsWorld			= nullptr;
	}

	void PlayerControlComponent::update( Camera& camera, float timeDelta )
	{
		Iterator componentStates = getIterator();
		State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			//Vector3 walkForce = { m_inputState.leftStick.x, 0.0f, m_inputState.leftStick.y };
			//vector::scale( walkForce, -pState->speed );
			//
			//const float rotationFactor = f32::piOver2 + (m_inputState.leftStick.x / 3.0f);
			//Quaternion rotation;
			//quaternion::fromYawPitchRoll( rotation, rotationFactor, 0.0f, 0.0f );
			//
			//m_pPhysicsCharacterControllerComponent->move( pState->pPhysicsController, walkForce );
			//m_pTransformComponent->setRotation( pState->pTransform, rotation );
		}
	}

	bool PlayerControlComponent::processInputEvent( const InputEvent& inputEvent )
	{
		//if ( inputEvent.eventType == InputEventType_Controller_StickChanged && inputEvent.deviceId == 0u )
		//{
		//	const InputEventControllerStickData& stickData = inputEvent.data.controllerStick;
		//	Vector2& targetVector = ( stickData.stickIndex == 0u ? m_inputState.leftStick : m_inputState.rightStick );

		//	targetVector.x = stickData.xState;
		//	targetVector.y = stickData.yState;

		//	return true;
		//}
		//else if ( inputEvent.eventType == InputEventType_Controller_ButtonDown && inputEvent.data.controllerButton.button == ControllerButton_A && inputEvent.deviceId == 0u )
		//{
		//	m_inputState.jump = true;
		//}

		return false;
	}

	crc32 PlayerControlComponent::getTypeCrc() const
	{
		return MechanicaComponentType_PlayerControl;
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
		pState->pTransform = (Transform2dComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );

		pState->speed = pInitData->speed;
		vector::clear( pState->rotation );

		vector::clear( pState->inputState.leftStick );
		vector::clear( pState->inputState.rightStick );

		return true;
	}

	void PlayerControlComponent::internalDisposeState( PlayerControlComponentState* pState )
	{
	}
}
