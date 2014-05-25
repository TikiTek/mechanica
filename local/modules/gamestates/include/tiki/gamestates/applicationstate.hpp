#pragma once
#ifndef TIKI_APPLICATIONSTATE_HPP__INCLUDED
#define TIKI_APPLICATIONSTATE_HPP__INCLUDED

#include "tiki/gameflow/gameflowstate.hpp"

#include "tiki/renderer/gamerenderer.hpp"

namespace tiki
{
	enum ApplicationStateTransitionSteps
	{
		ApplicationStateTransitionSteps_CreateGameRenderer,

		ApplicationStateTransitionSteps_Count
	};

	class ApplicationState : public GameFlowState
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ApplicationState );

	public:

		void					create();
		void					dispose();

		GameRenderer&			getGameRenderer() { return m_renderer; }

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital );

		virtual void			update();
		virtual void			render( GraphicsContext& graphicsContext );

		bool					processInputEvent( const InputEvent& inputEvent );

	private:

		GameRenderer			m_renderer;

	};
}

#endif // TIKI_APPLICATIONSTATE_HPP__INCLUDED
