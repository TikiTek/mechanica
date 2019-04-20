#pragma once

#include "tiki/game_flow/game_flow_state.hpp"

#include "tiki/gameplay/game_session.hpp"
#include "tiki/graphics/immediate_renderer.hpp"
#include "tiki/render_effects/post_bloom.hpp"

namespace tiki
{
	class ApplicationState;
	class Game;

	enum PlayStateTransitionSteps
	{
		PlayStateTransitionSteps_InitializeGraphics,
		PlayStateTransitionSteps_CreateClientSystems,
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

		virtual TransitionState		processTransitionStep( size_t currentStep, bool isCreating, bool isInital );

		virtual void				update();
		virtual void				render( GraphicsContext& graphicsContext );

		virtual bool				processInputEvent( const InputEvent& inputEvent );
		virtual void				processWindowEvent( const WindowEvent& windowEvent );

	private:

		Game*						m_pGame;
		ApplicationState*			m_pApplicationState;

		Renderer2d*					m_pRenderer;
		//ImmediateRenderer			m_immediateRenderer;
		//PostProcessBloom			m_bloom;

		GameSession					m_gameSession;

	};
}
