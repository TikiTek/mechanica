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

	TransitionState PlayState::processCreationStep( size_t currentStep, bool isInital )
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

				return waitForResources( resourceRequestPool );
			}
			break;

		case PlayStateTransitionSteps_SetRendererData:
			{
				Renderer2dRenderParameters& renderParameters = m_pApplicationState->getRenderParameters();

				renderParameters.enableBloom = true;
			}
			break;

		default:
			break;
		}

		return TransitionState_Finish;
	}

	TransitionState PlayState::processDestructionStep( size_t currentStep, bool isInital )
	{
		switch( currentStep )
		{
		case PlayStateTransitionSteps_SetRendererData:
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
		GameClientUpdateContext updateContext;
		updateContext.gameTime = m_pGame->getFrameTimer().getTime();

		m_gameClient.update( updateContext );
	}

	void PlayState::render( GraphicsContext& graphicsContext )
	{
		m_gameClient.render( m_pApplicationState->getRenderer() );
	}

	bool PlayState::processInputEvent( const InputEvent& inputEvent )
	{
		if( m_gameClient.processInputEvent( inputEvent ) )
		{
			return true;
		}

		return false;
	}
}