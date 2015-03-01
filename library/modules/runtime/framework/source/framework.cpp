
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

	DebugGui& framework::getDebugGui()
	{
		return getFrameworkData().debugGui;
	}
}