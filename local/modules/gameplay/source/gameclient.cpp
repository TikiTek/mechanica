
#include "tiki/gameplay/gameclient.hpp"

#include "tiki/components/entitytemplate.hpp"
#include "tiki/components/lifetimecomponent_initdata.hpp"
#include "tiki/components/physicscomponents_initdata.hpp"
#include "tiki/components/playercontrolcomponent_initdata.hpp"
#include "tiki/components/staticmodelcomponent_initdata.hpp"
#include "tiki/components/transformcomponent_initdata.hpp"
#include "tiki/gamecomponents/coincomponent_initdata.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	bool GameClient::create()
	{
		EntitySystemParameters entitySystemParams;
		entitySystemParams.typeRegisterMaxCount		= MaxTypeCount;
		entitySystemParams.storageChunkSize			= ChunkSize;
		entitySystemParams.storageChunkCount		= ChunkCount;

		EntityPool entityPools[] =
		{
			{ 1u,    1u },		// player
			{ 1000u, 999u },	// enemies
			{ 2000u, 999u },	// level objects
			{ 3000u, 999u },	// static objects
		};

		entitySystemParams.entityPools.create( entityPools, TIKI_COUNT( entityPools ) );

		if ( !m_entitySystem.create( entitySystemParams ) )
		{
			dispose();
			return false;
		}

		m_physicsWorld.create( vector::create( 0.0f, -9.81f, 0.0f ) );

		TIKI_VERIFY( m_transformComponent.create() );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_transformComponent ) );

		TIKI_VERIFY( m_staticModelComponent.create( m_transformComponent ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_staticModelComponent ) );

		TIKI_VERIFY( m_skinnedModelComponent.create( m_transformComponent ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_skinnedModelComponent ) );

		TIKI_VERIFY( m_physicsBodyComponent.create( m_physicsWorld, m_transformComponent ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_physicsBodyComponent ) );

		TIKI_VERIFY( m_physicsColliderComponent.create( m_physicsWorld ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_physicsColliderComponent ) );

		TIKI_VERIFY( m_physicsCharacterControllerComponent.create( m_physicsWorld, m_transformComponent ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_physicsCharacterControllerComponent ) );

		TIKI_VERIFY( m_playerControlComponent.create( m_transformComponent, m_physicsCharacterControllerComponent ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_playerControlComponent ) );

		TIKI_VERIFY( m_lifeTimeComponent.create() );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_lifeTimeComponent ) );

		TIKI_VERIFY( m_coinComponent.create( m_transformComponent, m_physicsBodyComponent, m_lifeTimeComponent, m_physicsWorld ) );
		TIKI_VERIFY( m_entitySystem.registerComponentType( &m_coinComponent ) );

		return true;
	}

	void GameClient::dispose()
	{
		m_entitySystem.update(); // to dispose all entities

		m_entitySystem.unregisterComponentType( &m_coinComponent );
		m_entitySystem.unregisterComponentType( &m_lifeTimeComponent );
		m_entitySystem.unregisterComponentType( &m_playerControlComponent );
		m_entitySystem.unregisterComponentType( &m_physicsCharacterControllerComponent );
		m_entitySystem.unregisterComponentType( &m_physicsColliderComponent );
		m_entitySystem.unregisterComponentType( &m_physicsBodyComponent );
		m_entitySystem.unregisterComponentType( &m_skinnedModelComponent );
		m_entitySystem.unregisterComponentType( &m_staticModelComponent );
		m_entitySystem.unregisterComponentType( &m_transformComponent );

		m_coinComponent.dispose();
		m_lifeTimeComponent.dispose();
		m_playerControlComponent.dispose();
		m_physicsCharacterControllerComponent.dispose();
		m_physicsColliderComponent.dispose();
		m_physicsBodyComponent.dispose();
		m_skinnedModelComponent.dispose();
		m_staticModelComponent.dispose();
		m_transformComponent.dispose();

		m_physicsWorld.dispose();

		m_entitySystem.dispose();
	}
	
	EntityId GameClient::createPlayerEntity( const Model* pModel, const Vector3& position )
	{
		TransformComponentInitData transformInitData;
		createFloat3( transformInitData.position, position.x, position.y, position.z );
		createFloat4( transformInitData.rotation, 0.0f, 0.0f, 0.0f, 1.0f );
		createFloat3( transformInitData.scale, 1.0f, 1.0f, 1.0f );

		StaticModelComponentInitData modelInitData;
		modelInitData.model = pModel;

		PhysicsCharacterControllerComponentInitData controllerInitData;
		createFloat3( controllerInitData.position, position.x, position.y, position.z );
		controllerInitData.shape.shapeType = ShapeType_Capsule;
		controllerInitData.shape.shapeCapsuleRadius = 0.5f;
		controllerInitData.shape.shapeCapsuleHeight = 1.0f;

		PlayerControlComponentInitData playerControlInitData;
		playerControlInitData.speed = 0.1f;

		EntityTemplateComponent entityComponents[] =
		{
			{ m_transformComponent.getTypeCrc(), &transformInitData },
			{ m_physicsCharacterControllerComponent.getTypeCrc(), &controllerInitData },
			{ m_staticModelComponent.getTypeCrc(), &modelInitData },
			{ m_playerControlComponent.getTypeCrc(), &playerControlInitData }
		};

		EntityTemplate entityTemplate;
		entityTemplate.componentCount	= TIKI_COUNT( entityComponents );
		entityTemplate.pComponents		= entityComponents;

		return m_entitySystem.createEntityFromTemplate( 0u, entityTemplate );
	}

	EntityId GameClient::createModelEntity( const Model* pModel, const Vector3& position )
	{
		TransformComponentInitData transformInitData;
		createFloat3( transformInitData.position, position.x, position.y, position.z );
		createFloat4( transformInitData.rotation, 0.0f, 0.0f, 0.0f, 1.0f );
		createFloat3( transformInitData.scale, 1.0f, 1.0f, 1.0f );

		StaticModelComponentInitData modelInitData;
		modelInitData.model = pModel;

		EntityTemplateComponent entityComponents[] =
		{
			{ m_transformComponent.getTypeCrc(), &transformInitData },
			{ m_staticModelComponent.getTypeCrc(), &modelInitData }
		};

		EntityTemplate entityTemplate;
		entityTemplate.componentCount	= TIKI_COUNT( entityComponents );
		entityTemplate.pComponents		= entityComponents;

		return m_entitySystem.createEntityFromTemplate( 1u, entityTemplate );
	}
	
	EntityId GameClient::createBoxEntity( const Model* pModel, const Vector3& position )
	{
		TransformComponentInitData transformInitData;
		createFloat3( transformInitData.position, position.x, position.y, position.z );
		createFloat4( transformInitData.rotation, 0.0f, 0.0f, 0.0f, 1.0f );
		createFloat3( transformInitData.scale, 1.0f, 1.0f, 1.0f );

		StaticModelComponentInitData modelInitData;
		modelInitData.model = pModel;

		PhysicsBodyComponentInitData bodyInitData;
		createFloat3( bodyInitData.position, position.x, position.y, position.z );
		bodyInitData.mass			= 100.0f;
		bodyInitData.freeRotation	= true;
		bodyInitData.shape.shapeType = ShapeType_Box;
		createFloat3( bodyInitData.shape.shapeBoxSize, 1.0f, 1.0f, 1.0f );

		LifeTimeComponentInitData lifeTimeInitData;
		lifeTimeInitData.lifeTimeInSeconds = 60.0f;

		EntityTemplateComponent entityComponents[] =
		{
			{ m_transformComponent.getTypeCrc(),	&transformInitData },
			{ m_physicsBodyComponent.getTypeCrc(),	&bodyInitData },
			{ m_staticModelComponent.getTypeCrc(),	&modelInitData },
			{ m_lifeTimeComponent.getTypeCrc(),		&lifeTimeInitData }
		};

		EntityTemplate entityTemplate;
		entityTemplate.componentCount	= TIKI_COUNT( entityComponents );
		entityTemplate.pComponents		= entityComponents;

		return m_entitySystem.createEntityFromTemplate( 1u, entityTemplate );
	}
	
	EntityId GameClient::createCoinEntity( const Model* pModel, const Vector3& position )
	{
		TransformComponentInitData transformInitData;
		createFloat3( transformInitData.position, position.x, position.y, position.z );
		createFloat4( transformInitData.rotation, 0.0f, 0.0f, 0.0f, 1.0f );
		createFloat3( transformInitData.scale, 0.5f, 0.5f, 0.5f );

		StaticModelComponentInitData modelInitData;
		modelInitData.model = pModel;

		PhysicsBodyComponentInitData bodyInitData;
		createFloat3( bodyInitData.position, position.x, position.y, position.z );
		bodyInitData.mass			= 100.0f;
		bodyInitData.freeRotation	= true;
		bodyInitData.shape.shapeType = ShapeType_Box;
		createFloat3( bodyInitData.shape.shapeBoxSize, 0.5f, 0.5f, 0.5f );

		LifeTimeComponentInitData lifeTimeInitData;
		lifeTimeInitData.lifeTimeInSeconds = 60.0f;

		CoinComponentInitData coinInitData;
		coinInitData.value = 1.0f;

		EntityTemplateComponent entityComponents[] =
		{
			{ m_transformComponent.getTypeCrc(),	&transformInitData },
			{ m_physicsBodyComponent.getTypeCrc(),	&bodyInitData },
			{ m_staticModelComponent.getTypeCrc(),	&modelInitData },
			{ m_lifeTimeComponent.getTypeCrc(),		&lifeTimeInitData },
			{ m_coinComponent.getTypeCrc(),			&coinInitData }
		};

		EntityTemplate entityTemplate;
		entityTemplate.componentCount	= TIKI_COUNT( entityComponents );
		entityTemplate.pComponents		= entityComponents;

		return m_entitySystem.createEntityFromTemplate( 1u, entityTemplate );
	}

	EntityId GameClient::createPlaneEntity( const Model* pModel, const Vector3& position )
	{
		TransformComponentInitData transformInitData;
		createFloat3( transformInitData.position, position.x, position.y, position.z );
		createFloat4( transformInitData.rotation, 0.0f, 0.0f, 0.0f, 1.0f );
		createFloat3( transformInitData.scale, 1.0f, 1.0f, 1.0f );

		StaticModelComponentInitData modelInitData;
		modelInitData.model = pModel;

		PhysicsColliderComponentInitData colliderInitData;
		createFloat3( colliderInitData.position, position.x, position.y + 0.005f, position.z );
		colliderInitData.shape.shapeType = ShapeType_Box;
		createFloat3( colliderInitData.shape.shapeBoxSize, 100.00f, 0.01f, 100.0f );

		EntityTemplateComponent entityComponents[] =
		{
			{ m_transformComponent.getTypeCrc(), &transformInitData },
			{ m_physicsColliderComponent.getTypeCrc(), &colliderInitData },
			{ m_staticModelComponent.getTypeCrc(), &modelInitData }
		};

		EntityTemplate entityTemplate;
		entityTemplate.componentCount	= TIKI_COUNT( entityComponents );
		entityTemplate.pComponents		= entityComponents;

		return m_entitySystem.createEntityFromTemplate( 1u, entityTemplate );
	}

	void GameClient::disposeEntity( EntityId entityId )
	{
		m_entitySystem.disposeEntity( entityId );
	}

	void GameClient::update( GameClientUpdateContext& updateContext )
	{
		m_entitySystem.update();

		m_physicsWorld.update( updateContext.timeDelta );

		m_physicsCharacterControllerComponent.update();
		m_physicsBodyComponent.update();
		m_playerControlComponent.update( updateContext.timeDelta );
		m_lifeTimeComponent.update( m_entitySystem, updateContext.timeDelta );
		m_coinComponent.update( updateContext.playerCollider, updateContext.collectedCoins, updateContext.totalGameTime );

		m_transformComponent.update();
	}

	void GameClient::render( GameRenderer& gameRenderer )
	{
		m_staticModelComponent.render( gameRenderer );
		m_skinnedModelComponent.render( gameRenderer );
	}
	
	bool GameClient::processInputEvent( const InputEvent& inputEvent )
	{
		if ( m_playerControlComponent.processInputEvent( inputEvent ) )
		{
			return true;
		}

		return false;
	}
}