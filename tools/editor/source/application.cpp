#include "application.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/debugrenderer/debugrenderer.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/runtimeshared/windowevent.hpp"

namespace tiki
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::fillToolParameters( ToolApplicationParamters& parameters )
	{
		parameters.pGamebuildPath = "../../../../../../gamebuild";
	}

	bool Application::initializeTool()
	{
		if( !m_camera.create( getGraphicsSystem() ) )
		{
			return false;
		}

		if( !m_transformGizmo.create( m_camera ) )
		{
			return false;
		}

		return true;
	}

	void Application::shutdownTool()
	{
		m_transformGizmo.dispose();
		m_camera.dispose();
	}

	void Application::updateTool( bool wantToShutdown )
	{
		const double deltaTime = getFrameTimer().getElapsedTime();

		m_camera.update( deltaTime );
		m_transformGizmo.update( deltaTime );
	}

	void Application::renderTool( GraphicsContext& graphicsContext ) const
	{
		graphicsContext.clear( graphicsContext.getBackBuffer() );

		m_transformGizmo.render();

		debugrenderer::drawLineRay( m_testRay );

		debugrenderer::flush( getImmediateRenderer(), m_camera.getCamera() );
	}

	bool Application::processToolInputEvent( const InputEvent& inputEvent )
	{
		if( inputEvent.eventType == InputEventType_Mouse_ButtonDown && inputEvent.data.mouseButton.button == MouseButton_Left )
		{
			m_testRay = m_camera.getMouseRay();
		}

		m_camera.processInputEvent( inputEvent );
		m_transformGizmo.processInputEvent( inputEvent );

		return false;
	}

	void Application::processToolWindowEvent( const WindowEvent& windowEvent )
	{
		if( windowEvent.type == WindowEventType_SizeChanged )
		{
			m_camera.resize( windowEvent.data.sizeChanged.size.x, windowEvent.data.sizeChanged.size.y );
		}
	}

	ToolApplication& framework::getTool()
	{
		static Application s_application;
		return s_application;
	}
}