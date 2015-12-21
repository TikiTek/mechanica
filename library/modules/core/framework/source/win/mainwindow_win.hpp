
#include "tiki/framework/mainwindow.hpp"

#include "tiki/base/memory.hpp"

#include <windows.h>

namespace tiki
{
	class MainWindow;

	typedef bool( *WindowMessageHandlerFunc )(void* pUserData, UINT message, WPARAM wParam, LPARAM lParam);

	enum MainWindowMesageHandler
	{
		MainWindowMesageHandler_Window,
		MainWindowMesageHandler_InputSystem,
		MainWindowMesageHandler_GraphicsSystem,

		MainWindowMesageHandler_Count
	};

	struct MainWindowUserData
	{
		MainWindowUserData()
		{
			for( uint i = 0u; i < TIKI_COUNT( apUserData ); ++i )
			{
				apUserData[ i ]			= nullptr;
				apHandlerFunction[ i ]	= nullptr;
			}
		}

		void*						apUserData[ MainWindowMesageHandler_Count ];
		WindowMessageHandlerFunc	apHandlerFunction[ MainWindowMesageHandler_Count ];
	};

	struct MainWindowPlatformData
	{
		MainWindowPlatformData()
		{
			windowHandle = nullptr;
		}

		HWND				windowHandle;

		MainWindowUserData	userData;
	};

	class MainWindowPlatform
	{
	public:

		static bool			handleWindowMessage( MainWindow& mainWindow, UINT message, WPARAM wParam, LPARAM lParam );

	};
}
