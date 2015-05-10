#pragma once
#ifndef TIKI_APPLICATIONSTATE_HPP__INCLUDED
#define TIKI_APPLICATIONSTATE_HPP__INCLUDED

#include "tiki/gameflow/gameflowstate.hpp"

#include "tiki/renderer/fallbackrendereffect.hpp"
#include "tiki/renderer/gamerenderer.hpp"
#include "tiki/renderer/scenerendereffect.hpp"

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

		virtual bool			processInputEvent( const InputEvent& inputEvent );
		virtual void			processWindowEvent( const WindowEvent& windowEvent );

	private:

		GameRenderer			m_renderer;

		FallbackRenderEffect	m_fallbackRenderEffect;
		SceneRenderEffect		m_sceneRenderEffect;

	};
}

#endif // TIKI_APPLICATIONSTATE_HPP__INCLUDED
