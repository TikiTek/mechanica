#include "editor_application.hpp"

#include "tiki/toolapplication/tool_ui.hpp"

#include "res_package_editor.hpp"

#include <imgui.h>

namespace tiki
{
	EditorApplication::EditorApplication()
	{
		//QSettings settings( "TikiTek", "editor" );
		//restoreGeometry( settings.value( "window/geometry" ).toByteArray() );
		//restoreState( settings.value( "window/windowState" ).toByteArray() );
	}

	EditorApplication::~EditorApplication()
	{
		//QSettings settings( "TikiTek", "editor" );
		//settings.setValue( "window/geometry", saveGeometry() );
		//settings.setValue( "window/windowState", saveState() );
	}

	void EditorApplication::fillToolParameters( ToolApplicationParamters& parameters )
	{
		parameters.pWindowTitle		= "TikiEditor";
		parameters.screenWidth		= 1600u;
		parameters.screenHeight		= 900u;
		parameters.assetBuildPath	= m_editor.getProject().getAssetBuildPath();
#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		parameters.pProject			= &m_editor.getProject();
#endif
	}

	bool EditorApplication::initializeTool()
	{
		return m_editor.create();
	}

	void EditorApplication::shutdownTool()
	{
		m_editor.dispose();
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
		m_editor.doUi();
	}

	ToolApplication& tool::getTool()
	{
		static EditorApplication s_application;
		return s_application;
	}
}