#include "tiki/gameplay/game_client.hpp"

#include "tiki/base/base_types.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/renderer_2d/renderer_2d.hpp"
#include "tiki/resource/resource_request_pool.hpp"

#include "components_2d.hpp"
#include "mechanica_components.hpp"

namespace tiki
{
	GameClient::GameClient()
	{
	}

	GameClient::~GameClient()
	{
	}

	bool GameClient::create()
	{
		EntitySystemParameters entitySystemParams;
		entitySystemParams.typeRegisterMaxCount		= MaxTypeCount;
		entitySystemParams.storageChunkSize			= ChunkSize;
		entitySystemParams.storageChunkCount		= ChunkCount;

		static const EntityPool s_entityPools[] =
		{
			{    1u,   1u },	// player
			{ 1000u, 999u },	// enemies
			{ 2000u, 999u },	// level objects
			{ 3000u, 999u },	// static objects
			{ 4000u, 999u },	// projectiles
		};

		entitySystemParams.entityPools.create( s_entityPools, TIKI_COUNT( s_entityPools ) );

		if ( !m_entitySystem.create( entitySystemParams ) )
		{
			dispose();
			return false;
		}

		m_physicsWorld.create( vector::create( 0.0f, 9.81f ), 1.0f );

		if( !m_transformComponent.create() ||
			!m_spriteComponent.create( m_transformComponent )   ||
			!m_polygonComponent.create( m_transformComponent ) ||
			!m_physicsBodyComponent.create( m_physicsWorld, m_transformComponent ) ||
			!m_physicsColliderComponent.create( m_physicsWorld ) ||
			!m_physicsJointComponent.create( m_physicsWorld ) ||
			!m_playerComponent.create( m_physicsWorld, m_transformComponent ) ||
			!m_lifeTimeComponent.create() ||
			!m_wiggleComponent.create( m_physicsWorld, m_transformComponent, m_physicsBodyComponent ) ||
			!m_breakableComponent.create( m_entitySystem, m_physicsWorld, m_transformComponent, m_physicsBodyComponent, m_spriteComponent, m_polygonComponent ) )
		{
			dispose();
			return false;
		}

		m_entitySystem.registerComponentType( &m_transformComponent );
		m_entitySystem.registerComponentType( &m_spriteComponent );
		m_entitySystem.registerComponentType( &m_polygonComponent );

		m_entitySystem.registerComponentType( &m_physicsBodyComponent );
		m_entitySystem.registerComponentType( &m_physicsColliderComponent );
		m_entitySystem.registerComponentType( &m_physicsJointComponent );

		m_entitySystem.registerComponentType( &m_breakableComponent );
		m_entitySystem.registerComponentType( &m_playerComponent );
		m_entitySystem.registerComponentType( &m_wiggleComponent );

		m_entitySystem.registerComponentType( &m_lifeTimeComponent );

		return true;
	}

	void GameClient::dispose()
	{
		m_entitySystem.update(); // to dispose all entities

		m_entitySystem.unregisterComponentType( &m_wiggleComponent );
		m_entitySystem.unregisterComponentType( &m_playerComponent );
		m_entitySystem.unregisterComponentType( &m_breakableComponent );

		m_entitySystem.unregisterComponentType( &m_physicsJointComponent );
		m_entitySystem.unregisterComponentType( &m_physicsColliderComponent );
		m_entitySystem.unregisterComponentType( &m_physicsBodyComponent );

		m_entitySystem.unregisterComponentType( &m_polygonComponent );
		m_entitySystem.unregisterComponentType( &m_spriteComponent );
		m_entitySystem.unregisterComponentType( &m_transformComponent );

		m_entitySystem.unregisterComponentType( &m_lifeTimeComponent );

		m_wiggleComponent.dispose();
		m_playerComponent.dispose();
		m_breakableComponent.dispose();

		m_physicsJointComponent.dispose();
		m_physicsColliderComponent.dispose();
		m_physicsBodyComponent.dispose();

		m_polygonComponent.dispose();
		m_spriteComponent.dispose();
		m_transformComponent.dispose();

		m_lifeTimeComponent.dispose();

		m_physicsWorld.dispose();
		m_entitySystem.dispose();
	}

	void GameClient::startLoadLevel( ResourceRequestPool& requestPool, DynamicString levelName )
	{
		const DynamicString levelResourceName = levelName + ".level";
		requestPool.beginLoadResource( &m_pLevel, levelResourceName.cStr() );
	}

	GameClientLoadResult GameClient::finalizeLoadLevel( ResourceRequestPool& requestPool )
	{
		if( requestPool.isFinish() )
		{
			if( requestPool.hasError() )
			{
				return GameClientLoadResult_Error;
			}

			createLevelEntities();

			return GameClientLoadResult_Successful;
		}

		return GameClientLoadResult_InProcess;
	}

	void GameClient::unloadLevel( ResourceRequestPool& requestPool )
	{
		requestPool.unloadResource( m_pLevel );
	}

	EntityId GameClient::createPlayerEntity( const Vector2& position )
	{
		Transform2dComponentInitData transformInitData;
		createFloat2( transformInitData.position, position.x, position.y );
		transformInitData.rotation = 0.0f;
		createFloat2( transformInitData.scale, 1.0f, 1.0f );

		PlayerComponentInitData playerInitData;
		playerInitData.mass = 20.0f;
		playerInitData.maxSpeed = 10.0f;
		playerInitData.runSpeed = 15.555f;
		playerInitData.sideWalkSpeed = 4.5f;
		playerInitData.jumpImpulse = -18.0f;
		playerInitData.friction = 10.0f;
		playerInitData.maxMotorTorque = 2000.0f;

		EntityComponent entityComponents[] =
		{
			{ m_transformComponent.getTypeCrc(),	&transformInitData },
			{ m_playerComponent.getTypeCrc(),		&playerInitData }
		};

		EntityTemplateData entityTemplate;
		entityTemplate.components = ResArray< EntityComponent >( entityComponents, TIKI_COUNT( entityComponents ) );

		return m_entitySystem.createEntityFromTemplate( 0u, entityTemplate );
	}

	EntityId GameClient::createEntityFromTemplate( const EntityTemplateData& entityTemplate )
	{
		return m_entitySystem.createEntityFromTemplate( 1u, entityTemplate );
	}

	//EntityId GameClient::createModelEntity( const Model* pModel, const Vector3& position )
	//{
	//	Transform3dComponentInitData transformInitData;
	//	createFloat3( transformInitData.position, position.x, position.y, position.z );
	//	createFloat4( transformInitData.rotation, 0.0f, 0.0f, 0.0f, 1.0f );
	//	createFloat3( transformInitData.scale, 1.0f, 1.0f, 1.0f );

	//	StaticModelComponentInitData modelInitData;
	//	modelInitData.model = pModel;

	//	EntityTemplateComponent entityComponents[] =
	//	{
	//		{ m_transformComponent.getTypeCrc(), &transformInitData },
	//		{ m_staticModelComponent.getTypeCrc(), &modelInitData }
	//	};

	//	EntityTemplate entityTemplate;
	//	entityTemplate.components.create( entityComponents, TIKI_COUNT( entityComponents ) );

	//	const EntityId result = m_entitySystem.createEntityFromTemplate( 1u, entityTemplate );
	//	entityTemplate.components.dispose();
	//	return result;
	//}
	//
	//EntityId GameClient::createBoxEntity( const Model* pModel, const Vector3& position )
	//{
	//	Transform3dComponentInitData transformInitData;
	//	createFloat3( transformInitData.position, position.x, position.y, position.z );
	//	createFloat4( transformInitData.rotation, 0.0f, 0.0f, 0.0f, 1.0f );
	//	createFloat3( transformInitData.scale, 1.0f, 1.0f, 1.0f );

	//	StaticModelComponentInitData modelInitData;
	//	modelInitData.model = pModel;

	//	Physics3dBodyComponentInitData bodyInitData;
	//	createFloat3( bodyInitData.position, position.x, position.y, position.z );
	//	bodyInitData.mass			= 100.0f;
	//	bodyInitData.freeRotation	= true;
	//	bodyInitData.shape.type		= Physics3dShapeType_Box;
	//	createFloat3( bodyInitData.shape.boxSize, 1.0f, 1.0f, 1.0f );

	//	LifeTimeComponentInitData lifeTimeInitData;
	//	lifeTimeInitData.lifeTimeInMs = 60000;

	//	EntityTemplateComponent entityComponents[] =
	//	{
	//		{ m_transformComponent.getTypeCrc(),	&transformInitData },
	//		{ m_physicsBodyComponent.getTypeCrc(),	&bodyInitData },
	//		{ m_staticModelComponent.getTypeCrc(),	&modelInitData },
	//		{ m_lifeTimeComponent.getTypeCrc(),		&lifeTimeInitData }
	//	};

	//	EntityTemplate entityTemplate;
	//	entityTemplate.components.create( entityComponents, TIKI_COUNT( entityComponents ) );

	//	const EntityId result = m_entitySystem.createEntityFromTemplate( 1u, entityTemplate );
	//	entityTemplate.components.dispose();
	//	return result;
	//}
	//
	//EntityId GameClient::createCoinEntity( const Model* pModel, const Vector3& position )
	//{
	//	Transform3dComponentInitData transformInitData;
	//	createFloat3( transformInitData.position, position.x, position.y, position.z );
	//	createFloat4( transformInitData.rotation, 0.0f, 0.0f, 0.0f, 1.0f );
	//	createFloat3( transformInitData.scale, 0.5f, 0.5f, 0.5f );

	//	StaticModelComponentInitData modelInitData;
	//	modelInitData.model = pModel;

	//	Physics3dBodyComponentInitData bodyInitData;
	//	createFloat3( bodyInitData.position, position.x, position.y, position.z );
	//	bodyInitData.mass			= 100.0f;
	//	bodyInitData.freeRotation	= true;
	//	bodyInitData.shape.type		= Physics3dShapeType_Box;
	//	createFloat3( bodyInitData.shape.boxSize, 0.5f, 0.5f, 0.5f );

	//	LifeTimeComponentInitData lifeTimeInitData;
	//	lifeTimeInitData.lifeTimeInMs = 60000;

	//	CoinComponentInitData coinInitData;
	//	coinInitData.value = 1.0f;

	//	EntityTemplateComponent entityComponents[] =
	//	{
	//		{ m_transformComponent.getTypeCrc(),	&transformInitData },
	//		{ m_physicsBodyComponent.getTypeCrc(),	&bodyInitData },
	//		{ m_staticModelComponent.getTypeCrc(),	&modelInitData },
	//		{ m_lifeTimeComponent.getTypeCrc(),		&lifeTimeInitData },
	//		{ m_coinComponent.getTypeCrc(),			&coinInitData }
	//	};

	//	EntityTemplate entityTemplate;
	//	entityTemplate.components.create( entityComponents, TIKI_COUNT( entityComponents ) );

	//	const EntityId result = m_entitySystem.createEntityFromTemplate( 1u, entityTemplate );
	//	entityTemplate.components.dispose();
	//	return result;
	//}

	//EntityId GameClient::createTerrainEntity( const Model* pModel, const Vector3& position )
	//{
	//	Transform3dComponentInitData transformInitData;
	//	createFloat3( transformInitData.position, position.x, position.y, position.z );
	//	createFloat4( transformInitData.rotation, 0.0f, 0.0f, 0.0f, 1.0f );
	//	createFloat3( transformInitData.scale, 1.0f, 1.0f, 1.0f );

	//	TerrainComponentInitData terrainInitData;
	//	terrainInitData.model = pModel;

	//	Physics3dColliderComponentInitData colliderInitData;
	//	createFloat3( colliderInitData.position, position.x, position.y + 0.005f, position.z );
	//	colliderInitData.shape.type = Physics3dShapeType_Box;
	//	createFloat3( colliderInitData.shape.boxSize, 100.00f, 0.01f, 100.0f );

	//	EntityTemplateComponent entityComponents[] =
	//	{
	//		{ m_transformComponent.getTypeCrc(), &transformInitData },
	//		{ m_physicsColliderComponent.getTypeCrc(), &colliderInitData },
	//		{ m_terrainComponent.getTypeCrc(), &terrainInitData }
	//	};

	//	EntityTemplate entityTemplate;
	//	entityTemplate.components.create( entityComponents, TIKI_COUNT( entityComponents ) );

	//	const EntityId result = m_entitySystem.createEntityFromTemplate( 1u, entityTemplate );
	//	entityTemplate.components.dispose();
	//	return result;
	//}

	void GameClient::disposeEntity( EntityId entityId )
	{
		m_entitySystem.disposeEntity( entityId );
	}

	void GameClient::applyRenderParameters( Renderer2dRenderParameters& renderParameters )
	{
		const LevelThemeData& levelTheme = m_pLevel->getData().theme->getData();

		renderParameters.enableBloom			= true;
		renderParameters.bloomCutoffThreshold.r	= levelTheme.bloomCutOffThreshold.x;
		renderParameters.bloomCutoffThreshold.g	= levelTheme.bloomCutOffThreshold.y;
		renderParameters.bloomCutoffThreshold.b	= levelTheme.bloomCutOffThreshold.z;
	}

	void GameClient::update( const MechanicaUpdateContext& updateContext )
	{
		const timems timeMs = timems( updateContext.gameTime.elapsedTime * 1000.0f );

		m_entitySystem.update();
		m_physicsWorld.update( updateContext.gameTime.elapsedTime );

		m_physicsBodyComponent.update();
		m_breakableComponent.update( updateContext.gameTime.elapsedTime );
		m_playerComponent.update( updateContext.gameTime.elapsedTime );
		m_lifeTimeComponent.update( m_entitySystem, timeMs );

		m_transformComponent.update();
	}

	void GameClient::render( Renderer2d& renderer )
	{
		m_spriteComponent.render( renderer );
		m_polygonComponent.render( renderer );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_physicsWorld.renderDebug();
#endif
	}

	bool GameClient::processInputEvent( const InputEvent& inputEvent )
	{
		if ( m_playerComponent.processInputEvent( inputEvent ) )
		{
			return true;
		}

		return false;
	}

	void GameClient::createLevelEntities()
	{
		const LevelData& levelData = m_pLevel->getData();

		for( const EntityTemplateData& entityTemplate : levelData.entities )
		{
			createEntityFromTemplate(entityTemplate);
		}
	}
}