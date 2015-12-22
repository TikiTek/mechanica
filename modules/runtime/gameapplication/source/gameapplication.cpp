#include "tiki/gameapplication/gameapplication.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/debuggui/debuggui.hpp"
#include "tiki/debugguiwindows/debugguiwindows.hpp"
#include "tiki/io/gamebuildfilesystem.hpp"
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

		GamebuildFileSystem	gamebuildFileSystem;

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

		parameters						= m_parameters;
		parameters.pResourceFileSystem	= &m_pGameData->gamebuildFileSystem;
	}

	bool GameApplication::initializeApplication()
	{
		if( !m_pGameData->gamebuildFileSystem.create( m_parameters.pGamebuildPath ) )
		{
			return false;
		}

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

		m_pGameData->gamebuildFileSystem.dispose();
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
