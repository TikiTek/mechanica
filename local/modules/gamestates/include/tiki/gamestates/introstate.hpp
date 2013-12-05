#pragma once
#ifndef TIKI_INTROSTATE_HPP__INCLUDED
#define TIKI_INTROSTATE_HPP__INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/gameflow/gamestate.hpp"

namespace tiki
{
	enum IntroStateTransitionSteps
	{
		IntroStateTransitionSteps_Count
	};

	class IntroState : public GameState
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( IntroState );

	public:								

		void					create();
		void					dispose();

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital );

		virtual void			update();
		virtual void			render( GraphicsContext* pGraphicsContext ) const;

	};
}

#endif // TIKI_INTROSTATE_HPP__INCLUDED
