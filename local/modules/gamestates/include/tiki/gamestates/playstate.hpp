#pragma once
#ifndef TIKI_PLAYSTATE_HPP__INCLUDED
#define TIKI_PLAYSTATE_HPP__INCLUDED

#include "tiki/gameflow/gameflowstate.hpp"

#include "tiki/gameplay/gamesession.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/renderer/postbloom.hpp"

namespace tiki
{
	class ApplicationState;
	class Game;

	enum PlayStateTransitionSteps
	{
		PlayStateTransitionSteps_InitializeGraphics,
		PlayStateTransitionSteps_CreateClientSystems,
		PlayStateTransitionSteps_SetRendererData,

		PlayStateTransitionSteps_Count
	};

	class PlayState : public GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( PlayState );

	public:

									PlayState();
		virtual						~PlayState();

		void						create( Game* pGame, ApplicationState* pParentState );
		void						dispose();

		virtual TransitionState		processTransitionStep( size_t currentStep, bool isCreating, bool isInital );

		virtual void				update();
		virtual void				render( GraphicsContext& graphicsContext );

		virtual bool				processInputEvent( const InputEvent& inputEvent );
		virtual void				processWindowEvent( const WindowEvent& windowEvent );

		const Camera&				getCamera() const;

	private:

		Game*						m_pGame;
		ApplicationState*			m_pParentState;

		GameRenderer*				m_pGameRenderer;
		ImmediateRenderer			m_immediateRenderer;
		PostProcessBloom			m_bloom;

		GameSession					m_gameSession;

	};
}

#endif // TIKI_PLAYSTATE_HPP__INCLUDED
