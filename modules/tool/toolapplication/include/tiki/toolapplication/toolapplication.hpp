#pragma once
#ifndef TIKI_GAMEAPPLICATION_HPP_INCLUDED
#define TIKI_GAMEAPPLICATION_HPP_INCLUDED

#include "tiki/base/types.hpp"

#include "tiki/graphics/graphicsrenderermode.hpp"

namespace tiki
{
	class DebugGui;
	class GameApplication;
	class GraphicsContext;
	class GraphicsSystem;
	class IWebInterface;
	class InputSystem;
	class MainWindow;
	class ResourceManager;
	class Timer;
	class WindowEventBuffer;
	struct GameApplicationkData;
	struct InputEvent;
	struct WindowEvent;

	namespace framework
	{
		GameApplication& getGame();
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
		TIKI_NONCOPYABLE_CLASS( GameApplication );

	public:

		int								run();

		MainWindow&						getMainWindow() const;
		GraphicsSystem&					getGraphicsSystem() const;
		ResourceManager&				getResourceManager() const;
		InputSystem&					getIputSystem() const;
		const Timer&					getFrameTimer() const;
		const WindowEventBuffer&		getWindowEventBufer() const;
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
		GameApplicationkData*		m_pApplicationData;
	
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

#endif // TIKI_GAMEAPPLICATION_HPP_INCLUDED
