#ifndef TIKI_FRAMEWORK_HPP__
#define TIKI_FRAMEWORK_HPP__

namespace tiki
{
	class GraphicsSystem;
	class InputSystem;
	class MainWindow;
	class PhysicSystem;
	class ResourceManager;
	class TimeSystem;
	class WindowEventBuffer;
	class EventSystem;

	namespace framework
	{
		int							run();

		MainWindow&					getMainWindow();
		PhysicSystem&				getPhysicSystem();

		GraphicsSystem&				getGraphicsSystem();
		ResourceManager&			getResourceManager();

		EventSystem&				getEventSystem();

		InputSystem&				getInputSystem();
		TimeSystem&					getTimeSystem();
		const WindowEventBuffer&	getWindowEventBuffer();
	}
}

#endif // TIKI_FRAMEWORK_HPP__
