#pragma once
#ifndef TIKI_APPLICATIONSTATE_HPP__INCLUDED
#define TIKI_APPLICATIONSTATE_HPP__INCLUDED

#include "tiki/gameflow/gamestate.hpp"

namespace tiki
{
	enum ApplicationStateTransitionSteps
	{
		ApplicationStateTransitionSteps_RegisterResourceTypes,

		ApplicationStateTransitionSteps_Count
	};

	class ApplicationState : public GameState
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( ApplicationState );

	public:

		void					create();
		void					dispose();

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital );

		virtual void			update();
		virtual void			render( GraphicsContext& graphicsContext ) const;

	};
}

#endif // TIKI_APPLICATIONSTATE_HPP__INCLUDED
