
#include "tiki/gameplay/gamestate.hpp"

#include "tiki/animation/animation.hpp"
#include "tiki/graphics/model.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	GameState::GameState()
	{
		m_pGameClient		= nullptr;
		m_playerEntityId	= InvalidEntityId;

		m_pModelBox			= nullptr;
		m_pModelPlane		= nullptr;
		m_pModelPlayer		= nullptr;
		m_pAnimationPlayer	= nullptr;
	}

	GameState::~GameState()
	{
		TIKI_ASSERT( m_pGameClient == nullptr );
	}

	bool GameState::create( GameClient& gameClient, ResourceManager& resourceManager )
	{
		TIKI_ASSERT( m_pGameClient == nullptr );

		m_pGameClient = &gameClient;

		if ( !m_boxEntityIds.create( MaxBoxCount ) || !m_coinEntityIds.create( MaxCoinCount ) )
		{
			dispose( resourceManager );
			return false;
		}

		m_pModelBox			= resourceManager.loadResource< Model >( "box.model" );
		m_pModelPlane		= resourceManager.loadResource< Model >( "plane.model" );
		m_pModelPlayer		= resourceManager.loadResource< Model >( "player.model" );
		m_pAnimationPlayer	= resourceManager.loadResource< Animation >( "player.run.animation" );		
		if ( m_pModelBox == nullptr || m_pModelPlane == nullptr || m_pModelPlayer == nullptr || m_pAnimationPlayer == nullptr )
		{
			dispose( resourceManager );
			return false;
		}

		m_planeEntityId = m_pGameClient->createPlaneEntity( m_pModelPlane, vector::create( 0.0f, -0.1f, 0.0f ) );
		m_playerEntityId = gameClient.createPlayerEntity( m_pModelPlayer, vector::create( 0.0f, 1.0f, 0.0f ) );
		if ( m_planeEntityId == InvalidEntityId || m_playerEntityId == InvalidEntityId )
		{
			dispose( resourceManager );
			return false;
		}

		return true;
	}

	void GameState::dispose( ResourceManager& resourceManager )
	{
		TIKI_ASSERT( m_pGameClient != nullptr );
		
		for (uint i = 0u; i < m_coinEntityIds.getCount(); ++i)
		{
			m_pGameClient->disposeEntity( m_coinEntityIds[ i ] );
		} 
		m_coinEntityIds.dispose();

		for (uint i = 0u; i < m_boxEntityIds.getCount(); ++i)
		{
			m_pGameClient->disposeEntity( m_boxEntityIds[ i ] );
		}
		m_boxEntityIds.dispose();
		
		m_pGameClient->disposeEntity( m_playerEntityId );
		m_playerEntityId = InvalidEntityId;

		m_pGameClient->disposeEntity( m_planeEntityId );
		m_planeEntityId = InvalidEntityId;

		resourceManager.unloadResource( m_pAnimationPlayer );
		resourceManager.unloadResource( m_pModelPlayer );
		resourceManager.unloadResource( m_pModelPlane );
		resourceManager.unloadResource( m_pModelBox );
		m_pModelBox			= nullptr;
		m_pModelPlane		= nullptr;
		m_pModelPlayer		= nullptr;
		m_pAnimationPlayer	= nullptr;

		m_pGameClient = nullptr;
	}

	void GameState::update( float totalGameTime )
	{
		if ( m_lastBoxSpawn + BoxSpawnIntervalSeconds < totalGameTime )
		{
			const Vector3 position = vector::create( f32::random( -1.0f, 1.0f ), 10.0f, f32::random( -1.0f, 1.0f ) );
			const EntityId entityId = m_pGameClient->createPhysicsBoxEntity( m_pModelBox, position );
			if ( entityId != InvalidEntityId )
			{
				m_boxEntityIds.push( entityId );
			}
		}
	}

	void GameState::render() const
	{
		// remove this?
	}
}