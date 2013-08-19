
#include "tiki/framework/framework.hpp"

#include "tiki/framework/gameframework.hpp"

#include "frameworkdata.hpp"

namespace tiki
{
	int framework::run()
	{
		return getGameFramework().run();
	}

	MainWindow& framework::getMainWindow()
	{
		return getFrameworkData().mainWindow;
	}

	PhysicSystem& framework::getPhysicSystem()
	{
		return getFrameworkData().physicSystem;
	}

	GraphicsSystem& framework::getGraphicsSystem()
	{
		return getFrameworkData().graphicSystem;
	}

	ResourceManager& framework::getResourceManager()
	{
		return getFrameworkData().resourceManager;
	}

	EventSystem& framework::getEventSystem()
	{
		return getFrameworkData().eventSystem;
	}

	InputSystem& framework::getInputSystem()
	{
		return getFrameworkData().inputSystem;
	}

	TimeSystem& framework::getTimeSystem()
	{
		return getFrameworkData().timeSystem;
	}

	const WindowEventBuffer& framework::getWindowEventBuffer()
	{
		return getFrameworkData().mainWindow.getEventBuffer();
	}
}