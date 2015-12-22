#pragma once
#ifndef TIKI_GAMEAPPLICATION_HPP_INCLUDED
#define TIKI_GAMEAPPLICATION_HPP_INCLUDED

#include "tiki/framework/baseapplication.hpp"

namespace tiki
{
	class DebugGui;
	class GameApplication;
	class IWebInterface;
	struct GameApplicationkData;

	namespace framework
	{
		GameApplication& getGame();
	}

	struct GameApplicationParamters : public BaseApplicationParamters
	{
		GameApplicationParamters()
		{
			pGamebuildPath = "../../../../../gamebuild/";
		}

		cstring					pGamebuildPath;
	};

	class GameApplication : public BaseApplication
	{
		TIKI_NONCOPYABLE_CLASS( GameApplication );

	public:

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		DebugGui&						getDebugGui() const;
#endif
#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		IWebInterface*					getWebInterface() const;
#endif

	protected:

									GameApplication();
		virtual						~GameApplication();

	protected:

		virtual void				fillBaseParameters( BaseApplicationParamters& parameters ) TIKI_OVERRIDE TIKI_FINAL;
		virtual bool				initializeFileSystem() TIKI_OVERRIDE TIKI_FINAL;
		virtual void				shutdownFileSystem() TIKI_OVERRIDE TIKI_FINAL;
		virtual bool				initializeApplication() TIKI_OVERRIDE TIKI_FINAL;
		virtual void				shutdownApplication() TIKI_OVERRIDE TIKI_FINAL;

		virtual void				updateApplication( bool wantToShutdown ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void				renderApplication( GraphicsContext& graphicsContext ) const TIKI_OVERRIDE TIKI_FINAL;

		virtual bool				processBaseInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void				processBaseWindowEvent( const WindowEvent& windowEvent ) TIKI_OVERRIDE TIKI_FINAL;

		virtual void				fillGameParameters( GameApplicationParamters& parameters ) TIKI_PURE;
		virtual bool				initializeGame() TIKI_PURE;
		virtual void				shutdownGame() TIKI_PURE;

		virtual void				updateGame( bool wantToShutdown ) TIKI_PURE;
		virtual void				renderGame( GraphicsContext& graphicsContext ) const TIKI_PURE;

		virtual bool				processGameInputEvent( const InputEvent& inputEvent ) TIKI_PURE;
		virtual void				processGameWindowEvent( const WindowEvent& windowEvent ) TIKI_PURE;

		bool						initializeDebugSystems();
		void						shutdownDebugSystems();

	private:

		GameApplicationParamters	m_parameters;
		GameApplicationkData*		m_pGameData;

	};
}

#endif // TIKI_GAMEAPPLICATION_HPP_INCLUDED
