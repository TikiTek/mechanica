#include "editor_application.hpp"

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
	}

	void EditorApplication::renderTool( GraphicsContext& graphicsContext ) const
	{
	}

	bool EditorApplication::processToolInputEvent( const InputEvent& inputEvent )
	{
		return false;
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