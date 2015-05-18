
#include "tiki/gamestates/applicationstate.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/framework/mainwindow.hpp"
#include "tiki/graphics/graphicssystem.hpp"

namespace tiki
{
	void ApplicationState::create()
	{
	}

	void ApplicationState::dispose()
	{
	}

	TransitionState ApplicationState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case ApplicationStateTransitionSteps_CreateGameRenderer:
			{
				TIKI_ASSERT( isInital );

				if ( isCreating )
				{
					GameRendererParamaters params;
					params.rendererWidth	= framework::getGraphicsSystem().getBackBuffer().getWidth();
					params.rendererHeight	= framework::getGraphicsSystem().getBackBuffer().getHeight();

					if ( !m_renderer.create( framework::getGraphicsSystem(), framework::getResourceManager(), params ) )
					{
						TIKI_TRACE_ERROR( "[applicationstate] Could not create GameRenderer.\n" );
						return TransitionState_Error;
					}

					RendererContext& rendererContext = m_renderer.getRendererContext();
					m_fallbackRenderEffect.create( rendererContext, framework::getGraphicsSystem(), framework::getResourceManager() );
					m_sceneRenderEffect.create( rendererContext, framework::getGraphicsSystem(), framework::getResourceManager() );

					m_renderer.registerRenderEffect( &m_fallbackRenderEffect );
					m_renderer.registerRenderEffect( &m_sceneRenderEffect );
				}
				else
				{
					m_renderer.unregisterRenderEffect( &m_sceneRenderEffect );
					m_renderer.unregisterRenderEffect( &m_fallbackRenderEffect );
					m_sceneRenderEffect.dispose( framework::getGraphicsSystem(), framework::getResourceManager() );
					m_fallbackRenderEffect.dispose( framework::getGraphicsSystem(), framework::getResourceManager() );

					m_renderer.dispose( framework::getResourceManager() );
				}

				return TransitionState_Finish;
			}
			break;

		}

		return TransitionState_Error;
	}

	void ApplicationState::update()
	{
		m_renderer.update();
	}

	void ApplicationState::render( GraphicsContext& graphicsContext )
	{
		m_renderer.render( graphicsContext );
	}

	bool ApplicationState::processInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}

	void ApplicationState::processWindowEvent( const WindowEvent& windowEvent )
	{
		if ( windowEvent.type == WindowEventType_SizeChanged )
		{
			if ( !m_renderer.resize( windowEvent.data.sizeChanged.size.x, windowEvent.data.sizeChanged.size.y ) )
			{
				m_renderer.dispose( framework::getResourceManager() );				
			}
		}
	}
}