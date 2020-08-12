
#include "tiki/debugguiwindows/debugguiwindows.hpp"

namespace tiki
{
	void DebugGuiWindows::create( DebugGui& debugGui )
	{
		m_windowDebugProp.create( debugGui );
	}

	void DebugGuiWindows::dispose()
	{
		m_windowDebugProp.dispose();
	}
}