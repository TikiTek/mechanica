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
	struct WindowParameters;
#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
	class Project;
#endif

	struct BaseApplicationParamters
	{
		uint16					screenWidth			= 900u;
		uint16					screenHeight		= 600u;
		PixelFormat				backBufferFormat	= PixelFormat_R8G8B8A8;
		bool					fullScreen			= false;

		const char*				pWindowTitle		= "TikiEngine 3.0";
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
		int						windowIconId		= 0;
#endif

		Path					assetBuildPath;
#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		const Project*			pProject			= nullptr;
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

		virtual void					fillBaseParameters( BaseApplicationParamters& parameters ) = 0;

		virtual bool					initializeApplication() = 0;
		virtual void					shutdownApplication() = 0;

		virtual void					updateApplication( bool wantToShutdown ) = 0;
		virtual void					renderApplication( GraphicsContext& graphicsContext ) const = 0;

		virtual bool					processBaseInputEvent( const InputEvent& inputEvent ) = 0;
		virtual void					processBaseWindowEvent( const WindowEvent& windowEvent ) = 0;

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

		void							loadWindowParameters( WindowParameters& parameters );
		void							saveWindowParameters();

		bool							frame();
	};
}
