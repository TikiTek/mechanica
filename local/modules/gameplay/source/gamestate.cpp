
#include "tiki/gameplay/gamestate.hpp"

#include "tiki/animation/animation.hpp"
#include "tiki/gameplay/gameclient.hpp"
#include "tiki/graphics/model.hpp"
#include "tiki/physics/physicsboxshape.hpp"
#include "tiki/renderer/renderercontext.hpp"
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

		m_pModelBox			= resourceManager.loadResource< Model >( "box.model" );
		m_pModelCoin		= resourceManager.loadResource< Model >( "coin.model" );
		m_pModelPlane		= resourceManager.loadResource< Model >( "plane.model" );
		m_pModelPlayer		= resourceManager.loadResource< Model >( "player.model" );
		m_pAnimationPlayer	= resourceManager.loadResource< Animation >( "player.run.animation" );		
		if ( m_pModelBox == nullptr || m_pModelPlane == nullptr || m_pModelPlayer == nullptr || m_pAnimationPlayer == nullptr )
		{
			dispose( resourceManager );
			return false;
		}

		m_planeEntityId = m_pGameClient->createPlaneEntity( m_pModelPlane, vector::create( 0.0f, -0.1f, 0.0f ) );
		m_playerEntityId = gameClient.createPlayerEntity( m_pModelBox, vector::create( 0.0f, 1.0f, 0.0f ) );
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
		
		m_pGameClient->disposeEntity( m_playerEntityId );
		m_playerEntityId = InvalidEntityId;

		m_pGameClient->disposeEntity( m_planeEntityId );
		m_planeEntityId = InvalidEntityId;

		resourceManager.unloadResource( m_pAnimationPlayer );
		resourceManager.unloadResource( m_pModelPlayer );
		resourceManager.unloadResource( m_pModelPlane );
		resourceManager.unloadResource( m_pModelCoin );
		resourceManager.unloadResource( m_pModelBox );
		m_pModelBox			= nullptr;
		m_pModelCoin		= nullptr;
		m_pModelPlane		= nullptr;
		m_pModelPlayer		= nullptr;
		m_pAnimationPlayer	= nullptr;

		m_pGameClient = nullptr;
	}

	void GameState::update( FrameData& frameData, float timeDelta, float totalGameTime )
	{
		if ( m_lastBoxSpawn + ( BoxSpawnIntervalMilliseconds / 1000.0f ) < totalGameTime )
		{
			spawnBox();			
			m_lastBoxSpawn = totalGameTime;
		}

		if ( m_lastCoinSpawn + ( CoinSpawnIntervalMilliseconds / 1000.0f ) < totalGameTime )
		{
			spawnCoin();			
			m_lastCoinSpawn = totalGameTime;
		}

		for (uint i = 0u; i < m_collectedCoins.getCount(); ++i)
		{
			if ( frameData.pointLights.isFull() )
			{
				break;
			}
			CollectedCoinState& coin = m_collectedCoins[ i ];

			PointLightData& light = frameData.pointLights.push();
			light.position	= coin.position;
			light.range		= sinf( coin.timeToLife ) * 10.0f;
			light.color		= TIKI_COLOR_YELLOW;

			coin.timeToLife -= timeDelta * 6.0f;
		} 

		for (uint i = 0u; i < m_collectedCoins.getCount(); ++i)
		{
			const CollectedCoinState& coin = m_collectedCoins[ i ];
			if ( coin.timeToLife < 0.0f )
			{
				m_collectedCoins.removeUnsortedByIndex( i );
			}
		} 
	}

	void GameState::render() const
	{
		// remove this?
	}

	void GameState::processCollectedCoins( const CollectedCoinIdArray& collectedCoins )
	{
		for (uint i = 0u; i < collectedCoins.getCount(); ++i)
		{
			const EntityId entityId = collectedCoins[ i ];

			if ( !m_collectedCoins.isFull() )
			{
				const TransformComponentState* pTransformState = (const TransformComponentState*)m_pGameClient->getEntitySystem().getFirstComponentOfEntityAndType( entityId, m_pGameClient->getTransformComponent().getTypeId() );

				CollectedCoinState& coin = m_collectedCoins.push();
				m_pGameClient->getTransformComponent().getPosition( coin.position, pTransformState );
				coin.timeToLife = f32::pi - 0.001f;
			}			

			m_pGameClient->getEntitySystem().disposeEntity( entityId );
		} 
	}

	bool GameState::findPositionForShape( Vector3& position, float y, const PhysicsShape& shape )
	{
		const PhysicsWorld& world = m_pGameClient->getPhysicsWorld();

		position = vector::create( 0.0f, y, 0.0f );

		uint tryCount = 0u;
		while (	world.checkIntersection( shape, position ) )
		{
			position = vector::create(
				f32::random( -10.0f, 10.0f ),
				y,
				f32::random( -10.0f, 10.0f )
			);

			tryCount++;
			if ( tryCount == 100u )
			{
				return false;
			}
		}

		return true;
	}

	void GameState::spawnBox()
	{
		PhysicsBoxShape shape;
		shape.create( vector::create( 1.0f, 10.0f, 1.0f ) );

		Vector3 position;
		if ( findPositionForShape( position, 5.5f, shape ) )
		{
			position.y = 10.0f;
			m_pGameClient->createBoxEntity( m_pModelBox, position );
		}

		shape.dispose();
	}

	void GameState::spawnCoin()
	{
		PhysicsBoxShape shape;
		shape.create( vector::create( 1.0f, 10.0f, 1.0f ) );

		Vector3 position;
		if ( findPositionForShape( position, 5.5f, shape ) )
		{
			position.y = 1.0f;
			m_pGameClient->createCoinEntity( m_pModelCoin, position );
		}

		shape.dispose();
	}
}