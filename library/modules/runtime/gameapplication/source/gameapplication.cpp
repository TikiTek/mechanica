
#include "tiki/gameapplication/gameapplication.hpp"

#include "tiki/threading/thread.hpp"

#include "gameapplicationdata.hpp"

#if TIKI_ENABLED( TIKI_SDL )
#	include "SDL.h"
#endif

namespace tiki
{
	GameApplication::GameApplication()
	{
		m_pApplicationData	= nullptr;

		m_running			= false;
		m_isInitialized		= false;
	}

	GameApplication::~GameApplication()
	{
		TIKI_ASSERT( m_pApplicationData == nullptr );
	}

	int GameApplication::run()
	{
		int resultValue = 0;

		if( initialize() )
		{
			m_running = true;

			// HACK: to handle device connect events
			InputEvent inputEvent;
			while( m_pApplicationData->inputSystem.popEvent( inputEvent ) )
			{
				processInputEvent( inputEvent );
			}

			while( frame() );
		}
		else
		{
			resultValue = -1;
		}

		shutdown();

		Thread::shutdownSystem();

		return resultValue;
	}

	MainWindow& GameApplication::getMainWindow() const
	{
		return m_pApplicationData->mainWindow;
	}

	GraphicsSystem& GameApplication::getGraphicsSystem() const
	{
		return m_pApplicationData->graphicSystem;
	}

	ResourceManager& GameApplication::getResourceManager() const
	{
		return m_pApplicationData->resourceManager;
	}

	InputSystem& GameApplication::getIputSystem() const
	{
		return m_pApplicationData->inputSystem;
	}

	const Timer& GameApplication::getFrameTimer() const
	{
		return m_pApplicationData->frameTimer;
	}

	const WindowEventBuffer& GameApplication::getWindowEventBufer() const
	{
		return m_pApplicationData->mainWindow.getEventBuffer();
	}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	DebugGui& GameApplication::getDebugGui() const
	{
		return m_pApplicationData->debugGui;
	}
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
	IWebInterface* GameApplication::getWebInterface() const
	{
		return m_pApplicationData->pWebInterface;
	}
#endif

	bool GameApplication::initialize()
	{
		m_pApplicationData = TIKI_MEMORY_NEW_OBJECT( GameApplicationkData );

		if ( !initializePlatform() )
		{
			return false;
		}

		if ( !initializeFramework() )
		{
			return false;
		}

		if ( !initializeGame() )
		{
			return false;
		}

		if ( !initializeDebugSystems() )
		{
			return false;
		}

		m_isInitialized = true;

		return true;
	}

	void GameApplication::shutdown()
	{
		shutdownDebugSystems();
		shutdownGame();
		shutdownFramework();
		shutdownPlatform();

		TIKI_MEMORY_DELETE_OBJECT(m_pApplicationData);
		m_pApplicationData = nullptr;

		m_isInitialized = false;
	}

	bool GameApplication::initializeFramework()
	{
		fillParameters( m_parameters );

		WindowParameters windowParams;
		windowParams.instanceHandle	= platform::getInstanceHandle();
		windowParams.width			= m_parameters.screenWidth;
		windowParams.height			= m_parameters.screenHeight;
		windowParams.pWindowTitle	= m_parameters.pWindowTitle;
		
		if ( !m_pApplicationData->mainWindow.create( windowParams ) )
		{
			TIKI_TRACE_ERROR( "[GameApplication] Could not create MainWindow.\n" );
			return false;
		}

		GraphicsSystemParameters graphicParams;
		graphicParams.fullScreen		= m_parameters.fullScreen;
		graphicParams.pWindowHandle		= m_pApplicationData->mainWindow.getHandle();
		graphicParams.rendererMode		= m_parameters.graphicsMode;

		const uint2 windowSize			= m_pApplicationData->mainWindow.getClientSize();
		graphicParams.backBufferWidth	= TIKI_MAX( windowSize.x, 640u );
		graphicParams.backBufferHeight	= TIKI_MAX( windowSize.y, 480u );

		if ( !m_pApplicationData->graphicSystem.create( graphicParams ) )
		{
			TIKI_TRACE_ERROR( "[GameApplication] Could not create GraphicsSystem.\n" );
			return false;
		}

		if ( !m_pApplicationData->gamebuildFileSystem.create( m_parameters.pGamebuildPath ) )
		{
			return false;
		}

		ResourceManagerParameters resourceParams;
		resourceParams.pFileSystem = &m_pApplicationData->gamebuildFileSystem;

		if ( !m_pApplicationData->resourceManager.create( resourceParams ) )
		{
			TIKI_TRACE_ERROR( "[GameApplication] Could not create ResourceManager.\n" );
			return false;
		}

		InputSystemParameters inputParams;
		inputParams.windowHandle	= m_pApplicationData->mainWindow.getHandle();
		inputParams.instanceHandle	= platform::getInstanceHandle();

		if( !m_pApplicationData->inputSystem.create( inputParams ) )
		{
			TIKI_TRACE_ERROR( "[GameApplication] Could not create InputSystem.\n" );
			return false;
		}

		m_pApplicationData->frameTimer.create();

		return true;
	}

	void GameApplication::shutdownFramework()
	{
		m_pApplicationData->inputSystem.dispose();
		m_pApplicationData->graphicSystem.dispose();
		m_pApplicationData->resourceManager.dispose();
		m_pApplicationData->mainWindow.dispose();

		m_pApplicationData->gamebuildFileSystem.dispose();
	}

	bool GameApplication::initializeDebugSystems()
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		if ( !m_pApplicationData->debugGui.create( m_pApplicationData->graphicSystem, m_pApplicationData->resourceManager ) )
		{
			return false;
		}

		m_pApplicationData->debugGuiWindows.create( m_pApplicationData->debugGui );
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		m_pApplicationData->pWebInterface = createWebInterface();
		if ( m_pApplicationData->pWebInterface == nullptr || !m_pApplicationData->pWebInterface->create() )
		{
			return false;
		}
#endif

		return true;
	}

	void GameApplication::shutdownDebugSystems()
	{
#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		if ( m_pApplicationData->pWebInterface != nullptr )
		{
			m_pApplicationData->pWebInterface->dispose();

			disposeWebInterface( m_pApplicationData->pWebInterface );
			m_pApplicationData->pWebInterface = nullptr;
		}
#endif

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_pApplicationData->debugGuiWindows.dispose();
		m_pApplicationData->debugGui.dispose( m_pApplicationData->graphicSystem, m_pApplicationData->resourceManager );
#endif
	}

	bool GameApplication::frame()
	{
		m_pApplicationData->frameTimer.update();
		m_pApplicationData->mainWindow.update();
		m_pApplicationData->resourceManager.update();
		m_pApplicationData->inputSystem.update();

		updatePlatform();

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_pApplicationData->debugGui.update( getFrameTimer().getElapsedTime() );
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		m_pApplicationData->pWebInterface->update();
#endif
		bool wantToShutdown = false;
		const uint windowEventCount = m_pApplicationData->mainWindow.getEventBuffer().getEventCount();
		for (uint i = 0u; i < windowEventCount; ++i)
		{
			const WindowEvent& windowEvent = m_pApplicationData->mainWindow.getEventBuffer().getEventByIndex( i );

			if ( windowEvent.type == WindowEventType_Destroy )
			{
				wantToShutdown = true;
			}
			else if ( windowEvent.type == WindowEventType_SizeChanged )
			{
				if ( !m_pApplicationData->graphicSystem.resize( windowEvent.data.sizeChanged.size.x, windowEvent.data.sizeChanged.size.y ) )
				{
					return false;
				}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
				m_pApplicationData->debugGui.setScreenSize( vector::create( (float)windowEvent.data.sizeChanged.size.x, (float)windowEvent.data.sizeChanged.size.y ) );
#endif
			}

			processWindowEvent( windowEvent );
		}

		InputEvent inputEvent;
		while ( m_pApplicationData->inputSystem.popEvent( inputEvent ) )
		{
			if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_Escape )
			{
				wantToShutdown = true;
			}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
			if ( m_pApplicationData->debugGui.processInputEvent( inputEvent ) )
			{
				continue;
			}
#endif

			if ( !processInputEvent( inputEvent ) )
			{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
				if ( inputEvent.eventType == InputEventType_Keyboard_Down )
				{
					switch ( inputEvent.data.keybaordKey.key )
					{
					case KeyboardKey_F1:
						m_pApplicationData->debugGui.setActive( !m_pApplicationData->debugGui.getActive() );
						break;
					}
				}
#endif

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

		update( wantToShutdown );

		// render
		{
			GraphicsContext& graphicsContext = m_pApplicationData->graphicSystem.beginFrame();

			render( graphicsContext );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
			m_pApplicationData->debugGui.render( graphicsContext );
#endif

			m_pApplicationData->graphicSystem.endFrame();
		}

		m_pApplicationData->inputSystem.endFrame();

		return m_running;
	}
}
