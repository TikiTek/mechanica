#pragma once

#include "tiki/base/types.hpp"

#include "tiki/game_flow/state_tree.hpp"

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
