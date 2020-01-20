#include "tiki/game_application/game_application.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/timer.hpp"
#include "tiki/input/input_event.hpp"
#include "tiki/runtime_shared/window_event.hpp"

namespace tiki
{
	struct GameApplicationData
	{
		GameApplicationData()
		{
		}
	};

	GameApplication::GameApplication()
	{
		m_pGameData	= nullptr;
	}

	GameApplication::~GameApplication()
	{
		TIKI_ASSERT( m_pGameData == nullptr );
	}

	void GameApplication::fillBaseParameters( BaseApplicationParamters& parameters )
	{
		fillGameParameters( m_parameters );

		parameters = m_parameters;
	}

	bool GameApplication::initializeApplication()
	{
		m_pGameData = TIKI_NEW( GameApplicationData );

		if ( !initializeDebugSystems() )
		{
			return false;
		}

		return initializeGame();
	}

	void GameApplication::shutdownApplication()
	{
		shutdownGame();

		shutdownDebugSystems();

		if( m_pGameData != nullptr )
		{
			TIKI_DELETE( m_pGameData );
			m_pGameData = nullptr;
		}
	}

	void GameApplication::updateApplication( bool wantToShutdown )
	{
		updateGame( wantToShutdown );
	}

	void GameApplication::renderApplication( GraphicsContext& graphicsContext ) const
	{
		renderGame( graphicsContext );
	}

	bool GameApplication::processBaseInputEvent( const InputEvent& inputEvent )
	{
		if( !processGameInputEvent( inputEvent ) )
		{
			return false;
		}

		return true;
	}

	void GameApplication::processBaseWindowEvent( const WindowEvent& windowEvent )
	{
		switch( windowEvent.type )
		{
		case WindowEventType_SizeChanged:
			break;
		}

		processGameWindowEvent( windowEvent );
	}

	bool GameApplication::initializeDebugSystems()
	{
		return true;
	}

	void GameApplication::shutdownDebugSystems()
	{
		if( m_pGameData == nullptr )
		{
			return;
		}
	}
}
