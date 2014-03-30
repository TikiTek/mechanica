
#include "tiki/framework/gameframework.hpp"

namespace tiki
{
	FrameworkData& framework::getFrameworkData()
	{
		return framework::getGame().m_frameworkData;
	}

	GameFramework::GameFramework()
	{
		m_isInitialized = false;
	}

	GameFramework::~GameFramework()
	{
	}

	int GameFramework::run()
	{
		int resultValue = 0;

		if ( internInitialize() )
		{
			while ( frame() ) { }
		}
		else
		{
			resultValue = -1;
		}

		internShutdown();

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
		graphicParams.backBufferWidth	= windowSize.x;
		graphicParams.backBufferHeight	= windowSize.y;

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
		inputParams.windowHandle = m_frameworkData.mainWindow.getHandle();

		if( !m_frameworkData.inputSystem.create( inputParams ) )
		{
			TIKI_TRACE_ERROR( "[gameframework] Could not create InputSystem.\n" );
			return false;
		}

		m_frameworkData.frameTimer.create();

		initialize();
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

		for (size_t i = 0u; i < m_frameworkData.mainWindow.getEventBuffer().getEventCount(); ++i)
		{
			const WindowEvent& event = m_frameworkData.mainWindow.getEventBuffer().getEventByIndex( i );

			switch (event.type)
			{
			case WindowEventType_Destroy:
				return false;
			default:
				break;
			}
		}

		m_frameworkData.inputSystem.update();

		InputEvent inputEvent;
		while ( m_frameworkData.inputSystem.popEvent( inputEvent ) )
		{
			if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_Escape )
			{
				return false;
			}

			if ( !processInputEvent( inputEvent ) )
			{
				if ( inputEvent.deviceType == InputDeviceType_Keyboard )
				{
					const char* pState = ( inputEvent.eventType == InputEventType_Keyboard_Up ? "released" : "pressed" );
					TIKI_TRACE_INFO( "[game] keyboard event not handled. %s has %s.\n", input::getKeyboardKeyName( inputEvent.data.keybaordKey.key ), pState );
				}
				else
				{
					//TIKI_TRACE_INFO( "[gameflow] InputEvent of type %u not handled by any state.\n", inputEvent.eventType );
				}
			}
		}

		update();

		GraphicsContext& graphicsContext = m_frameworkData.graphicSystem.beginFrame();
		render( graphicsContext );
		m_frameworkData.graphicSystem.endFrame();

		return true;
	}
}