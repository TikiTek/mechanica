#pragma once
#ifndef TIKI_TIKIARENAGAME_HPP
#define TIKI_TIKIARENAGAME_HPP

#include "tiki/framework/gameframework.hpp"

#include "tiki/framework/frameworkfactories.hpp"
#include "tiki/framework/touchgamesystem.hpp"
#include "tiki/gameflow/gameflowsystem.hpp"

namespace tiki
{
	struct TikiArenaStates;

	enum TikiArenaGameStates
	{
		TikiArenaGameStates_Root,
		TikiArenaGameStates_Intro,
		TikiArenaGameStates_Application,
		TikiArenaGameStates_Play,
		TikiArenaGameStates_Test
	};

	class TikiArenaGame : public GameFramework
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( TikiArenaGame );

	public:
		
		GameFlowSystem&			getGameFlowSystem() { return m_gameFlow; }

	protected:

		virtual void			fillParameters( GameFrameworkParamters& parameters );
		virtual bool			initialize();
		virtual void			shutdown();

		virtual void			update();
		virtual void			render( GraphicsContext& graphicsContext ) const;

		virtual bool			processInputEvent( const InputEvent& inputEvent );

	private:

		FrameworkFactories		m_factories;

		GameFlowSystem			m_gameFlow;
		TikiArenaStates*		m_pStates;

		TouchGameSystem			m_touchSystem;

	};
}

#endif // TIKI_TIKIARENAGAME_HPP
