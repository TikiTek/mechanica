
#include "tiki/framework/gameapplication.hpp"

#include "tiki/threading/thread.hpp"

#include "frameworkdata.hpp"

#if TIKI_ENABLED( TIKI_SDL )
#	include "SDL.h"
#endif

namespace tiki
{
	FrameworkData& framework::getFrameworkData()
	{
		return *framework::getGame().m_pFrameworkData;
	}

	GameApplication::GameApplication()
	{
		m_pFrameworkData	= nullptr;

		m_running			= false;
		m_isInitialized		= false;
	}

	GameApplication::~GameApplication()
	{
		TIKI_ASSERT( m_pFrameworkData == nullptr );
	}

	int GameApplication::run()
	{
		int resultValue = 0;

		if ( initialize() )
		{
			m_running = true;

			// HACK: to handle device connect events
			InputEvent inputEvent;
			while ( m_pFrameworkData->inputSystem.popEvent( inputEvent ) )
			{
				processInputEvent( inputEvent );
			}

			while ( frame() );
		}
		else
		{
			resultValue = -1;
		}

		shutdown();

		Thread::shutdownSystem();

		return resultValue;
	}

	bool GameApplication::initialize()
	{
		m_pFrameworkData = TIKI_MEMORY_NEW_OBJECT( FrameworkData );

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

		TIKI_MEMORY_DELETE_OBJECT( m_pFrameworkData );
		m_pFrameworkData = nullptr;

		m_isInitialized = false;
	}

	bool GameApplication::initializeFramework()
	{
		fillParameters( m_parameters );

		WindowParameters windowParams;
		windowParams.width			= m_parameters.screenWidth;
		windowParams.height			= m_parameters.screenHeight;
		windowParams.pWindowTitle	= m_parameters.pWindowTitle;
		windowParams.pClassName		= "TikiEngineMainWindow";
		
		if ( !m_pFrameworkData->mainWindow.create( windowParams ) )
		{
			TIKI_TRACE_ERROR( "[GameApplication] Could not create MainWindow.\n" );
			return false;
		}

		GraphicsSystemParameters graphicParams;
		graphicParams.fullScreen		= m_parameters.fullScreen;
		graphicParams.pWindowHandle		= m_pFrameworkData->mainWindow.getHandle();
		graphicParams.rendererMode		= m_parameters.graphicsMode;

		const uint2 windowSize			= m_pFrameworkData->mainWindow.getClientSize();
		graphicParams.backBufferWidth	= TIKI_MAX( windowSize.x, 640u );
		graphicParams.backBufferHeight	= TIKI_MAX( windowSize.y, 480u );

		if ( !m_pFrameworkData->graphicSystem.create( graphicParams ) )
		{
			TIKI_TRACE_ERROR( "[GameApplication] Could not create GraphicsSystem.\n" );
			return false;
		}

		if ( !m_pFrameworkData->gamebuildFileSystem.create( m_parameters.pGamebuildPath ) )
		{
			return false;
		}

		ResourceManagerParameters resourceParams;
		resourceParams.pFileSystem = &m_pFrameworkData->gamebuildFileSystem;

		if ( !m_pFrameworkData->resourceManager.create( resourceParams ) )
		{
			TIKI_TRACE_ERROR( "[GameApplication] Could not create ResourceManager.\n" );
			return false;
		}

		InputSystemParameters inputParams;
		inputParams.windowHandle	= m_pFrameworkData->mainWindow.getHandle();
		inputParams.instanceHandle	= platform::getInstanceHandle();

		if( !m_pFrameworkData->inputSystem.create( inputParams ) )
		{
			TIKI_TRACE_ERROR( "[GameApplication] Could not create InputSystem.\n" );
			return false;
		}

		m_pFrameworkData->frameTimer.create();

		return true;
	}

	void GameApplication::shutdownFramework()
	{
		m_pFrameworkData->inputSystem.dispose();
		m_pFrameworkData->graphicSystem.dispose();
		m_pFrameworkData->resourceManager.dispose();
		m_pFrameworkData->mainWindow.dispose();

		m_pFrameworkData->gamebuildFileSystem.dispose();
	}

	bool GameApplication::initializeDebugSystems()
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		if ( !m_pFrameworkData->debugGui.create( m_pFrameworkData->graphicSystem, m_pFrameworkData->resourceManager ) )
		{
			return false;
		}

		m_pFrameworkData->debugGuiWindows.create( m_pFrameworkData->debugGui );
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		m_pFrameworkData->pWebInterface = createWebInterface();
		if ( m_pFrameworkData->pWebInterface == nullptr || !m_pFrameworkData->pWebInterface->create() )
		{
			return false;
		}
#endif

		return true;
	}

	void GameApplication::shutdownDebugSystems()
	{
#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		if ( m_pFrameworkData->pWebInterface != nullptr )
		{
			m_pFrameworkData->pWebInterface->dispose();

			disposeWebInterface( m_pFrameworkData->pWebInterface );
			m_pFrameworkData->pWebInterface = nullptr;
		}
#endif

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_pFrameworkData->debugGuiWindows.dispose();
		m_pFrameworkData->debugGui.dispose( m_pFrameworkData->graphicSystem, m_pFrameworkData->resourceManager );
#endif
	}

	bool GameApplication::frame()
	{
		m_pFrameworkData->frameTimer.update();
		m_pFrameworkData->mainWindow.update();
		m_pFrameworkData->resourceManager.update();
		m_pFrameworkData->inputSystem.update();

		updatePlatform();

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_pFrameworkData->debugGui.update();
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		m_pFrameworkData->pWebInterface->update();
#endif
		bool wantToShutdown = false;
		const uint windowEventCount = m_pFrameworkData->mainWindow.getEventBuffer().getEventCount();
		for (uint i = 0u; i < windowEventCount; ++i)
		{
			const WindowEvent& windowEvent = m_pFrameworkData->mainWindow.getEventBuffer().getEventByIndex( i );

			if ( windowEvent.type == WindowEventType_Destroy )
			{
				wantToShutdown = true;
			}
			else if ( windowEvent.type == WindowEventType_SizeChanged )
			{
				if ( !m_pFrameworkData->graphicSystem.resize( windowEvent.data.sizeChanged.size.x, windowEvent.data.sizeChanged.size.y ) )
				{
					return false;
				}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
				m_pFrameworkData->debugGui.setScreenSize( vector::create( (float)windowEvent.data.sizeChanged.size.x, (float)windowEvent.data.sizeChanged.size.y ) );
#endif
			}

			processWindowEvent( windowEvent );
		}

		InputEvent inputEvent;
		while ( m_pFrameworkData->inputSystem.popEvent( inputEvent ) )
		{
			if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_Escape )
			{
				wantToShutdown = true;
			}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
			if ( m_pFrameworkData->debugGui.processInputEvent( inputEvent ) )
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
						m_pFrameworkData->debugGui.setActive( !m_pFrameworkData->debugGui.getActive() );
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
			GraphicsContext& graphicsContext = m_pFrameworkData->graphicSystem.beginFrame();

			render( graphicsContext );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
			m_pFrameworkData->debugGui.render( graphicsContext );
#endif

			m_pFrameworkData->graphicSystem.endFrame();
		}

		m_pFrameworkData->inputSystem.endFrame();

		return m_running;
	}
}
