#pragma once

#include "tiki/game_flow/game_flow_state.hpp"

#include "tiki/gameplay/game_client.hpp"
#include "tiki/gameplay/level_renderer.hpp"
#include "tiki/graphics/immediate_renderer.hpp"
#include "tiki/render_effects/post_bloom.hpp"

namespace tiki
{
	class ApplicationState;
	class Game;

	enum PlayStateTransitionSteps
	{
		PlayStateTransitionSteps_CreateClientSystems,
		PlayStateTransitionSteps_LoadLevel,
		PlayStateTransitionSteps_SetRendererData,

		PlayStateTransitionSteps_Count
	};

	class PlayState : public GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( PlayState );

	public:

									PlayState();
		virtual						~PlayState();

		void						create( Game* pGame, ApplicationState* pApplicationState );
		void						dispose();

		virtual TransitionState		processCreationStep( uintreg currentStep, bool isInital ) TIKI_OVERRIDE_FINAL;
		virtual TransitionState		processDestructionStep( uintreg currentStep, bool isInital ) TIKI_OVERRIDE_FINAL;

		virtual void				update() TIKI_OVERRIDE_FINAL;
		virtual void				render( GraphicsContext& graphicsContext ) TIKI_OVERRIDE_FINAL;

		virtual bool				processInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE_FINAL;

	private:

		Game*						m_pGame;
		ApplicationState*			m_pApplicationState;

		float						m_scrollState = 0.0f;

		LevelRenderer				m_levelRenderer;
		GameClient					m_gameClient;
	};
}
