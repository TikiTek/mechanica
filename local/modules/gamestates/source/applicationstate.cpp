
#include "tiki/gamestates/applicationstate.hpp"

#include "tiki/framework/mainwindow.hpp"
#include "tiki/game/game.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/resource/resourcerequestpool.hpp"

namespace tiki
{
	ApplicationState::ApplicationState()
	{
		m_pGame = nullptr;
	}

	ApplicationState::~ApplicationState()
	{
		TIKI_ASSERT( m_pGame == nullptr );
	}

	void ApplicationState::create( Game* pGame )
	{
		TIKI_ASSERT( pGame != nullptr );
		TIKI_ASSERT( m_pGame == nullptr );

		m_pGame = pGame;
	}

	void ApplicationState::dispose()
	{
		m_pGame = nullptr;
	}

	TransitionState ApplicationState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case ApplicationStateTransitionSteps_CreateGameRenderer:
			{
				GraphicsSystem& graphicsSystem = m_pGame->getGraphicsSystem();
				ResourceRequestPool& resourceRequestPool = m_pGame->getResourceRequestPool();

				if ( isCreating )
				{
					if ( isInital )
					{
						resourceRequestPool.resetError();

						uint16 width	= (uint16)graphicsSystem.getBackBuffer().getWidth();
						uint16 height	= (uint16)graphicsSystem.getBackBuffer().getHeight();

						if ( !m_renderer.create( graphicsSystem, resourceRequestPool, width, height, 5u ) )
						{
							TIKI_TRACE_ERROR( "[applicationstate] Could not create Renderer.\n" );
							return TransitionState_Error;
						}

						//if( !m_renderer.registerRenderEffect( m_fallbackRenderEffect, resourceRequestPool ) ||
						//	!m_renderer.registerRenderEffect( m_sceneRenderEffect, resourceRequestPool ) )
						//{
						//	return TransitionState_Error;
						//}
					}

					if ( resourceRequestPool.isFinish() )
					{
						if ( resourceRequestPool.hasError() )
						{
							return TransitionState_Error;
						}

						if ( !m_renderer.createResources( graphicsSystem, resourceRequestPool ) )
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

					//m_renderer.unregisterRenderEffect( m_sceneRenderEffect, resourceRequestPool );
					//m_renderer.unregisterRenderEffect( m_fallbackRenderEffect, resourceRequestPool );

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
	}

	void ApplicationState::postRender( GraphicsContext& graphicsContext )
	{
	}

	bool ApplicationState::processInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}

	void ApplicationState::processWindowEvent( const WindowEvent& windowEvent )
	{
		if ( windowEvent.type == WindowEventType_SizeChanged )
		{
			//if ( !m_renderer.resize( windowEvent.data.sizeChanged.size.x, windowEvent.data.sizeChanged.size.y ) )
			//{
			//	m_renderer.dispose( m_pGame->getResourceRequestPool() );
			//	m_pGame->getGameFlowSystem().startTransition( GameStates_Root )
			//}
		}
	}
}