#pragma once

#include "tiki/base/types.hpp"

#include "tiki/game_flow/state_tree.hpp"

namespace tiki
{
	class GraphicsContext;
	class ResourceRequestPool;
	struct InputEvent;
	struct WindowEvent;

	class GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( GameFlowState );

	public:

								GameFlowState();
		virtual					~GameFlowState();

		virtual TransitionState	processTransitionStep( uintreg currentStep, bool isCreating, bool isInital );
		virtual TransitionState	processCreationStep( uintreg currentStep, bool isInital );
		virtual TransitionState	processDestructionStep( uintreg currentStep, bool isInital );

		virtual void			update() = 0;
		virtual void			render( GraphicsContext& graphicsContext ) { }
		virtual void			postRender( GraphicsContext& graphicsContext ) { }

		virtual bool			processInputEvent( const InputEvent& inputEvent ) = 0;
		virtual void			processWindowEvent( const WindowEvent& windowEvent ) { }

	protected:

		TransitionState			waitForResources( const ResourceRequestPool& resourceRequestPool ) const;
	};
}
