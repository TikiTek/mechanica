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

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital );
		virtual TransitionState	processCreationStep( size_t currentStep, bool isInital );
		virtual TransitionState	processDestructionStep( size_t currentStep, bool isInital );

		virtual void			update() TIKI_PURE;
		virtual void			render( GraphicsContext& graphicsContext ) { }
		virtual void			postRender( GraphicsContext& graphicsContext ) { }

		virtual bool			processInputEvent( const InputEvent& inputEvent ) TIKI_PURE;
		virtual void			processWindowEvent( const WindowEvent& windowEvent ) { }

	protected:

		TransitionState			waitForResources( const ResourceRequestPool& resourceRequestPool ) const;
	};
}
