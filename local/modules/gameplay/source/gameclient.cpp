
#include "tiki/gameplay/gameclient.hpp"

#include "tiki/components/entitytemplate.hpp"
#include "tiki/components/physicscomponents_initdata.hpp"
#include "tiki/components/staticmodelcomponent_initdata.hpp"
#include "tiki/components/transformcomponent_initdata.hpp"

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

		m_physicsWorld.create( vector::create( 0.0f, -9.81f, 0.0f ) );

		if ( !m_entitySystem.create( entitySystemParams ) )
		{
			dispose();
			return false;
		}

		TIKI_VERIFY( m_transformComponent.create() );
		m_entitySystem.registerComponentType( &m_transformComponent );

		TIKI_VERIFY( m_staticModelComponent.create( m_transformComponent ) );
		m_entitySystem.registerComponentType( &m_staticModelComponent );

		TIKI_VERIFY( m_skinnedModelComponent.create( m_transformComponent ) );
		m_entitySystem.registerComponentType( &m_skinnedModelComponent );

		TIKI_VERIFY( m_physicsBodyComponent.create( m_physicsWorld, m_transformComponent ) );
		m_entitySystem.registerComponentType( &m_physicsBodyComponent );

		TIKI_VERIFY( m_physicsColliderComponent.create( m_physicsWorld ) );
		m_entitySystem.registerComponentType( &m_physicsColliderComponent );

		return true;
	}

	void GameClient::dispose()
	{
		m_entitySystem.unregisterComponentType( &m_physicsColliderComponent );
		m_entitySystem.unregisterComponentType( &m_physicsBodyComponent );
		m_entitySystem.unregisterComponentType( &m_skinnedModelComponent );
		m_entitySystem.unregisterComponentType( &m_staticModelComponent );
		m_entitySystem.unregisterComponentType( &m_transformComponent );

		m_physicsColliderComponent.dispose();
		m_physicsBodyComponent.dispose();
		m_skinnedModelComponent.dispose();
		m_staticModelComponent.dispose();
		m_transformComponent.dispose();

		m_entitySystem.dispose();

		m_physicsWorld.dispose();
	}

	EntityId GameClient::createModelEntity( const Model* pModel, const Vector3& position )
	{
		TransformComponentInitData transformInitData;
		createFloat3( transformInitData.position, position.x, position.y, position.z );
		createFloat4( transformInitData.rotation, 0.0f, 0.0f, 0.0f, 1.0f );

		StaticModelComponentInitData modelInitData;
		modelInitData.model = pModel;

		EntityTemplateComponent entityComponents[] =
		{
			{ m_transformComponent.getTypeCrc(), &transformInitData },
			{ m_staticModelComponent.getTypeCrc(), &modelInitData }
		};

		EntityTemplate entityTemplate;
		entityTemplate.componentCount	= 2u;
		entityTemplate.pComponents		= entityComponents;

		return m_entitySystem.createEntityFromTemplate( 1u, entityTemplate );
	}
	
	EntityId GameClient::createPhysicsBoxEntity( const Model* pModel, const Vector3& position )
	{
		TransformComponentInitData transformInitData;
		createFloat3( transformInitData.position, position.x, position.y, position.z );
		createFloat4( transformInitData.rotation, 0.0f, 0.0f, 0.0f, 1.0f );

		StaticModelComponentInitData modelInitData;
		modelInitData.model = pModel;

		PhysicsBodyComponentInitData bodyInitData;
		createFloat3( bodyInitData.position, position.x, position.y, position.z );
		bodyInitData.mass = 100.0f;
		bodyInitData.shape.shapeType = ShapeType_Box;
		createFloat3( bodyInitData.shape.shapeBoxSize, 1.0f, 1.0f, 1.0f );

		EntityTemplateComponent entityComponents[] =
		{
			{ m_transformComponent.getTypeCrc(), &transformInitData },
			{ m_physicsBodyComponent.getTypeCrc(), &bodyInitData },
			{ m_staticModelComponent.getTypeCrc(), &modelInitData }
		};

		EntityTemplate entityTemplate;
		entityTemplate.componentCount	= 3u;
		entityTemplate.pComponents		= entityComponents;

		return m_entitySystem.createEntityFromTemplate( 1u, entityTemplate );
	}

	EntityId GameClient::createPlaneEntity( const Model* pModel, const Vector3& position )
	{
		TransformComponentInitData transformInitData;
		createFloat3( transformInitData.position, position.x, position.y, position.z );
		createFloat4( transformInitData.rotation, 0.0f, 0.0f, 0.0f, 1.0f );

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
		entityTemplate.componentCount	= 3u;
		entityTemplate.pComponents		= entityComponents;

		return m_entitySystem.createEntityFromTemplate( 1u, entityTemplate );
	}

	void GameClient::disposeEntity( EntityId entityId )
	{
		m_entitySystem.disposeEntity( entityId );
	}

	void GameClient::update( float timeStep )
	{
		m_physicsWorld.update( timeStep );

		m_physicsBodyComponent.update();
		m_transformComponent.update();
	}

	void GameClient::render( GameRenderer& gameRenderer )
	{
		m_staticModelComponent.render( gameRenderer );
		m_skinnedModelComponent.render( gameRenderer );
	}
}