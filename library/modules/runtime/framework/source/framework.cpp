
#include "tiki/framework/framework.hpp"

#include "tiki/framework/gameframework.hpp"

#include "frameworkdata.hpp"

namespace tiki
{
	int framework::run()
	{
		return framework::getGame().run();
	}

	MainWindow& framework::getMainWindow()
	{
		return getFrameworkData().mainWindow;
	}

	GraphicsSystem& framework::getGraphicsSystem()
	{
		return getFrameworkData().graphicSystem;
	}

	ResourceManager& framework::getResourceManager()
	{
		return getFrameworkData().resourceManager;
	}

	InputSystem& framework::getInputSystem()
	{
		return getFrameworkData().inputSystem;
	}

	const Timer& framework::getFrameTimer()
	{
		return getFrameworkData().frameTimer;
	}

	const WindowEventBuffer& framework::getWindowEventBuffer()
	{
		return getFrameworkData().mainWindow.getEventBuffer();
	}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	DebugGui& framework::getDebugGui()
	{
		return getFrameworkData().debugGui;
	}
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
	IWebInterface* framework::getWebInterface()
	{
		return getFrameworkData().pWebInterface;
	}
#endif
}