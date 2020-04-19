#pragma once

#include "tiki/game_flow/game_flow_state.hpp"

#include "tiki/base/types.hpp"

namespace tiki
{
	class ApplicationState;

	enum IntroStateTransitionSteps
	{
		IntroStateTransitionSteps_Count
	};

	class IntroState : public GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( IntroState );

	public:

								IntroState();
		virtual					~IntroState();

		void					create( ApplicationState* pParentState );
		void					dispose();

		virtual TransitionState	processTransitionStep( uintreg currentStep, bool isCreating, bool isInital );

		virtual void			update();
		virtual void			render( GraphicsContext& graphicsContext );

		virtual bool			processInputEvent( const InputEvent& inputEvent );

	private:

		ApplicationState*		m_pParentState;

	};
}
