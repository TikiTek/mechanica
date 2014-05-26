
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

					if ( m_renderer.create( framework::getGraphicsSystem(), framework::getResourceManager(), params ) == false )
					{
						TIKI_TRACE_ERROR( "[applicationstate] Could not create GameRenderer.\n" );
						return TransitionState_Error;
					}
				}
				else
				{
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
		const WindowEvent* pEvent = framework::getMainWindow().getEventBuffer().getEventByType( WindowEventType_SizeChanged );
		if ( pEvent != nullptr )
		{
			if ( framework::getGraphicsSystem().resize( pEvent->data.sizeChanged.size.x, pEvent->data.sizeChanged.size.y ) )
			{
				if ( !m_renderer.resize( pEvent->data.sizeChanged.size.x, pEvent->data.sizeChanged.size.y ) )
				{
					m_renderer.dispose( framework::getResourceManager() );				
				}
			}
		}

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
}