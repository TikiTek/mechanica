#pragma once

#include "tiki/game_flow/game_flow_state.hpp"

#include "tiki/graphics/immediate_renderer.hpp"

namespace tiki
{
	class ApplicationState;

	enum CreditsStateTransitionSteps
	{
		CreditsStateTransitionSteps_Count
	};

	class CreditsState : public GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( CreditsState );

	public:

								CreditsState();
		virtual					~CreditsState();

		void					create( ApplicationState* pParentState );
		void					dispose();

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital ) TIKI_OVERRIDE;

		virtual void			update() TIKI_OVERRIDE;
		virtual void			render( GraphicsContext& graphicsContext ) TIKI_OVERRIDE;

		virtual bool			processInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE;

	private:

		ApplicationState*		m_pParentState;

		ImmediateRenderer		m_renderer;

	};
}
