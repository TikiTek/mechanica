#ifndef TIKI_FRAMEWORK_HPP__
#define TIKI_FRAMEWORK_HPP__

#include "tiki/base/types.hpp"

namespace tiki
{
	class DebugGui;
	class GraphicsSystem;
	class IWebInterface;
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

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		DebugGui&					getDebugGui();
#endif

#if TIKI_ENABLED( TIKI_WEB_INTERFACE )
		IWebInterface*				getWebInterface();
#endif
	}
}

#endif // TIKI_FRAMEWORK_HPP__
