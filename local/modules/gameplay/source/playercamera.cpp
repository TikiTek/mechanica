
#include "tiki/gameplay/playercamera.hpp"

#include "tiki/gamecomponents/playercontrolcomponent.hpp"
#include "tiki/math/camera.hpp"

namespace tiki
{
	PlayerCamera::PlayerCamera()
	{		
		m_pControlComponent	= nullptr;
		
		m_playerEntityId	= InvalidEntityId;
		m_pControlState		= nullptr;
	}

	PlayerCamera::~PlayerCamera()
	{
		TIKI_ASSERT( m_pControlComponent	== nullptr );

		TIKI_ASSERT( m_playerEntityId		== InvalidEntityId );
		TIKI_ASSERT( m_pControlState		== nullptr );
	}

	void PlayerCamera::create( EntityId playerEntityId, const PlayerControlComponentState* pControlState, const PlayerControlComponent& playerControlComponent )
	{
		TIKI_ASSERT( playerEntityId != InvalidEntityId );
		TIKI_ASSERT( pControlState != nullptr );
		
		m_pControlComponent	= &playerControlComponent;

		m_playerEntityId	= playerEntityId;
		m_pControlState		= pControlState;
	}

	void PlayerCamera::dispose()
	{
		m_pControlComponent	= nullptr;

		m_playerEntityId	= InvalidEntityId;
		m_pControlState		= nullptr;
	}

	void PlayerCamera::update( Camera& targetCamera )
	{
		TIKI_ASSERT( m_pControlComponent != nullptr );

		PlayerViewState viewState;
		m_pControlComponent->getPlayerViewState( viewState, m_pControlState );

		targetCamera.setTransform( viewState.eyePosition, viewState.eyeDirection );
	}
}