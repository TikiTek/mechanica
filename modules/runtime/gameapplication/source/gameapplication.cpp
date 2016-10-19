#include "tiki/gameapplication/gameapplication.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/debuggui/debuggui.hpp"
#include "tiki/debugguiwindows/debugguiwindows.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/runtimeshared/windowevent.hpp"
#include "tiki/toollibraries/iwebinterrface.hpp"

namespace tiki
{
	struct GameApplicationkData
	{
		GameApplicationkData()
		{
#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
			pWebInterface = nullptr;
#endif
		}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		DebugGui			debugGui;
		DebugGuiWindows		debugGuiWindows;
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		IWebInterface*		pWebInterface;
#endif
	};

	GameApplication::GameApplication()
	{
		m_pGameData	= nullptr;
	}

	GameApplication::~GameApplication()
	{
		TIKI_ASSERT( m_pGameData == nullptr );
	}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	DebugGui& GameApplication::getDebugGui() const
	{
		return m_pGameData->debugGui;
	}
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
	IWebInterface* GameApplication::getWebInterface() const
	{
		return m_pGameData->pWebInterface;
	}
#endif

	void GameApplication::fillBaseParameters( BaseApplicationParamters& parameters )
	{
		fillGameParameters( m_parameters );

		parameters = m_parameters;
	}

	bool GameApplication::initializeApplication()
	{
		m_pGameData = TIKI_NEW( GameApplicationkData );

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
			TIKI_MEMORY_DELETE_OBJECT( m_pGameData );
			m_pGameData = nullptr;
		}
	}

	void GameApplication::updateApplication( bool wantToShutdown )
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_pGameData->debugGui.update( getFrameTimer().getElapsedTime() );
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		m_pGameData->pWebInterface->update();
#endif

		updateGame( wantToShutdown );
	}

	void GameApplication::renderApplication( GraphicsContext& graphicsContext ) const
	{
		renderGame( graphicsContext );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_pGameData->debugGui.render( graphicsContext );
#endif
	}

	bool GameApplication::processBaseInputEvent( const InputEvent& inputEvent )
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		if( m_pGameData->debugGui.processInputEvent( inputEvent ) )
		{
			return true;
		}
#endif

		if( !processGameInputEvent( inputEvent ) )
		{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
			if( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_F1 )
			{
				m_pGameData->debugGui.setActive( !m_pGameData->debugGui.getActive() );

				return true;
			}
#endif
			return false;
		}

		return true;
	}

	void GameApplication::processBaseWindowEvent( const WindowEvent& windowEvent )
	{
		switch( windowEvent.type )
		{
		case WindowEventType_SizeChanged:
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
			m_pGameData->debugGui.setScreenSize( vector::create( (float)windowEvent.data.sizeChanged.size.x, (float)windowEvent.data.sizeChanged.size.y ) );
#endif
			break;
		}

		processGameWindowEvent( windowEvent );
	}

	bool GameApplication::initializeDebugSystems()
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		if ( !m_pGameData->debugGui.create( getGraphicsSystem(), getResourceManager() ) )
		{
			return false;
		}

		m_pGameData->debugGuiWindows.create( m_pGameData->debugGui );
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		m_pGameData->pWebInterface = createWebInterface();
		if ( m_pGameData->pWebInterface == nullptr || !m_pGameData->pWebInterface->create() )
		{
			return false;
		}
#endif

		return true;
	}

	void GameApplication::shutdownDebugSystems()
	{
		if( m_pGameData == nullptr )
		{
			return;
		}

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		if ( m_pGameData->pWebInterface != nullptr )
		{
			m_pGameData->pWebInterface->dispose();

			disposeWebInterface( m_pGameData->pWebInterface );
			m_pGameData->pWebInterface = nullptr;
		}
#endif

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_pGameData->debugGuiWindows.dispose();
		m_pGameData->debugGui.dispose( getGraphicsSystem(), getResourceManager() );
#endif
	}
}
