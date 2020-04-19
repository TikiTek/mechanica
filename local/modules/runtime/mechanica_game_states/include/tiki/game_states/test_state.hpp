#pragma once

#include "tiki/game_flow/game_flow_state.hpp"

#include "tiki/gameplay/game_client.hpp"
#include "tiki/render_effects/post_bloom.hpp"
#include "tiki/runtime_shared/free_camera.hpp"

namespace tiki
{
	class Animation;
	class ApplicationState;
	class Font;
	class Game;
	class GameRenderer;
	class Model;
	class Texture;

	enum TestStateTransitionSteps
	{
		TestStateTransitionSteps_Initialize,
		TestStateTransitionSteps_LoadResources,
		TestStateTransitionSteps_CreateGameClient,
		TestStateTransitionSteps_SetRendererValues,

		TestStateTransitionSteps_Count
	};

	class TestState : public GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( TestState );

	public:

								TestState();
		virtual					~TestState();

		void					create( Game* pGame, ApplicationState* pApplicationState );
		void					dispose();

		virtual TransitionState	processTransitionStep( uintreg currentStep, bool isCreating, bool isInital ) TIKI_OVERRIDE_FINAL;

		virtual void			update() TIKI_OVERRIDE_FINAL;
		virtual void			render( GraphicsContext& graphicsContext ) TIKI_OVERRIDE_FINAL;
		virtual void			postRender( GraphicsContext& graphicsContext ) TIKI_OVERRIDE_FINAL;

		virtual bool			processInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE_FINAL;

	private:

		Game*										m_pGame;
		ApplicationState*							m_pApplicationState;

		Renderer2d*									m_pRenderer;

		const Font*									m_pFont;
		const EntityTemplateGenericDataResource*	m_pIsland1;
		const EntityTemplateGenericDataResource*	m_pIsland2;
		const EntityTemplateGenericDataResource*	m_pIsland3;

		Vector2										m_mousePosition;

		GameClient									m_gameClient;

		EntityId									m_playerEntityId;
		EntityId									m_islandEntityId;
	};
}
