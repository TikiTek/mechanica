
#include "tiki/framework/gameframework.hpp"

#include "tiki/threading/thread.hpp"

namespace tiki
{
	FrameworkData& framework::getFrameworkData()
	{
		return framework::getGame().m_frameworkData;
	}

	GameFramework::GameFramework()
	{
#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		m_frameworkData.pWebInterface	= nullptr;
#endif

		m_isInitialized					= false;
	}

	GameFramework::~GameFramework()
	{
	}

	int GameFramework::run()
	{
		int resultValue = 0;

		if ( internInitialize() )
		{
			// HACK: to handle device connect events
			InputEvent inputEvent;
			while ( m_frameworkData.inputSystem.popEvent( inputEvent ) )
			{
				processInputEvent( inputEvent );
			}

			while ( frame() ) { }
		}
		else
		{
			resultValue = -1;
		}

		internShutdown();

		Thread::shutdownSystem();

		return resultValue;
	}

	bool GameFramework::internInitialize()
	{
		fillParameters( m_parameters );

		WindowParameters windowParams;
		windowParams.width			= m_parameters.screenWidth;
		windowParams.height			= m_parameters.screenHeight;
		windowParams.pWindowTitle	= m_parameters.pWindowTitle;
		windowParams.pClassName		= "TikiEngineMainWindow";
		
		if ( !m_frameworkData.mainWindow.create( windowParams ) )
		{
			TIKI_TRACE_ERROR( "[gameframework] Could not create MainWindow.\n" );
			return false;
		}
		
		GraphicsSystemParameters graphicParams;
		graphicParams.fullScreen		= m_parameters.fullScreen;
		graphicParams.pWindowHandle		= m_frameworkData.mainWindow.getHandle();
		graphicParams.rendererMode		= m_parameters.graphicsMode;

		const uint2 windowSize			= m_frameworkData.mainWindow.getClientSize();
		graphicParams.backBufferWidth	= TIKI_MAX( windowSize.x, 640u );
		graphicParams.backBufferHeight	= TIKI_MAX( windowSize.y, 480u );

		if ( !m_frameworkData.graphicSystem.create( graphicParams ) )
		{
			TIKI_TRACE_ERROR( "[gameframework] Could not create GraphicsSystem.\n" );
			return false;
		}

		m_frameworkData.gamebuildFileSystem.create( m_parameters.pGamebuildPath );

		ResourceManagerParameters resourceParams;
		resourceParams.pFileSystem = &m_frameworkData.gamebuildFileSystem;

		if ( !m_frameworkData.resourceManager.create( resourceParams ) )
		{
			TIKI_TRACE_ERROR( "[gameframework] Could not create ResourceManager.\n" );
			return false;
		}

		InputSystemParameters inputParams;
		inputParams.windowHandle	= m_frameworkData.mainWindow.getHandle();
		inputParams.instanceHandle	= getInstanceHandle();

		if( !m_frameworkData.inputSystem.create( inputParams ) )
		{
			TIKI_TRACE_ERROR( "[gameframework] Could not create InputSystem.\n" );
			return false;
		}

		m_frameworkData.frameTimer.create();

		if ( !initialize() )
		{
			return false;
		}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		if ( !m_frameworkData.debugGui.create( m_frameworkData.graphicSystem, m_frameworkData.resourceManager ) )
		{
			return false;
		}

		m_frameworkData.debugGuiWindows.create( m_frameworkData.debugGui );
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		m_frameworkData.pWebInterface = createWebInterface();
		if ( m_frameworkData.pWebInterface == nullptr || !m_frameworkData.pWebInterface->create() )
		{
			return false;
		}
#endif

		m_isInitialized = true;
		return true;
	}

	void GameFramework::internShutdown()
	{
		if ( m_isInitialized )
		{
			shutdown();
		}
		m_isInitialized = false;

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		if ( m_frameworkData.pWebInterface != nullptr )
		{
			m_frameworkData.pWebInterface->dispose();

			disposeWebInterface( m_frameworkData.pWebInterface );
			m_frameworkData.pWebInterface = nullptr;
		}	
#endif

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_frameworkData.debugGuiWindows.dispose();
		m_frameworkData.debugGui.dispose( m_frameworkData.graphicSystem, m_frameworkData.resourceManager );
#endif
		m_frameworkData.inputSystem.dispose();
		m_frameworkData.graphicSystem.dispose();
		m_frameworkData.resourceManager.dispose();
		m_frameworkData.mainWindow.dispose();

		m_frameworkData.gamebuildFileSystem.dispose();
	}

	bool GameFramework::frame()
	{
		m_frameworkData.frameTimer.update();
		m_frameworkData.mainWindow.update();
		m_frameworkData.resourceManager.update();
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_frameworkData.debugGui.update();
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		m_frameworkData.pWebInterface->update();
#endif

		const WindowEvent* pDestroyEvent = m_frameworkData.mainWindow.getEventBuffer().getEventByType( WindowEventType_Destroy );
		if ( pDestroyEvent != nullptr )
		{
			return false;
		}

		m_frameworkData.inputSystem.update( m_frameworkData.mainWindow.getEventBuffer() );

		InputEvent inputEvent;
		while ( m_frameworkData.inputSystem.popEvent( inputEvent ) )
		{
			if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_Escape )
			{
				return false;
			}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
			if ( m_frameworkData.debugGui.processInputEvent( inputEvent ) )
			{
				continue;
			}
#endif

			if ( !processInputEvent( inputEvent ) )
			{
				//if ( inputEvent.deviceType == InputDeviceType_Keyboard )
				//{
				//	const char* pState = ( inputEvent.eventType == InputEventType_Keyboard_Up ? "released" : "pressed" );
				//	TIKI_TRACE_INFO( "[framework] keyboard event not handled. %s has %s.\n", input::getKeyboardKeyName( inputEvent.data.keybaordKey.key ), pState );
				//}
				//else
				//{
				//	TIKI_TRACE_INFO( "[framework] InputEvent of type %u not handled by any state.\n", inputEvent.eventType );
				//}
			}
		}

		update();

		// render
		{
			GraphicsContext& graphicsContext = m_frameworkData.graphicSystem.beginFrame();

			render( graphicsContext );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
			m_frameworkData.debugGui.render( graphicsContext );
#endif

			m_frameworkData.graphicSystem.endFrame();
		}

		return true;
	}
}