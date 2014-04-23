#pragma once
#ifndef __TIKI_DEBUGGUI_TESTWINDOW_HPP_INCLUDED__
#define __TIKI_DEBUGGUI_TESTWINDOW_HPP_INCLUDED__

#include "tiki/debuggui/debugguiwindow.hpp"

namespace tiki
{
	class DebugGuiTestWindow : public DebugGuiWindow
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiTestWindow );

	public:

		void	create( DebugGui& debugGui );
		void	dispose();

	};
}

#endif // __TIKI_DEBUGGUI_TESTWINDOW_HPP_INCLUDED__