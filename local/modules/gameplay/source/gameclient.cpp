#include "tiki/gameplay/gameclient.hpp"

#include "tiki/base/debugprop.hpp"
#include "tiki/graphics/color_xkcd.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/math/basetypes.hpp"
#include "tiki/math/vector.hpp"

#include "components2d.hpp"
#include "mechanica_components.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_BOOL( s_drawPhysicsDebug, "GameClient/DrawPhysicsDebug", false );

	GameClient::GameClient()
	{
	}

	GameClient::~GameClient()
	{
	}

	bool GameClient::create( GraphicsSystem& graphicsSystem )
	{
		m_pGraphicsSystem = &graphicsSystem;

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

		TIKI_VERIFY( m_transformComponent.create() );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_transformComponent ) );

		TIKI_VERIFY( m_textureComponent.create( m_transformComponent ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_textureComponent ) );

		TIKI_VERIFY( m_physicsBodyComponent.create( m_physicsWorld, m_transformComponent ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_physicsBodyComponent ) );

		TIKI_VERIFY( m_physicsColliderComponent.create( m_physicsWorld ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_physicsColliderComponent ) );

		TIKI_VERIFY( m_physicsJointComponent.create( m_physicsWorld ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_physicsJointComponent ) );

		TIKI_VERIFY( m_playerComponent.create( m_physicsWorld, m_transformComponent ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_playerComponent ) );

		TIKI_VERIFY( m_lifeTimeComponent.create() );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_lifeTimeComponent ) );

		TIKI_VERIFY( m_wiggleComponent.create( m_physicsWorld, m_transformComponent, m_physicsBodyComponent ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_wiggleComponent ) );

		return true;
	}

	void GameClient::dispose()
	{
		m_entitySystem.update(); // to dispose all entities

		m_entitySystem.unregisterComponentType( &m_wiggleComponent );
		m_entitySystem.unregisterComponentType( &m_lifeTimeComponent );
		m_entitySystem.unregisterComponentType( &m_physicsBodyComponent );
		m_entitySystem.unregisterComponentType( &m_physicsJointComponent );
		m_entitySystem.unregisterComponentType( &m_physicsColliderComponent );
		m_entitySystem.unregisterComponentType( &m_playerComponent );
		m_entitySystem.unregisterComponentType( &m_textureComponent );
		m_entitySystem.unregisterComponentType( &m_transformComponent );

		m_wiggleComponent.dispose();
		m_lifeTimeComponent.dispose();
		m_playerComponent.dispose();
		m_physicsJointComponent.dispose();
		m_physicsColliderComponent.dispose();
		m_physicsBodyComponent.dispose();
		m_textureComponent.dispose();
		m_transformComponent.dispose();

		m_physicsWorld.dispose();

		m_entitySystem.dispose();

		m_pGraphicsSystem = nullptr;
	}
	
	EntityId GameClient::createPlayerEntity( const Vector2& position )
	{
		Transform2dComponentInitData transformInitData;
		createFloat2( transformInitData.position, position.x, position.y );
		transformInitData.rotation = 0.0f;
		createFloat2( transformInitData.scale, 1.0f, 1.0f );

		PlayerComponentInitData playerInitData;
		playerInitData.speed = 0.1f;

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

	void GameClient::update( GameClientUpdateContext& updateContext )
	{
		const timems timeMs = timems( updateContext.timeDelta * 1000.0f );

		m_entitySystem.update();
		m_physicsWorld.update( updateContext.timeDelta );

		m_physicsBodyComponent.update();
		m_playerComponent.update( updateContext.timeDelta );
		m_lifeTimeComponent.update( m_entitySystem, timeMs );

		m_transformComponent.update();
	}

	void GameClient::render( Renderer2d& renderer )
	{
		m_textureComponent.render( renderer );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		if( s_drawPhysicsDebug )
		{
			m_physicsWorld.renderDebug();
		}
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
}