
#include "tiki/gameplay/gamesession.hpp"

#include "tiki/base/debugprop.hpp"
#include "tiki/gameplay/gameclient.hpp"
#include "tiki/graphics/model.hpp"
#include "tiki/physics/physicsboxshape.hpp"
#include "tiki/renderer/renderercontext.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_BOOL( s_useFreeCamera, "GameSession/UseFreeCamera", false );

	GameSession::GameSession()
	{
		m_pGameClient		= nullptr;
		m_playerEntityId	= InvalidEntityId;

		m_pModelPlayer		= nullptr;
		m_pModelTerrain		= nullptr;
	}

	GameSession::~GameSession()
	{
		TIKI_ASSERT( m_pGameClient == nullptr );
	}

	bool GameSession::create( GameClient& gameClient, ResourceManager& resourceManager )
	{
		TIKI_ASSERT( m_pGameClient == nullptr );

		m_pGameClient		= &gameClient;

		m_pModelPlayer		= resourceManager.loadResource< Model >( "spaceship.model" );
		m_pModelTerrain		= resourceManager.loadResource< Model >( "plane.model" );
		m_pModelCoin		= resourceManager.loadResource< Model >( "coin.model" );
		if ( m_pModelPlayer == nullptr || m_pModelTerrain == nullptr || m_pModelCoin == nullptr )
		{
			dispose( resourceManager );
			return false;
		}

		m_planeEntityId		= m_pGameClient->createPlaneEntity( m_pModelTerrain, vector::create( 0.0f, 0.0f, 0.0f ) );
		m_playerEntityId	= gameClient.createPlayerEntity( m_pModelPlayer, vector::create( 0.0f, 5.0f, 0.0f ) );
		if ( m_planeEntityId == InvalidEntityId || m_playerEntityId == InvalidEntityId )
		{
			dispose( resourceManager );
			return false;
		}

		m_gameCamera.create(
			m_playerEntityId,
			(TransformComponentState*)gameClient.getEntitySystem().getFirstComponentOfEntityAndType( m_playerEntityId, gameClient.getTransformComponent().getTypeId() ),
			gameClient.getTransformComponent()
		);

		m_freeCamera.create( Vector3::zero, Quaternion::identity );
		m_freeCamera.setMouseControl( true );

		return true;
	}

	void GameSession::dispose( ResourceManager& resourceManager )
	{
		TIKI_ASSERT( m_pGameClient != nullptr );

		m_freeCamera.dispose();
		m_gameCamera.dispose();
		
		m_pGameClient->disposeEntity( m_playerEntityId );
		m_playerEntityId = InvalidEntityId;

		m_pGameClient->disposeEntity( m_planeEntityId );
		m_planeEntityId = InvalidEntityId;

		resourceManager.unloadResource( m_pModelPlayer );
		resourceManager.unloadResource( m_pModelTerrain );
		resourceManager.unloadResource( m_pModelCoin );
		m_pModelPlayer		= nullptr;
		m_pModelTerrain		= nullptr;
		m_pModelCoin		= nullptr;

		m_pGameClient = nullptr;
	}

	void GameSession::update( FrameData& frameData, float timeDelta, float totalGameTime )
	{
		if ( s_useFreeCamera )
		{
			m_freeCamera.update( frameData.mainCamera, timeDelta );
		}
		else
		{
			m_gameCamera.update( frameData.mainCamera );
		}
	}

	void GameSession::render() const
	{
		// remove this?
	}

	bool GameSession::processInputEvent( const InputEvent& inputEvent )
	{
		if ( s_useFreeCamera )
		{
			return m_freeCamera.processInputEvent( inputEvent );
		}

		return false;
	}
}