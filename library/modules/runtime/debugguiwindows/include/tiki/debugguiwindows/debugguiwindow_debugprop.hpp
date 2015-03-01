#pragma once
#ifndef TIKI_DEBUGGUIWINDOW_DEBUGPROP_HPP_INCLUDED__
#define TIKI_DEBUGGUIWINDOW_DEBUGPROP_HPP_INCLUDED__

#include "tiki/debuggui/debugguiwindow.hpp"

#include "tiki/base/array.hpp"
#include "tiki/debuggui/debugguibutton.hpp"
#include "tiki/debuggui/debugguihorizontallayout.hpp"
#include "tiki/debuggui/debugguilabel.hpp"
#include "tiki/debuggui/debugguiverticallayout.hpp"

namespace tiki
{
	class DebugGuiWindowDebugProp : public DebugGuiWindow
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiWindowDebugProp );

	public:

		void						create( DebugGui& debugGui );
		void						dispose();

	private:

		struct TreeFolderNode
		{
			DebugGuiHorizontalLayout	layout;
			DebugGuiButton				expandButton;
			DebugGuiLabel				name;
			DebugGuiVerticalLayout		childrenLayout;
		};

		struct TreePropNode
		{
			DebugGuiHorizontalLayout	layout;
			DebugGuiLabel				name;
		};

		Array< TreeFolderNode >		m_folderNodes;
		Array< TreePropNode >		m_propNodes;

	};
}

#endif // TIKI_DEBUGGUIWINDOW_DEBUGPROP_HPP_INCLUDED__
