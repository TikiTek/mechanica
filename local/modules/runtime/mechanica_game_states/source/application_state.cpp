#include "tiki/game_states/application_state.hpp"

#include "tiki/base/debug_property.hpp"
#include "tiki/base/timer.hpp"
#include "tiki/framework/main_window.hpp"
#include "tiki/game/game.hpp"
#include "tiki/graphics/color_xkcd.hpp"
#include "tiki/graphics/graphics_system.hpp"
#include "tiki/mechanica_types/mechanica_units.hpp"
#include "tiki/resource/resource_request_pool.hpp"

#include "mechanica_base.hpp"

namespace tiki
{
	TIKI_DEBUGPROP_BOOL( s_gameBloomEnable, "Bloom/Enable", true );
	TIKI_DEBUGPROP_FLOAT( s_gameBloomThresholdR, "Bloom/ThresholdRed", 0.9f, 0.0f, 3.0f );
	TIKI_DEBUGPROP_FLOAT( s_gameBloomThresholdG, "Bloom/ThresholdGreen", 0.8f, 0.0f, 3.0f );
	TIKI_DEBUGPROP_FLOAT( s_gameBloomThresholdB, "Bloom/ThresholdBlue", 0.87f, 0.0f, 3.0f );

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

	TransitionState ApplicationState::processTransitionStep( uintreg currentStep, bool isCreating, bool isInital )
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

						Renderer2dCreationParameters parameters;
						parameters.width				= (uint16)graphicsSystem.getBackBuffer().getWidth();
						parameters.height				= (uint16)graphicsSystem.getBackBuffer().getHeight();
						parameters.layerCount			= MechanicaRenderLayer_Count;
						parameters.emissivLayerId		= MechanicaRenderLayer_Emissive;
						parameters.drawToWorldFactor	= DrawToUnitFactor;

						if ( !m_renderer.create( graphicsSystem, resourceRequestPool, parameters ) )
						{
							TIKI_TRACE_ERROR( "[applicationstate] Could not create Renderer.\n" );
							return TransitionState_Error;
						}

						m_renderParameters.backgroundColor			= TIKI_COLOR_XKCD_BLUEBERRY;
						m_renderParameters.enableBloom				= s_gameBloomEnable;
						m_renderParameters.bloomCutoffThreshold.r	= s_gameBloomThresholdR;
						m_renderParameters.bloomCutoffThreshold.g	= s_gameBloomThresholdG;
						m_renderParameters.bloomCutoffThreshold.b	= s_gameBloomThresholdB;
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
		m_renderer.update( (float)m_pGame->getFrameTimer().getElapsedTime() );
	}

	void ApplicationState::postRender( GraphicsContext& graphicsContext )
	{
		m_renderer.render( graphicsContext, m_renderParameters );
	}

	bool ApplicationState::processInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}

	void ApplicationState::processWindowEvent( const WindowEvent& windowEvent )
	{
		if ( windowEvent.type == WindowEventType_SizeChanged )
		{
			if( !m_renderer.resize( windowEvent.data.sizeChanged.size.x, windowEvent.data.sizeChanged.size.y ) )
			{
				// everything is broken

				//m_renderer.dispose( m_pGame->getResourceRequestPool() );
				//m_pGame->startTransition( GameState_Root, GameTransitionData() );
			}
		}
	}
}