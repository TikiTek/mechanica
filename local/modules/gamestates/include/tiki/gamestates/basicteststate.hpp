#pragma once
#ifndef __TIKI_BASICSTATE_HPP_INCLUDED__
#define __TIKI_BASICSTATE_HPP_INCLUDED__

#include "tiki/gameflow/gameflowstate.hpp"

#include "tiki/base/types.hpp"

namespace tiki
{
	enum BasicTestStateTransitionSteps
	{
		BasicTestStateTransitionSteps_Count
	};

	class BasicTestState : public GameFlowState
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( BasicTestState );

	public:								

		void					create();
		void					dispose();

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital );

		virtual void			update();
		virtual void			render( GraphicsContext& graphicsContext );

		virtual bool			processInputEvent( const InputEvent& inputEvent );

	};
}

#endif // __TIKI_BASICSTATE_HPP_INCLUDED__
