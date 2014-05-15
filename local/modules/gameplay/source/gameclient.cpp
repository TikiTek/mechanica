
#include "tiki/gameplay/gameclient.hpp"

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

	void GameClient::update()
	{
	}

	void GameClient::render( GameRenderer& gameRenderer )
	{
		m_staticModelComponent.render( gameRenderer );
		m_skinnedModelComponent.render( gameRenderer );
	}

}