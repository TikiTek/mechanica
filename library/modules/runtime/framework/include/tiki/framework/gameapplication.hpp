#pragma once
#ifndef TIKI_GAMEFRAMEWORK_HPP
#define TIKI_GAMEFRAMEWORK_HPP

#include "tiki/base/types.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/graphics/graphicsrenderermode.hpp"

namespace tiki
{
	class GameApplication;
	class GraphicsContext;
	struct FrameworkData;
	struct InputEvent;
	struct WindowEvent;

	namespace framework
	{
		GameApplication&	getGame();
		FrameworkData&		getFrameworkData();
	}

	struct GameApplicationParamters
	{
		GameApplicationParamters()
		{
			screenWidth			= 900;
			screenHeight		= 600;
			fullScreen			= false;
			graphicsMode		= GraphicsRendererMode_Hardware;

			pWindowTitle		= "TikiEngine 3.0";

			pGamebuildPath		= "../../../../../gamebuild/";
		}

		uint					screenWidth;
		uint					screenHeight;
		bool					fullScreen;
		GraphicsRendererMode	graphicsMode;

		cstring					pWindowTitle;

		cstring					pGamebuildPath;

	};

	class GameApplication
	{
		friend int				framework::run();
		friend FrameworkData&	framework::getFrameworkData();

	protected:

									GameApplication();
		virtual						~GameApplication();

		int							run();

	protected:

		virtual void				fillParameters( GameApplicationParamters& parameters ) TIKI_PURE;
		virtual bool				initializeGame() TIKI_PURE;
		virtual void				shutdownGame() TIKI_PURE;

		virtual void				update( bool wantToShutdown ) TIKI_PURE;
		virtual void				render( GraphicsContext& graphicsContext ) const TIKI_PURE;

		virtual bool				processInputEvent( const InputEvent& inputEvent ) TIKI_PURE;
		virtual void				processWindowEvent( const WindowEvent& windowEvent ) TIKI_PURE;

		bool						m_running;

	private:
		
		bool						m_isInitialized;
		
		GameApplicationParamters	m_parameters;
		FrameworkData*				m_pFrameworkData;
	
		bool						initialize();
		void						shutdown();

		bool						initializePlatform();
		void						shutdownPlatform();
		void						updatePlatform();

		bool						initializeFramework();
		void						shutdownFramework();

		bool						initializeDebugSystems();
		void						shutdownDebugSystems();

		bool						frame();

	};
}

#endif // TIKI_GAMEFRAMEWORK_HPP
