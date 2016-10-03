#pragma once
#ifndef TIKI_TIKIARENAGAME_HPP
#define TIKI_TIKIARENAGAME_HPP

#include "tiki/gameapplication/gameapplication.hpp"

#include "tiki/framework/touchgamesystem.hpp"
#include "tiki/gameflow/gameflowsystem.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/resource/resourcerequestpool.hpp"

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
			GameStates_PhysicsTest,

		GameStates_Count
	};

	class Game : public GameApplication
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( Game );

	public:

		GameFlowSystem&			getGameFlowSystem()			{ return m_gameFlow; }
		ResourceRequestPool&	getResourceRequestPool()	{ return m_resourceRequestPool; }

	protected:

		virtual void			fillGameParameters( GameApplicationParamters& parameters ) TIKI_OVERRIDE TIKI_FINAL;
		virtual bool			initializeGame() TIKI_OVERRIDE TIKI_FINAL;
		virtual void			shutdownGame() TIKI_OVERRIDE TIKI_FINAL;

		virtual void			updateGame( bool wantToShutdown ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void			renderGame( GraphicsContext& graphicsContext ) const TIKI_OVERRIDE TIKI_FINAL;

		virtual bool			processGameInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void			processGameWindowEvent( const WindowEvent& windowEvent ) TIKI_OVERRIDE TIKI_FINAL;

	private:

		ResourceRequestPool		m_resourceRequestPool;

		GameFlowSystem			m_gameFlow;
		States*					m_pStates;

		TouchGameSystem			m_touchSystem;

	};
}

#endif // TIKI_TIKIARENAGAME_HPP
