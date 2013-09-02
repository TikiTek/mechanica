
#include "tiki/framework/gameframework.hpp"

namespace tiki
{
	FrameworkData& framework::getFrameworkData()
	{
		return getGameFramework().m_frameworkData;
	}

	GameFramework::GameFramework()
	{
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
		this->fillParameters( &m_parameters );

		WindowParameters windowParams;
		windowParams.width			= m_parameters.screenWidth;
		windowParams.height			= m_parameters.screenHeight;
		windowParams.pWindowTitle	= m_parameters.pTitle;
		windowParams.pClassName		= "TikiEngineMainWindow";
		
		if (!m_frameworkData.mainWindow.create( windowParams ))
			return false;
		
		GraphicsSystemParameters graphicParams;
		graphicParams.fullScreen		= m_parameters.fullScreen;
		graphicParams.pWindowHandle		= m_frameworkData.mainWindow.getHandle();
		graphicParams.rendererMode		= (m_parameters.useHardwareRenderer ? GraphicsRendererMode_Hardware : GraphicsRendererMode_Software);

		const uint2 windowSize			= m_frameworkData.mainWindow.getClientSize();
		graphicParams.backBufferWidth	= windowSize.x;
		graphicParams.backBufferHeight	= windowSize.y;

		if (!m_frameworkData.graphicSystem.create( graphicParams ))
			return false;

		ResourceManagerParameters resourceParams;
		resourceParams.pGamebuildPath	= m_parameters.pGamebuildPath;

		if ( !m_frameworkData.resourceManager.create( resourceParams ) )
			return false;

		InputSystemParameters inputParams;
		inputParams.pWindow = &m_frameworkData.mainWindow;

		if( !m_frameworkData.inputSystem.create( inputParams ) )
			return false;

		m_frameworkData.factories.create( m_frameworkData.resourceManager, m_frameworkData.graphicSystem );

		m_frameworkData.frameTimer.create();

		initialize();

		return true;
	}

	void GameFramework::internShutdown()
	{
		shutdown();

		m_frameworkData.factories.dispose();

		m_frameworkData.inputSystem.dispose();
		m_frameworkData.graphicSystem.dispose();
		m_frameworkData.resourceManager.dispose();
		m_frameworkData.mainWindow.dispose();
	}

	bool GameFramework::frame()
	{
		m_frameworkData.frameTimer.update();
		m_frameworkData.mainWindow.update();

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

		m_frameworkData.inputSystem.update( m_frameworkData.mainWindow.getEventBuffer() );

		if( m_frameworkData.inputSystem.hasKeyReleased( KEY_ESCAPE ) )
		{
			return false;
		}
		
		update();

		GraphicsContext* pContext = m_frameworkData.graphicSystem.beginFrame();
		render( pContext );
		m_frameworkData.graphicSystem.endFrame();

		return true;
	}
}