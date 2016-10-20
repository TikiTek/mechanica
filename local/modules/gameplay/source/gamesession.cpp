#include "tiki/gameplay/gamesession.hpp"

#include "tiki/graphics/model.hpp"
#include "tiki/physics2d/physics2dboxshape.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	GameSession::GameSession()
	{
	}

	GameSession::~GameSession()
	{
	}

	bool GameSession::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		if( !m_gameClient.create( graphicsSystem ) )
		{
			return false;
		}		

		return true;
	}

	void GameSession::dispose( ResourceManager& resourceManager )
	{
		m_gameClient.dispose();
	}

	void GameSession::update( float timeDelta, float totalGameTime )
	{
		GameClientUpdateContext gameClientUpdateContext;
		gameClientUpdateContext.timeDelta		= timeDelta;
		gameClientUpdateContext.totalGameTime	= totalGameTime;

		m_gameClient.update( gameClientUpdateContext );
	}

	void GameSession::render( GraphicsContext& graphicsContext )
	{
		m_gameClient.render( graphicsContext );
	}

	bool GameSession::processInputEvent( const InputEvent& inputEvent )
	{
		return m_gameClient.processInputEvent( inputEvent );
	}
}