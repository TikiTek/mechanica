#pragma once
#ifndef TIKI_APPLICATIONSTATE_HPP__INCLUDED
#define TIKI_APPLICATIONSTATE_HPP__INCLUDED

#include "tiki/gameflow/gameflowstate.hpp"

#include "tiki/renderer/fallbackrendereffect.hpp"
#include "tiki/renderer/gamerenderer.hpp"
#include "tiki/renderer/scenerendereffect.hpp"

namespace tiki
{
	class Game;

	enum ApplicationStateTransitionSteps
	{
		ApplicationStateTransitionSteps_CreateGameRenderer,

		ApplicationStateTransitionSteps_Count
	};

	class ApplicationState : public GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( ApplicationState );

	public:

								ApplicationState();
		virtual					~ApplicationState();

		void					create( Game* pGame );
		void					dispose();

		GameRenderer&			getGameRenderer() { return m_renderer; }

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital ) TIKI_OVERRIDE TIKI_FINAL;

		virtual void			update() TIKI_OVERRIDE TIKI_FINAL;
		virtual void			postRender( GraphicsContext& graphicsContext ) TIKI_OVERRIDE TIKI_FINAL;

		virtual bool			processInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void			processWindowEvent( const WindowEvent& windowEvent ) TIKI_OVERRIDE TIKI_FINAL;

	private:

		Game*					m_pGame;

		GameRenderer			m_renderer;

		FallbackRenderEffect	m_fallbackRenderEffect;
		SceneRenderEffect		m_sceneRenderEffect;

	};
}

#endif // TIKI_APPLICATIONSTATE_HPP__INCLUDED
