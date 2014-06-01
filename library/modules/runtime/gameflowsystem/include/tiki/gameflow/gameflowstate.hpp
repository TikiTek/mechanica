#pragma once
#ifndef __TIKI_GAMEFLOAWSTATE_HPP_INCLUDED__
#define __TIKI_GAMEFLOAWSTATE_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#include "tiki/gameflow/statetree.hpp"

namespace tiki
{
	class GraphicsContext;
	struct InputEvent;

	class GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( GameFlowState );

	public:

								GameFlowState()		{ }
		virtual					~GameFlowState()	{ }

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital ) = 0;

		virtual void			update() = 0;
		virtual void			render( GraphicsContext& graphicsContext ) = 0;

		virtual bool			processInputEvent( const InputEvent& inputEvent ) = 0u;

	};
}

#endif // __TIKI_GAMEFLOAWSTATE_HPP_INCLUDED__
