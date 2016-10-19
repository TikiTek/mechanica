#include "tiki/framework/baseapplication.hpp"

#include "tiki/base/platform.hpp"
#include "tiki/base/timer.hpp"
#include "tiki/debugrenderer/debugrenderer.hpp"
#include "tiki/framework/frameworkfactories.hpp"
#include "tiki/framework/mainwindow.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/input/inputsystem.hpp"
#include "tiki/io/gamebuildfilesystem.hpp"
#include "tiki/resource/resourcemanager.hpp"
#include "tiki/threading/thread.hpp"
#include "tiki/ui/uisystem.hpp"

namespace tiki
{
	struct BaseApplicationkData
	{
		MainWindow			mainWindow;
		GraphicsSystem		graphicSystem;
		InputSystem			inputSystem;

		GamebuildFileSystem	gamebuildFileSystem;

		ResourceManager		resourceManager;
		FrameworkFactories	resourceFactories;

		ImmediateRenderer	immediateRenderer;
		UiSystem			uiSystem;

		Timer				frameTimer;

	};

	BaseApplication::BaseApplication()
	{
		m_pBaseData = nullptr;
	}

	BaseApplication::~BaseApplication()
	{
		TIKI_ASSERT( m_pBaseData == nullptr );
	}

	int BaseApplication::run()
	{
		int resultValue = 0;

		if( initialize() )
		{
			m_running = true;

			// HACK: to handle device connect events
			InputEvent inputEvent;
			while( m_pBaseData->inputSystem.popEvent( inputEvent ) )
			{
				processBaseInputEvent( inputEvent );
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

	MainWindow& BaseApplication::getMainWindow() const
	{
		return m_pBaseData->mainWindow;
	}

	GraphicsSystem& BaseApplication::getGraphicsSystem() const
	{
		return m_pBaseData->graphicSystem;
	}

	ResourceManager& BaseApplication::getResourceManager() const
	{
		return m_pBaseData->resourceManager;
	}

	InputSystem& BaseApplication::getInputSystem() const
	{
		return m_pBaseData->inputSystem;
	}

	UiSystem& BaseApplication::getUiSystem() const
	{
		return m_pBaseData->uiSystem;
	}

	const Timer& BaseApplication::getFrameTimer() const
	{
		return m_pBaseData->frameTimer;
	}

	const ImmediateRenderer& BaseApplication::getImmediateRenderer() const
	{
		return m_pBaseData->immediateRenderer;
	}

	bool BaseApplication::initialize()
	{
		m_pBaseData = TIKI_NEW( BaseApplicationkData );

		if( !initializePlatform() )
		{
			return false;
		}

		if( !initializeFramework() )
		{
			return false;
		}

		if( !initializeApplication() )
		{
			return false;
		}

		m_isInitialized = true;

		return true;
	}

	void BaseApplication::shutdown()
	{
		shutdownApplication();
		shutdownFramework();
		shutdownPlatform();

		TIKI_MEMORY_DELETE_OBJECT( m_pBaseData );
		m_pBaseData = nullptr;

		m_isInitialized = false;
	}

	bool BaseApplication::initializeFramework()
	{
		fillBaseParameters( m_parameters );

		WindowParameters windowParams;
#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
		windowParams.instanceHandle	= platform::getInstanceHandle();
#endif
		windowParams.width			= m_parameters.screenWidth;
		windowParams.height			= m_parameters.screenHeight;
		windowParams.pWindowTitle	= m_parameters.pWindowTitle;

		if( !m_pBaseData->mainWindow.create( windowParams ) )
		{
			TIKI_TRACE_ERROR( "[BaseApplication] Could not create MainWindow.\n" );
			return false;
		}

		GraphicsSystemParameters graphicsParams;
		graphicsParams.fullScreen		= m_parameters.fullScreen;
		graphicsParams.pWindowHandle	= m_pBaseData->mainWindow.getHandle();
		graphicsParams.rendererMode		= m_parameters.graphicsMode;

		const uint2 windowSize			= m_pBaseData->mainWindow.getClientSize();
		graphicsParams.backBufferWidth	= TIKI_MAX( windowSize.x, 640u );
		graphicsParams.backBufferHeight	= TIKI_MAX( windowSize.y, 480u );

		if( !m_pBaseData->graphicSystem.create( graphicsParams ) )
		{
			TIKI_TRACE_ERROR( "[BaseApplication] Could not create GraphicsSystem.\n" );
			return false;
		}

		if( !m_pBaseData->gamebuildFileSystem.create( m_parameters.pGamebuildPath ) )
		{
			return false;
		}

		ResourceManagerParameters resourceParams;
		resourceParams.pFileSystem = &m_pBaseData->gamebuildFileSystem;

		if( !m_pBaseData->resourceManager.create( resourceParams ) )
		{
			TIKI_TRACE_ERROR( "[BaseApplication] Could not create ResourceManager.\n" );
			return false;
		}

		m_pBaseData->resourceFactories.create( m_pBaseData->resourceManager, m_pBaseData->graphicSystem );

		InputSystemParameters inputParams;
		inputParams.windowHandle	= m_pBaseData->mainWindow.getHandle();
		inputParams.instanceHandle	= platform::getInstanceHandle();

		if( !m_pBaseData->inputSystem.create( inputParams ) )
		{
			TIKI_TRACE_ERROR( "[BaseApplication] Could not create InputSystem.\n" );
			return false;
		}

		if( !m_pBaseData->immediateRenderer.create( m_pBaseData->graphicSystem, m_pBaseData->resourceManager ) )
		{
			TIKI_TRACE_ERROR( "[BaseApplication] Could not create ImmediateRenderer.\n" );
			return false;
		}
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		debugrenderer::initialize( m_pBaseData->resourceManager );
#endif

		UiSystemParameters uiParams;
		uiParams.width	= graphicsParams.backBufferWidth;
		uiParams.height	= graphicsParams.backBufferHeight;
		
		if( !m_pBaseData->uiSystem.create( m_pBaseData->graphicSystem, m_pBaseData->resourceManager, uiParams ) )
		{
			TIKI_TRACE_ERROR( "[BaseApplication] Could not create UiSystem.\n" );
			return false;
		}

		m_pBaseData->frameTimer.create();

		return true;
	}

	void BaseApplication::shutdownFramework()
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		debugrenderer::shutdown( m_pBaseData->resourceManager );
#endif

		m_pBaseData->immediateRenderer.dispose( m_pBaseData->graphicSystem, m_pBaseData->resourceManager );
		m_pBaseData->uiSystem.dispose( m_pBaseData->graphicSystem, m_pBaseData->resourceManager );
		m_pBaseData->inputSystem.dispose();
		m_pBaseData->graphicSystem.dispose();
		m_pBaseData->resourceFactories.dispose( m_pBaseData->resourceManager );
		m_pBaseData->resourceManager.dispose();
		m_pBaseData->gamebuildFileSystem.dispose();
		m_pBaseData->mainWindow.dispose();
	}

	bool BaseApplication::frame()
	{
		bool wantToShutdown = false;

		m_pBaseData->frameTimer.update();
		m_pBaseData->mainWindow.update();
		m_pBaseData->resourceManager.update();
		m_pBaseData->inputSystem.update();
		m_pBaseData->uiSystem.update();

		updatePlatform();

		WindowEventBuffer& windowEventBuffer = m_pBaseData->mainWindow.getEventBuffer();

		WindowEvent windowEvent;
		while ( windowEventBuffer.popEvent( windowEvent ) )
		{
			if( windowEvent.type == WindowEventType_Destroy )
			{
				wantToShutdown = true;
			}
			else if( windowEvent.type == WindowEventType_SizeChanged )
			{
				m_pBaseData->uiSystem.setScreenSize( (float)windowEvent.data.sizeChanged.size.x, (float)windowEvent.data.sizeChanged.size.y );

				if( !m_pBaseData->graphicSystem.resize( windowEvent.data.sizeChanged.size.x, windowEvent.data.sizeChanged.size.y ) )
				{
					return false;
				}
			}

			processBaseWindowEvent( windowEvent );
		}

		InputEvent inputEvent;
		while( m_pBaseData->inputSystem.popEvent( inputEvent ) )
		{
			if( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_Escape )
			{
				wantToShutdown = true;
			}

			if( !m_pBaseData->uiSystem.processInputEvent( inputEvent ) )
			{
				if( !processBaseInputEvent( inputEvent ) )
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
		}

		updateApplication( wantToShutdown );

		// render
		if ( m_running )
		{
			GraphicsContext& graphicsContext = m_pBaseData->graphicSystem.beginFrame();
			m_pBaseData->immediateRenderer.beginRendering( graphicsContext );

			renderApplication( graphicsContext );

			m_pBaseData->uiSystem.render( graphicsContext, graphicsContext.getBackBuffer() );

			m_pBaseData->immediateRenderer.endRendering();
			m_pBaseData->graphicSystem.endFrame();
		}

		m_pBaseData->inputSystem.endFrame();

		return m_running;
	}
}