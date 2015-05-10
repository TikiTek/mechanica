
#include "tiki/gameplay/gamecamera.hpp"

#include "tiki/base/debugprop.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/math/camera.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_FLOAT( s_cameraAltitude, "GameCamera/CameraAltitude", 15.0f, 1.0f, 100.0f );

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

		Vector3 lookAt = position;
		lookAt.y += 1.0f;

		Quaternion rotation;
		quaternion::createLookAt( rotation, position, lookAt );

		targetCamera.setTransform( position, rotation );
	}
}