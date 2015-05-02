#pragma once
#ifndef TIKI_TIKIARENAGAME_HPP
#define TIKI_TIKIARENAGAME_HPP

#include "tiki/framework/gameframework.hpp"

#include "tiki/runtimeshared/frameworkfactories.hpp"
#include "tiki/framework/touchgamesystem.hpp"
#include "tiki/game/framework_game.hpp"
#include "tiki/gameflow/gameflowsystem.hpp"

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

	class Game : public GameFramework
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( Game );
		friend GameFlowSystem& framework::getGameFlowSystem();

	protected:

		virtual void			fillParameters( GameFrameworkParamters& parameters );
		virtual bool			initialize();
		virtual void			shutdown();

		virtual void			update();
		virtual void			render( GraphicsContext& graphicsContext ) const;

		virtual bool			processInputEvent( const InputEvent& inputEvent );

	private: // friend

		GameFlowSystem&			getGameFlowSystem() { return m_gameFlow; }

	private:

		FrameworkFactories		m_factories;

		GameFlowSystem			m_gameFlow;
		States*					m_pStates;

		TouchGameSystem			m_touchSystem;

	};
}

#endif // TIKI_TIKIARENAGAME_HPP
