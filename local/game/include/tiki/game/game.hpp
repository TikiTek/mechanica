#pragma once
#ifndef TIKI_TIKIARENAGAME_HPP
#define TIKI_TIKIARENAGAME_HPP

#include "tiki/framework/gameapplication.hpp"

#include "tiki/framework/touchgamesystem.hpp"
#include "tiki/game/framework_game.hpp"
#include "tiki/gameflow/gameflowsystem.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/resource/resourcerequestpool.hpp"
#include "tiki/runtimeshared/frameworkfactories.hpp"

namespace tiki
{
	struct States;

	enum GameStates
	{
		GameStates_Root,
			GameStates_Application,
				GameStates_Intro,
				GameStates_Menu,
				GameStates_Play,
				GameStates_Credits,
				GameStates_Test,
			GameStates_BasicTest,

		GameStates_Count
	};

	class Game : public GameApplication
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( Game );
		friend GameFlowSystem& framework::getGameFlowSystem();
		friend ResourceRequestPool& framework::getResourceRequestPool();
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		friend ImmediateRenderer& framework::getImmediateRenderer();
#endif

	protected:

		virtual void			fillParameters( GameApplicationParamters& parameters ) TIKI_OVERRIDE TIKI_FINAL;
		virtual bool			initializeGame() TIKI_OVERRIDE TIKI_FINAL;
		virtual void			shutdownGame() TIKI_OVERRIDE TIKI_FINAL;

		virtual void			update( bool wantToShutdown ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void			render( GraphicsContext& graphicsContext ) const TIKI_OVERRIDE TIKI_FINAL;

		virtual bool			processInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void			processWindowEvent( const WindowEvent& windowEvent ) TIKI_OVERRIDE TIKI_FINAL;

	private: // friend

		GameFlowSystem&			getGameFlowSystem() { return m_gameFlow; }
		ResourceRequestPool&	getResourceRequestPool() { return m_resourceRequestPool; }
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		ImmediateRenderer&		getImmediateRenderer() { return m_immediateRenderer; }
#endif

	private:

		FrameworkFactories		m_factories;

		ImmediateRenderer		m_immediateRenderer;

		ResourceRequestPool		m_resourceRequestPool;

		GameFlowSystem			m_gameFlow;
		States*					m_pStates;

		TouchGameSystem			m_touchSystem;

	};
}

#endif // TIKI_TIKIARENAGAME_HPP
