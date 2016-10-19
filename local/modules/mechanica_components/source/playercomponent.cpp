#include "tiki/mechanica_components/playercomponent.hpp"

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
	TIKI_DEBUGPROP_FLOAT( s_playerBorder, "GameComponents/PlayerBorder", 0.0f, 0.0f, 300.0f );

	struct PlayerInputState
	{
		Vector2		leftStick;
		Vector2		rightStick;

		bool		jump;
	};

	struct PlayerComponentState : public ComponentState
	{
		Transform2dComponentState*	pTransform;

		float						speed;
		Vector2						rotation;
		Quaternion					positionRotation;

		PlayerInputState			inputState;
	};

	PlayerComponent::PlayerComponent()
	{
		m_pTransformComponent	= nullptr;
		m_pPhysicsWorld			= nullptr;
	}

	PlayerComponent::~PlayerComponent()
	{
		TIKI_ASSERT( m_pTransformComponent	== nullptr );
		TIKI_ASSERT( m_pPhysicsWorld		== nullptr );
	}

	bool PlayerComponent::create( Physics2dWorld& physicsWorld, const Transform2dComponent& transformComponent )
	{
		TIKI_ASSERT( m_pTransformComponent	== nullptr );
		TIKI_ASSERT( m_pPhysicsWorld		== nullptr );

		m_pPhysicsWorld			= &physicsWorld;
		m_pTransformComponent	= &transformComponent;


		return true;
	}

	void PlayerComponent::dispose()
	{
		m_pTransformComponent	= nullptr;
		m_pPhysicsWorld			= nullptr;
	}

	void PlayerComponent::update( Camera& camera, float timeDelta )
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
			//m_pPhysicsCharacterlerComponent->move( pState->pPhysicsler, walkForce );
			//m_pTransformComponent->setRotation( pState->pTransform, rotation );
		}
	}

	bool PlayerComponent::processInputEvent( const InputEvent& inputEvent )
	{
		//if ( inputEvent.eventType == InputEventType_ler_StickChanged && inputEvent.deviceId == 0u )
		//{
		//	const InputEventlerStickData& stickData = inputEvent.data.lerStick;
		//	Vector2& targetVector = ( stickData.stickIndex == 0u ? m_inputState.leftStick : m_inputState.rightStick );

		//	targetVector.x = stickData.xState;
		//	targetVector.y = stickData.yState;

		//	return true;
		//}
		//else if ( inputEvent.eventType == InputEventType_ler_ButtonDown && inputEvent.data.lerButton.button == lerButton_A && inputEvent.deviceId == 0u )
		//{
		//	m_inputState.jump = true;
		//}

		return false;
	}

	crc32 PlayerComponent::getTypeCrc() const
	{
		return MechanicaComponentType_Player;
	}

	uint32 PlayerComponent::getStateSize() const
	{
		return sizeof( PlayerComponentState );
	}

	const char* PlayerComponent::getTypeName() const
	{
		return "PlayerComponent";
	}

	bool PlayerComponent::internalInitializeState( ComponentEntityIterator& componentIterator, PlayerComponentState* pState, const PlayerComponentInitData* pInitData )
	{
		pState->pTransform = (Transform2dComponentState*)componentIterator.getFirstOfType( m_pTransformComponent->getTypeId() );

		pState->speed = pInitData->speed;
		vector::clear( pState->rotation );

		vector::clear( pState->inputState.leftStick );
		vector::clear( pState->inputState.rightStick );

		return true;
	}

	void PlayerComponent::internalDisposeState( PlayerComponentState* pState )
	{
	}
}
