
#include "tiki/gameplay/gameclient.hpp"

#include "tiki/components/entitytemplate.hpp"
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

		if ( !m_entitySystem.create( entitySystemParams ) )
		{
			dispose();
			return false;
		}

		TIKI_VERIFY( m_transformComponent.create() );
		m_entitySystem.registerComponentType( &m_transformComponent );

		TIKI_VERIFY( m_staticModelComponent.create( m_entitySystem, m_transformComponent ) );
		m_entitySystem.registerComponentType( &m_staticModelComponent );

		TIKI_VERIFY( m_skinnedModelComponent.create( m_entitySystem, m_transformComponent ) );
		m_entitySystem.registerComponentType( &m_skinnedModelComponent );

		return true;
	}

	void GameClient::dispose()
	{
		m_entitySystem.unregisterComponentType( &m_skinnedModelComponent );
		m_entitySystem.unregisterComponentType( &m_staticModelComponent );
		m_entitySystem.unregisterComponentType( &m_transformComponent );

		m_skinnedModelComponent.dispose();
		m_staticModelComponent.dispose();
		m_transformComponent.dispose();

		m_entitySystem.dispose();
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

	void GameClient::update()
	{
		m_transformComponent.update();
	}

	void GameClient::render( GameRenderer& gameRenderer )
	{
		m_staticModelComponent.render( gameRenderer );
		m_skinnedModelComponent.render( gameRenderer );
	}
}