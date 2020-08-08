#pragma once
#ifndef TIKI_DEBUGGUIWINDOWS_HPP_INCLUDED__
#define TIKI_DEBUGGUIWINDOWS_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#include "tiki/debugguiwindows/debugguiwindow_debugprop.hpp"

namespace tiki
{
	class DebugGui;

	class DebugGuiWindows
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiWindows );

	public:

		void	create( DebugGui& debugGui );
		void	dispose();

	private:

		DebugGuiWindowDebugProp	m_windowDebugProp;

	};
}

#endif // TIKI_DEBUGGUIWINDOWS_HPP_INCLUDED__
