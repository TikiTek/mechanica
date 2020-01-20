#pragma once

#include "tiki/game_application/game_application.hpp"

#include "tiki/framework/touch_game_system.hpp"
#include "tiki/game_flow/game_flow_system.hpp"
#include "tiki/graphics/immediate_renderer.hpp"
#include "tiki/resource/resource_request_pool.hpp"

namespace tiki
{
	struct States;

	enum GameState
	{
		GameState_Root,
			GameState_Application,
				GameState_Intro,
				GameState_Menu,
				GameState_Play,
				GameState_Credits,
				GameState_Test,
			GameState_BasicTest,
			GameState_PhysicsTest,

		GameState_Count
	};

	struct GameTransitionPlayerData
	{
		string						levelName;
	};

	struct GameTransitionData
	{
		GameState					state;
		GameTransitionPlayerData	play;

		static GameTransitionData	createMenu();
		static GameTransitionData	createPlay( const string& levelName );
		static GameTransitionData	createExit();
	};

	class Game : public GameApplication
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( Game );

	public:

		void						startTransition( const GameTransitionData& data );
		const GameTransitionData&	getTransitionData() const { return m_transitionData; }

		ResourceRequestPool&		getResourceRequestPool()	{ return m_resourceRequestPool; }

	protected:

		virtual void				fillGameParameters( GameApplicationParamters& parameters ) TIKI_OVERRIDE_FINAL;
		virtual bool				initializeGame() TIKI_OVERRIDE_FINAL;
		virtual void				shutdownGame() TIKI_OVERRIDE_FINAL;

		virtual void				updateGame( bool wantToShutdown ) TIKI_OVERRIDE_FINAL;
		virtual void				renderGame( GraphicsContext& graphicsContext ) const TIKI_OVERRIDE_FINAL;

		virtual bool				processGameInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE_FINAL;
		virtual void				processGameWindowEvent( const WindowEvent& windowEvent ) TIKI_OVERRIDE_FINAL;

	private:

		ResourceRequestPool			m_resourceRequestPool;

		GameFlowSystem				m_gameFlow;
		GameTransitionData			m_transitionData;
		States*						m_pStates;

		TouchGameSystem				m_touchSystem;

		void						startFirstTransition();
	};
}
