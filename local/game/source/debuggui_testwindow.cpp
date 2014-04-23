
#include "tiki/game/debuggui_testwindow.hpp"

namespace tiki
{
	void DebugGuiTestWindow::create( DebugGui& debugGui )
	{
		DebugGuiWindow::create( debugGui, "Test Window" );
	}

	void DebugGuiTestWindow::dispose()
	{
		DebugGuiWindow::dispose();
	}
}