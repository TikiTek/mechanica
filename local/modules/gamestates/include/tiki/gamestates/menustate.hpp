#pragma once
#ifndef TIKI_MENUSTATE_HPP__INCLUDED
#define TIKI_MENUSTATE_HPP__INCLUDED

#include "tiki/gameflow/gameflowstate.hpp"

namespace tiki
{
	class ApplicationState;

	enum MenuStateTransitionSteps
	{
		MenuStateTransitionSteps_Count
	};
	
	class MenuState : public GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( MenuState );

	public:

								MenuState();
		virtual					~MenuState();

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

#endif // TIKI_MENUSTATE_HPP__INCLUDED
