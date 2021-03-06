#pragma once

#include "tiki/game_flow/game_flow_state.hpp"

#include "tiki/renderer_2d/renderer_2d.hpp"

namespace tiki
{
	class Game;

	enum ApplicationStateTransitionSteps
	{
		ApplicationStateTransitionSteps_CreateGameRenderer,

		ApplicationStateTransitionSteps_Count
	};

	class ApplicationState : public GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( ApplicationState );

	public:

									ApplicationState();
		virtual						~ApplicationState();

		void						create( Game* pGame );
		void						dispose();

		Renderer2d&					getRenderer() { return m_renderer; }
		Renderer2dRenderParameters&	getRenderParameters() { return m_renderParameters; }

		virtual TransitionState		processTransitionStep( uintreg currentStep, bool isCreating, bool isInital ) TIKI_OVERRIDE_FINAL;

		virtual void				update() TIKI_OVERRIDE_FINAL;
		virtual void				postRender( GraphicsContext& graphicsContext ) TIKI_OVERRIDE_FINAL;

		virtual bool				processInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE_FINAL;
		virtual void				processWindowEvent( const WindowEvent& windowEvent ) TIKI_OVERRIDE_FINAL;

	private:

		Game*						m_pGame;

		Renderer2d					m_renderer;
		Renderer2dRenderParameters	m_renderParameters;
	};
}
