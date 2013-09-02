#ifndef TIKI_FRAMEWORK_HPP__
#define TIKI_FRAMEWORK_HPP__

namespace tiki
{
	class GraphicsSystem;
	class InputSystem;
	class MainWindow;
	class ResourceManager;
	class Timer;
	class WindowEventBuffer;

	namespace framework
	{
		int							run();

		MainWindow&					getMainWindow();

		GraphicsSystem&				getGraphicsSystem();
		ResourceManager&			getResourceManager();

		InputSystem&				getInputSystem();
		const Timer&				getFrameTimer();
		const WindowEventBuffer&	getWindowEventBuffer();
	}
}

#endif // TIKI_FRAMEWORK_HPP__
