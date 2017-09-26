#pragma once
#ifndef TIKI_INTROSTATE_HPP__INCLUDED
#define TIKI_INTROSTATE_HPP__INCLUDED

#include "tiki/gameflow/gameflowstate.hpp"

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

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital );

		virtual void			update();
		virtual void			render( GraphicsContext& graphicsContext );

		virtual bool			processInputEvent( const InputEvent& inputEvent );

	private:

		ApplicationState*		m_pParentState;

	};
}

#endif // TIKI_INTROSTATE_HPP__INCLUDED
