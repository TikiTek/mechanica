#include "tiki/framework/base_application.hpp"

#include "tiki/base/platform.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/base/timer.hpp"
#include "tiki/debug_renderer/debug_renderer.hpp"
#include "tiki/framework/framework_factories.hpp"
#include "tiki/framework/main_window.hpp"
#include "tiki/graphics/graphics_system.hpp"
#include "tiki/graphics/immediate_renderer.hpp"
#include "tiki/input/input_system.hpp"
#include "tiki/io/game_build_file_system.hpp"
#include "tiki/resource/resource_manager.hpp"
#include "tiki/threading/thread.hpp"
#include "tiki/tool_xml/xml_document.hpp"
#include "tiki/tool_xml/xml_element.hpp"

#include "generic_data_factories.hpp"

namespace tiki
{
	struct BaseApplicationkData
	{
		MainWindow				mainWindow;
		GraphicsSystem			graphicSystem;
		InputSystem				inputSystem;

		GamebuildFileSystem		gamebuildFileSystem;

		ResourceManager			resourceManager;
		FrameworkFactories		resourceFactories;
		GenericDataFactories	genericDataFactories;

		ImmediateRenderer		immediateRenderer;

		Timer					frameTimer;
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

		TIKI_DELETE( m_pBaseData );
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

		loadWindowParameters( windowParams );

		if( !m_pBaseData->mainWindow.create( windowParams ) )
		{
			TIKI_TRACE_ERROR( "[BaseApplication] Could not create MainWindow.\n" );
			return false;
		}

		const uint2 windowSize			= m_pBaseData->mainWindow.getClientSize();

		GraphicsSystemParameters graphicsParams;
		graphicsParams.fullScreen		= m_parameters.fullScreen;
		graphicsParams.pWindowHandle	= m_pBaseData->mainWindow.getHandle();
		graphicsParams.backBufferWidth	= TIKI_MAX( windowSize.x, 16u );
		graphicsParams.backBufferHeight	= TIKI_MAX( windowSize.y, 16u );
		graphicsParams.backBufferFormat	= m_parameters.backBufferFormat;

		if( !m_pBaseData->graphicSystem.create( graphicsParams ) )
		{
			TIKI_TRACE_ERROR( "[BaseApplication] Could not create GraphicsSystem.\n" );
			return false;
		}

		if( !m_pBaseData->gamebuildFileSystem.create( m_parameters.assetBuildPath ) )
		{
			return false;
		}

		ResourceManagerParameters resourceParams;
		resourceParams.pFileSystem	= &m_pBaseData->gamebuildFileSystem;
#if TIKI_ENABLED( TIKI_RESOUCE_ENABLE_CONVERTER )
		resourceParams.pProject		= m_parameters.pProject;
#endif

		if( !m_pBaseData->resourceManager.create( resourceParams ) )
		{
			TIKI_TRACE_ERROR( "[BaseApplication] Could not create ResourceManager.\n" );
			return false;
		}

		m_pBaseData->resourceFactories.create( m_pBaseData->resourceManager, m_pBaseData->graphicSystem );
		m_pBaseData->genericDataFactories.create( m_pBaseData->resourceManager );

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

		m_pBaseData->frameTimer.create();

		return true;
	}

	void BaseApplication::shutdownFramework()
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		debugrenderer::shutdown( m_pBaseData->resourceManager );
#endif

		saveWindowParameters();

		m_pBaseData->immediateRenderer.dispose( m_pBaseData->graphicSystem, m_pBaseData->resourceManager );
		m_pBaseData->inputSystem.dispose();
		m_pBaseData->graphicSystem.dispose();
		m_pBaseData->genericDataFactories.dispose( m_pBaseData->resourceManager );
		m_pBaseData->resourceFactories.dispose( m_pBaseData->resourceManager );
		m_pBaseData->resourceManager.dispose();
		m_pBaseData->gamebuildFileSystem.dispose();
		m_pBaseData->mainWindow.dispose();
	}

	void BaseApplication::loadWindowParameters( WindowParameters& parameters )
	{
		Path configPath = platform::getExecutablePath();
		configPath.push( "config.xml" );

		XmlDocument configDocument;
		if( !configDocument.loadFromFile( configPath.getCompletePath() ) )
		{
			return;
		}

		const XmlElement* pRootNode = configDocument.findFirstChild( "config" );
		if( pRootNode == nullptr )
		{
			return;
		}

		const XmlElement* pPositionXNode	= pRootNode->findFirstChild( "positionX" );
		const XmlElement* pPositionYNode	= pRootNode->findFirstChild( "positionY" );
		const XmlElement* pSizeXNode		= pRootNode->findFirstChild( "sizeX" );
		const XmlElement* pSizeYNode		= pRootNode->findFirstChild( "sizeY" );
		if( pPositionXNode == nullptr ||
			pPositionYNode == nullptr ||
			pSizeXNode == nullptr ||
			pSizeYNode == nullptr )
		{
			return;
		}

		sint16 positionX;
		sint16 positionY;
		uint16 sizeX;
		uint16 sizeY;
		if( !string_tools::tryParseSInt16( positionX, pPositionXNode->getValue() ) ||
			!string_tools::tryParseSInt16( positionY, pPositionYNode->getValue() ) ||
			!string_tools::tryParseUInt16( sizeX, pSizeXNode->getValue() ) ||
			!string_tools::tryParseUInt16( sizeY, pSizeYNode->getValue() ) )
		{
			return;
		}

		parameters.postionX	= positionX;
		parameters.postionY	= positionY;
		parameters.width	= sizeX;
		parameters.height	= sizeY;
	}

	void BaseApplication::saveWindowParameters()
	{
		Path configPath = platform::getExecutablePath();
		configPath.push( "config.xml" );

		XmlDocument configDocument;

		XmlElement* pRootNode = configDocument.createChild( "config" );

		const sint2 windowPosition	= m_pBaseData->mainWindow.getPosition();
		const uint2 windowSize		= m_pBaseData->mainWindow.getSize();
		pRootNode->createChild( "positionX" )->setValue( string_tools::toString( windowPosition.x ).cStr() );
		pRootNode->createChild( "positionY" )->setValue( string_tools::toString( windowPosition.y ).cStr() );
		pRootNode->createChild( "sizeX" )->setValue( string_tools::toString( windowSize.x ).cStr() );
		pRootNode->createChild( "sizeY" )->setValue( string_tools::toString( windowSize.y ).cStr() );

		configDocument.saveToFile( configPath.getCompletePath() );
	}

	bool BaseApplication::frame()
	{
		bool wantToShutdown = false;

		m_pBaseData->frameTimer.update();
		m_pBaseData->mainWindow.update();
		m_pBaseData->resourceManager.update();
		m_pBaseData->inputSystem.update();

		updatePlatform();

		WindowEventBuffer& windowEventBuffer = m_pBaseData->mainWindow.getEventBuffer();

		WindowEvent windowEvent;
		while ( windowEventBuffer.popEvent( windowEvent ) )
		{
			if( windowEvent.type == WindowEventType_Close )
			{
				wantToShutdown = true;
			}
			else if( windowEvent.type == WindowEventType_SizeChanged )
			{
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
			if( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keyboardKey.key == KeyboardKey_Escape )
			{
				wantToShutdown = true;
			}

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

		updateApplication( wantToShutdown );

		// render
		if ( m_running )
		{
			GraphicsContext& graphicsContext = m_pBaseData->graphicSystem.beginFrame();
			m_pBaseData->immediateRenderer.beginRendering( graphicsContext );

			renderApplication( graphicsContext );

			m_pBaseData->immediateRenderer.endRendering();
			m_pBaseData->graphicSystem.endFrame();
		}

		m_pBaseData->inputSystem.endFrame();

		return m_running;
	}
}