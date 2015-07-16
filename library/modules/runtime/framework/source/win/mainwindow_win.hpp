
#include "tiki/framework/mainwindow.hpp"

#include "tiki/base/memory.hpp"

#include <windows.h>

namespace tiki
{
	struct MainWindowPlatformData
	{
		MainWindowPlatformData()
		{
			windowHandle = nullptr;
		}

		HWND	windowHandle;
	};
}
