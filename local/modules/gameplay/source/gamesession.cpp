
#include "tiki/gameplay/gamesession.hpp"

#include "tiki/graphics/model.hpp"
#include "tiki/physics3d/physics3dboxshape.hpp"
#include "tiki/renderer/renderercontext.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	GameSession::GameSession()
	{
		m_playerEntityId	= InvalidEntityId;

		m_pModelPlayer		= nullptr;
		m_pModelTerrain		= nullptr;
		m_pModelCoin		= nullptr;
	}

	GameSession::~GameSession()
	{
	}

	bool GameSession::create( ResourceManager& resourceManager )
	{
		if( !m_gameClient.create() )
		{
			return false;
		}		

		m_pModelPlayer		= resourceManager.loadResource< Model >( "spaceship.model" );
		m_pModelTerrain		= resourceManager.loadResource< Model >( "plane.model" );
		m_pModelCoin		= resourceManager.loadResource< Model >( "coin.model" );
		if ( m_pModelPlayer == nullptr || m_pModelTerrain == nullptr || m_pModelCoin == nullptr )
		{
			dispose( resourceManager );
			return false;
		}

		m_planeEntityId		= m_gameClient.createTerrainEntity( m_pModelTerrain, vector::create( 0.0f, 0.0f, 0.0f ) );
		m_playerEntityId	= m_gameClient.createPlayerEntity( m_pModelPlayer, vector::create( 0.0f, 5.0f, 0.0f ) );
		if ( m_planeEntityId == InvalidEntityId || m_playerEntityId == InvalidEntityId )
		{
			dispose( resourceManager );
			return false;
		}

		return true;
	}

	void GameSession::dispose( ResourceManager& resourceManager )
	{
		m_gameClient.disposeEntity( m_playerEntityId );
		m_playerEntityId = InvalidEntityId;

		m_gameClient.disposeEntity( m_planeEntityId );
		m_planeEntityId = InvalidEntityId;

		resourceManager.unloadResource( m_pModelPlayer );
		resourceManager.unloadResource( m_pModelTerrain );
		resourceManager.unloadResource( m_pModelCoin );
		m_pModelPlayer		= nullptr;
		m_pModelTerrain		= nullptr;
		m_pModelCoin		= nullptr;

		m_gameClient.dispose();
	}

	void GameSession::update( float timeDelta, float totalGameTime )
	{
		const Physics3dCharacterControllerComponentState* pPlayerControllerState = (const Physics3dCharacterControllerComponentState*)m_gameClient.getEntitySystem().getFirstComponentOfEntityAndType( m_playerEntityId, m_gameClient.getPhysicsCharacterControllerComponent().getTypeId() );

		GameClientUpdateContext gameClientUpdateContext;
		gameClientUpdateContext.timeDelta		= timeDelta;
		gameClientUpdateContext.totalGameTime	= totalGameTime;
		gameClientUpdateContext.pPlayerCollider	= &m_gameClient.getPhysicsCharacterControllerComponent().getPhysicsObject( pPlayerControllerState );
		gameClientUpdateContext.pTerrainState	= (const TerrainComponentState*)m_gameClient.getEntitySystem().getFirstComponentOfEntityAndType( m_planeEntityId, m_gameClient.getTerrainComponent().getTypeId() );

		m_gameClient.update( gameClientUpdateContext );
	}

	void GameSession::render( GameRenderer& gameRenderer, GraphicsContext& graphicsContext )
	{
		m_gameClient.render( gameRenderer, graphicsContext );
	}

	bool GameSession::processInputEvent( const InputEvent& inputEvent )
	{
		return m_gameClient.processInputEvent( inputEvent );
	}
}