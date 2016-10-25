#include "tiki/qtapplication/toolapplication.hpp"

namespace tiki
{
	ToolApplication::ToolApplication()
	{
	}

	ToolApplication::~ToolApplication()
	{
	}

	void ToolApplication::fillBaseParameters( BaseApplicationParamters& parameters )
	{
		fillToolParameters( m_parameters );
		parameters = m_parameters;
	}

	bool ToolApplication::initializeApplication()
	{
		return initializeTool();
	}

	void ToolApplication::shutdownApplication()
	{
		shutdownTool();
	}

	void ToolApplication::updateApplication( bool wantToShutdown )
	{
		updateTool( wantToShutdown );

		if( wantToShutdown )
		{
			m_running = false;
		}
	}

	void ToolApplication::renderApplication( GraphicsContext& graphicsContext ) const
	{
		renderTool( graphicsContext );
	}

	bool ToolApplication::processBaseInputEvent( const InputEvent& inputEvent )
	{
		return processToolInputEvent( inputEvent );
	}

	void ToolApplication::processBaseWindowEvent( const WindowEvent& windowEvent )
	{
		processToolWindowEvent( windowEvent );
	}
}
