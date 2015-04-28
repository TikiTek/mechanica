#pragma once
#ifndef TIKI_PLAYSTATE_HPP__INCLUDED
#define TIKI_PLAYSTATE_HPP__INCLUDED

#include "tiki/gameflow/gameflowstate.hpp"

#include "tiki/gameplay/gameclient.hpp"
#include "tiki/gameplay/gamesession.hpp"
#include "tiki/gameplay/playercamera.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/renderer/postbloom.hpp"

namespace tiki
{
	class ApplicationState;

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

		void						create( ApplicationState* pParentState );
		void						dispose();

		virtual TransitionState		processTransitionStep( size_t currentStep, bool isCreating, bool isInital );

		virtual void				update();
		virtual void				render( GraphicsContext& graphicsContext );

		virtual bool				processInputEvent( const InputEvent& inputEvent );

	private:
		
		ApplicationState*			m_pParentState;

		GameRenderer*				m_pGameRenderer;
		ImmediateRenderer			m_immediateRenderer;
		PostProcessBloom			m_bloom;

		PlayerCamera				m_playerCamera;
		GameClient					m_gameClient;
		GameSession					m_gameSession;

	};
}

#endif // TIKI_PLAYSTATE_HPP__INCLUDED
