#pragma once
#ifndef __TIKI_GAMEFLOAWSTATE_HPP_INCLUDED__
#define __TIKI_GAMEFLOAWSTATE_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#include "tiki/gameflow/statetree.hpp"

namespace tiki
{
	class GraphicsContext;
	struct InputEvent;
	struct WindowEvent;

	class GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( GameFlowState );

	public:

								GameFlowState()		{ }
		virtual					~GameFlowState()	{ }

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital ) TIKI_PURE;

		virtual void			update() TIKI_PURE;
		virtual void			render( GraphicsContext& graphicsContext ) { };
		virtual void			postRender( GraphicsContext& graphicsContext ) { }

		virtual bool			processInputEvent( const InputEvent& inputEvent ) TIKI_PURE;
		virtual void			processWindowEvent( const WindowEvent& windowEvent ) { }

	};
}

#endif // __TIKI_GAMEFLOAWSTATE_HPP_INCLUDED__
