#pragma once
#ifndef __TIKI_DEBUGGUI_TESTWINDOW_HPP_INCLUDED__
#define __TIKI_DEBUGGUI_TESTWINDOW_HPP_INCLUDED__

#include "tiki/debuggui/debugguiwindow.hpp"

#include "tiki/debuggui/debugguibutton.hpp"
#include "tiki/debuggui/debugguicheckbox.hpp"
#include "tiki/debuggui/debugguihorizontallayout.hpp"
#include "tiki/debuggui/debugguilabel.hpp"
#include "tiki/debuggui/debugguiminimumlayout.hpp"
#include "tiki/debuggui/debugguislider.hpp"
#include "tiki/debuggui/debugguiverticallayout.hpp"

namespace tiki
{
	class DebugGuiTestWindow : public DebugGuiWindow
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiTestWindow );

	public:

		void						create( DebugGui& debugGui );
		void						dispose();

	private:

		DebugGuiMinimumLayout		m_baseLayout;
		DebugGuiHorizontalLayout	m_bottomLayout;
		DebugGuiVerticalLayout		m_labelLayout;

		DebugGuiButton				m_buttons[ 5u ];
		DebugGuiLabel				m_labels[ 5u ];
		DebugGuiSlider				m_slider;
		DebugGuiCheckBox			m_checkBox;

	};
}

#endif // __TIKI_DEBUGGUI_TESTWINDOW_HPP_INCLUDED__