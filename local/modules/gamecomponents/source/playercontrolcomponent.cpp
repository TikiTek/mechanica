#include "tiki/gamecomponents/playercontrolcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/debugprop.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/debugrenderer/debugrenderer.hpp"
#include "tiki/gameplay/gamecamera.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/intersection.hpp"
#include "tiki/math/matrix.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/ray.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics3dcomponents/physics3dbodycomponent.hpp"
#include "tiki/physics3dcomponents/physics3dcharactercontrollercomponent.hpp"

#include "gamecomponents.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_FLOAT( s_controlBorder, "GameComponents/PlayerControlBorder", 0.0f, 0.0f, 300.0f );

    struct Physics3dCharacterControllerComponentState;

	struct PlayerControlComponentState : public ComponentState
	{
		TransformComponentState*					pTransform;
		Physics3dCharacterControllerComponentState*	pPhysicsController;

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

	bool PlayerControlComponent::create( const TransformComponent& transformComponent, const Physics3dCharacterControllerComponent& physicsCharacterControllerComponent )
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

	void PlayerControlComponent::update( const GameCamera& gameCamera, const Camera& camera, float timeDelta )
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		const float nearPlane = camera.getProjection().getNearPlane();
		const float farPlane = camera.getProjection().getFarPlane();
		const float width = camera.getProjection().getWidth();
		const float height = camera.getProjection().getHeight();
		const Matrix44& viewProjection = camera.getViewProjectionMatrix();
		const Vector3& cameraPosition = gameCamera.getCameraPosition();

		const float left = s_controlBorder;
		const float right = width - s_controlBorder;
		const float top = s_controlBorder;
		const float bottom = height - s_controlBorder;

		Plane plane;
		plane.create( Vector3::zero, Vector3::unitY );

		//Vector3 topLeft = vector::create( left, top, 1.0f );
		//Vector3 topRight = vector::create( right, top, 1.0f );
		//Vector3 bottomLeft = vector::create( left, bottom, 1.0f );
		//Vector3 bottomRight = vector::create( right, bottom, 1.0f );
		//Ray rayTopLeft;
		//Ray rayTopRight;
		//Ray rayBottomLeft;
		//Ray rayBottomRight;
		//camera.getCameraRay( rayTopLeft, vector::create( left, top ) );
		//camera.getCameraRay( rayTopRight, vector::create( right, top ) );
		//camera.getCameraRay( rayBottomLeft, vector::create( left, bottom ) );
		//camera.getCameraRay( rayBottomRight, vector::create( right, bottom ) );
		//
		//intersection::intersectRayPlane( rayTopLeft, plane, topLeft );
		//intersection::intersectRayPlane( rayTopRight, plane, topRight );
		//intersection::intersectRayPlane( rayBottomLeft, plane, bottomLeft );
		//intersection::intersectRayPlane( rayBottomRight, plane, bottomRight );
		//vector::add( topLeft, cameraPosition );
		//vector::add( topRight, cameraPosition );
		//vector::add( bottomLeft, cameraPosition );
		//vector::add( bottomRight, cameraPosition );

		debugrenderer::drawLineFrustum( camera.getFrustum() );

		//debugrenderer::drawLine( topLeft, topRight, TIKI_COLOR_RED );
		//debugrenderer::drawLine( topLeft, bottomLeft, TIKI_COLOR_RED );
		//debugrenderer::drawLine( bottomLeft, bottomRight, TIKI_COLOR_RED );
		//debugrenderer::drawLine( topRight, bottomRight, TIKI_COLOR_RED );

		//const Vector3& camersPos = gameCamera.getCameraPosition();
		//
		//const float left = camersPos.x - 5.0f;
		//const float right = camersPos.x + 5.0f;
		//const float top = camersPos.z + 20.0f;
		//const float bottom = camersPos.z + 5.0f;
		//
		//debugrenderer::drawLine( vector::create( left, 5.0f, top ), vector::create( right, 5.0f, top ), TIKI_COLOR_RED );
		//debugrenderer::drawLine( vector::create( left, 5.0f, top ), vector::create( left, 5.0f, bottom ), TIKI_COLOR_RED );
		//debugrenderer::drawLine( vector::create( left, 5.0f, bottom ), vector::create( right, 5.0f, bottom ), TIKI_COLOR_RED );
		//debugrenderer::drawLine( vector::create( right, 5.0f, top ), vector::create( right, 5.0f, bottom ), TIKI_COLOR_RED );
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
		pState->pPhysicsController	= (Physics3dCharacterControllerComponentState*)componentIterator.getFirstOfType( m_pPhysicsCharacterControllerComponent->getTypeId() );

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
