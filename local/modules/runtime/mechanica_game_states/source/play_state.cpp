#include "tiki/game_states/play_state.hpp"

#include "tiki/base/debug_property.hpp"
#include "tiki/base/timer.hpp"
#include "tiki/game/game.hpp"
#include "tiki/game_states/application_state.hpp"
#include "tiki/graphics/graphics_system.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/runtime_shared/window_event.hpp"

namespace tiki
{
	PlayState::PlayState()
	{
		m_pGame				= nullptr;
		m_pApplicationState	= nullptr;
	}

	PlayState::~PlayState()
	{
		TIKI_ASSERT( m_pGame				== nullptr );
		TIKI_ASSERT( m_pApplicationState	== nullptr );
	}

	void PlayState::create( Game* pGame, ApplicationState* pApplicationState )
	{
		m_pGame				= pGame;
		m_pApplicationState	= pApplicationState;
	}

	void PlayState::dispose()
	{
		m_pGame				= nullptr;
		m_pApplicationState	= nullptr;
	}

	TransitionState PlayState::processCreationStep( uintreg currentStep, bool isInital )
	{
		switch( currentStep )
		{
		case PlayStateTransitionSteps_CreateClientSystems:
			{
				if( !m_gameClient.create() )
				{
					return TransitionState_Error;
				}
			}
			break;

		case PlayStateTransitionSteps_LoadLevel:
			{
				ResourceRequestPool& resourceRequestPool = m_pGame->getResourceRequestPool();
				if( isInital )
				{
					m_gameClient.startLoadLevel( resourceRequestPool, m_pGame->getTransitionData().play.levelName );
				}

				switch( m_gameClient.finalizeLoadLevel( resourceRequestPool ) )
				{
				case GameClientLoadResult_InProcess:
					return TransitionState_InProcess;

				case GameClientLoadResult_Successful:
					return TransitionState_Finish;

				default:
				case GameClientLoadResult_Error:
					return TransitionState_Error;
				}
			}
			break;

		case PlayStateTransitionSteps_SetRendererData:
			{
				if( !m_levelRenderer.create( m_gameClient.getLevel()->getData().theme->getData() ) )
				{
					return TransitionState_Error;
				}
			}
			break;

		default:
			break;
		}

		return TransitionState_Finish;
	}

	TransitionState PlayState::processDestructionStep( uintreg currentStep, bool isInital )
	{
		switch( currentStep )
		{
		case PlayStateTransitionSteps_SetRendererData:
			{
				m_levelRenderer.dispose();
			}
			break;

		case PlayStateTransitionSteps_LoadLevel:
			{
				m_gameClient.unloadLevel( m_pGame->getResourceRequestPool() );
			}
			break;

		case PlayStateTransitionSteps_CreateClientSystems:
			{
				m_gameClient.dispose();
			}
			break;

		default:
			break;
		}

		return TransitionState_Finish;
	}

	void PlayState::update()
	{
		MechanicaUpdateContext updateContext;
		updateContext.renderUnitSize = mech::toUnitSize( m_pApplicationState->getRenderer().getVectorSize() );
		updateContext.gameTime = m_pGame->getFrameTimer().getTime();

		m_levelRenderer.update( updateContext );
		m_gameClient.update( updateContext );

#if TIKI_ENABLED( TIKI_DEBUG_RENDERER )
		debugrenderer::drawText( Vector2::create( 10.0f, 10.0f), TIKI_COLOR_YELLOW, "Zoom: %.3f", m_pApplicationState->getRenderer().getTargetZoom() );
		debugrenderer::drawText( Vector2::create( 10.0f, 20.0f ), TIKI_COLOR_YELLOW, "Wheel: %.3f", m_scrollState );
#endif
	}

	void PlayState::render( GraphicsContext& graphicsContext )
	{
		m_levelRenderer.render( m_pApplicationState->getRenderer() );
		m_gameClient.applyRenderParameters( m_pApplicationState->getRenderParameters() );
		m_gameClient.render( m_pApplicationState->getRenderer() );
	}

	bool PlayState::processInputEvent( const InputEvent& inputEvent )
	{
#if 1
		if( inputEvent.eventType == InputEventType_Mouse_Wheel )
		{
			m_scrollState += inputEvent.data.mouseWheel.offset;

			const float targetZoom = ((m_scrollState / 1000.0f) + 1.0f);
			if( targetZoom > 0u )
			{
				m_pApplicationState->getRenderer().setTargetZoom( targetZoom );
			}
		}
#endif

		if( m_gameClient.processInputEvent( inputEvent ) )
		{
			return true;
		}

		return false;
	}
}