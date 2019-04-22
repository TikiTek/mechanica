#pragma once

#include "tiki/base/path.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/pixel_format.hpp"

namespace tiki
{
	class GraphicsContext;
	class GraphicsSystem;
	class ImmediateRenderer;
	class InputSystem;
	class MainWindow;
	class ResourceManager;
	class Timer;
	struct BaseApplicationkData;
	struct InputEvent;
	struct WindowEvent;
#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
	class Project;
#endif

	struct BaseApplicationParamters
	{
		BaseApplicationParamters()
		{
			screenWidth			= 900;
			screenHeight		= 600;
			backBufferFormat	= PixelFormat_R8G8B8A8;
			fullScreen			= false;

			pWindowTitle		= "TikiEngine 3.0";
		}

		uint					screenWidth;
		uint					screenHeight;
		PixelFormat				backBufferFormat;
		bool					fullScreen;

		const char*				pWindowTitle;

		Path					assetBuildPath;
#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		const Project*			pProject = nullptr;
#endif
	};

	class BaseApplication
	{
		TIKI_NONCOPYABLE_CLASS( BaseApplication );

	public:

		int								run();

		MainWindow&						getMainWindow() const;
		GraphicsSystem&					getGraphicsSystem() const;
		ResourceManager&				getResourceManager() const;
		InputSystem&					getInputSystem() const;
		const Timer&					getFrameTimer() const;
		const ImmediateRenderer&		getImmediateRenderer() const;

	protected:

										BaseApplication();
		virtual							~BaseApplication();

		virtual void					fillBaseParameters( BaseApplicationParamters& parameters ) TIKI_PURE;

		virtual bool					initializeApplication() TIKI_PURE;
		virtual void					shutdownApplication() TIKI_PURE;

		virtual void					updateApplication( bool wantToShutdown ) TIKI_PURE;
		virtual void					renderApplication( GraphicsContext& graphicsContext ) const TIKI_PURE;

		virtual bool					processBaseInputEvent( const InputEvent& inputEvent ) TIKI_PURE;
		virtual void					processBaseWindowEvent( const WindowEvent& windowEvent ) TIKI_PURE;

		bool							m_running;

	private:

		bool							m_isInitialized;

		BaseApplicationParamters		m_parameters;
		BaseApplicationkData*			m_pBaseData;

		bool							initialize();
		void							shutdown();

		bool							initializePlatform();
		void							shutdownPlatform();
		void							updatePlatform();

		bool							initializeFramework();
		void							shutdownFramework();

		bool							frame();
	};
}
