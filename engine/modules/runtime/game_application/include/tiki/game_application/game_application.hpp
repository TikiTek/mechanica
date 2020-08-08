#pragma once

#include "tiki/framework/base_application.hpp"

namespace tiki
{
	class DebugGui;
	class GameApplication;
	struct GameApplicationData;

	namespace framework
	{
		GameApplication& getGame();
	}

	struct GameApplicationParamters : public BaseApplicationParamters
	{
		GameApplicationParamters()
		{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
			assetBuildPath = Path( "../../../../../asset_build/" );
#endif
		}
	};

	class GameApplication : public BaseApplication
	{
		TIKI_NONCOPYABLE_CLASS( GameApplication );

	public:

	protected:

									GameApplication();
		virtual						~GameApplication();

		virtual void				fillBaseParameters( BaseApplicationParamters& parameters ) TIKI_OVERRIDE_FINAL;

		virtual bool				initializeApplication() TIKI_OVERRIDE_FINAL;
		virtual void				shutdownApplication() TIKI_OVERRIDE_FINAL;

		virtual void				updateApplication( bool wantToShutdown ) TIKI_OVERRIDE_FINAL;
		virtual void				renderApplication( GraphicsContext& graphicsContext ) const TIKI_OVERRIDE_FINAL;

		virtual bool				processBaseInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE_FINAL;
		virtual void				processBaseWindowEvent( const WindowEvent& windowEvent ) TIKI_OVERRIDE_FINAL;

		virtual void				fillGameParameters( GameApplicationParamters& parameters ) = 0;
		virtual bool				initializeGame() = 0;
		virtual void				shutdownGame() = 0;

		virtual void				updateGame( bool wantToShutdown ) = 0;
		virtual void				renderGame( GraphicsContext& graphicsContext ) const = 0;

		virtual bool				processGameInputEvent( const InputEvent& inputEvent ) = 0;
		virtual void				processGameWindowEvent( const WindowEvent& windowEvent ) = 0;

		bool						initializeDebugSystems();
		void						shutdownDebugSystems();

	private:

		GameApplicationParamters	m_parameters;
		GameApplicationData*		m_pGameData;

	};
}
