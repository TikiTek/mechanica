#include "editor_application.hpp"

#include "tiki/base/timer.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include "../icon/icon.h"
#endif

namespace tiki
{
	EditorApplication::EditorApplication()
	{
		m_pProject	= TIKI_NEW( Project )();
		m_pEditor	= nullptr;
	}

	EditorApplication::~EditorApplication()
	{
	}

	void EditorApplication::fillToolParameters( ToolApplicationParamters& parameters )
	{
		parameters.pWindowTitle		= "TikiEditor";
		parameters.screenWidth		= 1600u;
		parameters.screenHeight		= 900u;
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
		parameters.windowIconId		= MAINICON;
#endif
		parameters.assetBuildPath	= m_pProject->getAssetBuildPath();
#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		parameters.pProject			= m_pProject;
#endif
	}

	bool EditorApplication::initializeTool()
	{
		m_pEditor = TIKI_NEW( Editor )( *m_pProject );

		if( !m_pEditor->create() )
		{
			TIKI_DELETE( m_pEditor );
			return false;
		}

		return true;
	}

	void EditorApplication::shutdownTool()
	{
		if( m_pEditor != nullptr )
		{
			m_pEditor->dispose();
			TIKI_DELETE( m_pEditor );
			m_pEditor = nullptr;
		}

		TIKI_DELETE( m_pProject );
		m_pProject = nullptr;
	}

	void EditorApplication::updateTool( bool wantToShutdown )
	{
		m_pEditor->update( (float)tool::getFrameTimer().getElapsedTime() );
	}

	void EditorApplication::renderTool( GraphicsContext& graphicsContext ) const
	{
		m_pEditor->render( graphicsContext );
	}

	bool EditorApplication::processToolInputEvent( const InputEvent& inputEvent )
	{
		return m_pEditor->processToolInputEvent( inputEvent );
	}

	void EditorApplication::processToolWindowEvent( const WindowEvent& windowEvent )
	{
	}

	void EditorApplication::doUi()
	{
		m_pEditor->doUi();
	}

	ToolApplication& tool::getTool()
	{
		static EditorApplication s_application;
		return s_application;
	}
}