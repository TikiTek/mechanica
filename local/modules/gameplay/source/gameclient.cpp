
#include "tiki/gameplay/gameclient.hpp"

namespace tiki
{
	bool GameClient::create()
	{
		EntitySystemParameters entitySystemParams;
		entitySystemParams.typeRegisterMaxCount		= MaxTypeCount;
		entitySystemParams.storageChunkSize			= ChunkSize;
		entitySystemParams.storageChunkCount		= ChunkCount;

		if ( !m_entitySystem.create( entitySystemParams ) )
		{
			dispose();
			return false;
		}

		m_entitySystem.registerComponentType( &m_staticModelComponent );
		m_entitySystem.registerComponentType( &m_skinnedModelComponent );

		return true;
	}

	void GameClient::dispose()
	{
		m_entitySystem.unregisterComponentType( &m_skinnedModelComponent );
		m_entitySystem.unregisterComponentType( &m_staticModelComponent );

		m_entitySystem.dispose();
	}

	void GameClient::update()
	{

	}

	void GameClient::render()
	{

	}

}