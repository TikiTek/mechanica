
#include "tiki/gameplay/gamecamera.hpp"

#include "tiki/base/debugprop.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/math/camera.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_FLOAT( s_cameraAltitude, "GameCamera/CameraAltitude", 15.0f, 1.0f, 100.0f );
	TIKI_DEBUGPROP_FLOAT( s_cameraDistanceZ, "GameCamera/CameraDistanceZ", 8.0f, 0.0f, 100.0f );
	TIKI_DEBUGPROP_FLOAT( s_cameraXAngle, "GameCamera/CameraXAngle", f32::piOver4, 0.0f, f32::twoPi );

	GameCamera::GameCamera()
	{		
		m_pTransformComponent	= nullptr;
		
		m_playerEntityId		= InvalidEntityId;
		m_pPlayerState			= nullptr;
	}

	GameCamera::~GameCamera()
	{
		TIKI_ASSERT( m_pTransformComponent	== nullptr );

		TIKI_ASSERT( m_playerEntityId		== InvalidEntityId );
		TIKI_ASSERT( m_pPlayerState			== nullptr );
	}

	void GameCamera::create( EntityId playerEntityId, const TransformComponentState* pControlState, const TransformComponent& playerControlComponent )
	{
		TIKI_ASSERT( playerEntityId	!= InvalidEntityId );
		TIKI_ASSERT( pControlState	!= nullptr );
		
		m_pTransformComponent	= &playerControlComponent;

		m_playerEntityId	= playerEntityId;
		m_pPlayerState		= pControlState;
	}

	void GameCamera::dispose()
	{
		m_pTransformComponent	= nullptr;

		m_playerEntityId	= InvalidEntityId;
		m_pPlayerState		= nullptr;
	}

	void GameCamera::update( Camera& targetCamera )
	{
		TIKI_ASSERT( m_pTransformComponent != nullptr );

		Vector3 position;
		m_pTransformComponent->getPosition( position, m_pPlayerState );
		position.y += s_cameraAltitude;
		position.z += s_cameraDistanceZ;

		Vector3 lookAt = position;
		lookAt.y += 1.0f;

		Quaternion rotation;
		quaternion::createLookAt( rotation, position, lookAt );

		Quaternion angleRotation;
		quaternion::fromYawPitchRoll( angleRotation, 0.0f, s_cameraXAngle, 0.0f );

		quaternion::mul( rotation, angleRotation );

		targetCamera.setTransform( position, rotation );
	}
}