
#include "tiki/gamestates/applicationstate.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/framework/mainwindow.hpp"
#include "tiki/game/framework_game.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/resource/resourcerequestpool.hpp"

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
				GraphicsSystem& graphicsSystem = framework::getGraphicsSystem();
				ResourceRequestPool& resourceRequestPool = framework::getResourceRequestPool();

				if ( isCreating )
				{
					if ( isInital )
					{
						resourceRequestPool.resetError();

						GameRendererParamaters params;
						params.rendererWidth	= graphicsSystem.getBackBuffer().getWidth();
						params.rendererHeight	= graphicsSystem.getBackBuffer().getHeight();

						if ( !m_renderer.create( graphicsSystem, resourceRequestPool, params ) )
						{
							TIKI_TRACE_ERROR( "[applicationstate] Could not create GameRenderer.\n" );
							return TransitionState_Error;
						}

						if( !m_renderer.registerRenderEffect( m_fallbackRenderEffect, resourceRequestPool ) ||
							!m_renderer.registerRenderEffect( m_sceneRenderEffect, resourceRequestPool ) )
						{
							return TransitionState_Error;
						}
					}

					if ( resourceRequestPool.isFinish() )
					{
						if ( resourceRequestPool.hasError() )
						{
							return TransitionState_Error;
						}

						if ( !m_renderer.createShaderResources( graphicsSystem, resourceRequestPool ) )
						{
							TIKI_TRACE_ERROR( "[applicationstate] Could not create GameRenderer shaders.\n" );
							return TransitionState_Error;
						}

						return TransitionState_Finish;
					}

					return TransitionState_InProcess;
				}
				else
				{
					TIKI_ASSERT( isInital );

					m_renderer.unregisterRenderEffect( m_sceneRenderEffect, resourceRequestPool );
					m_renderer.unregisterRenderEffect( m_fallbackRenderEffect, resourceRequestPool );

					m_renderer.dispose( resourceRequestPool );
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

	void ApplicationState::postRender( GraphicsContext& graphicsContext )
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
				m_renderer.dispose( framework::getResourceRequestPool() );				
			}
		}
	}
}