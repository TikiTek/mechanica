#pragma once
#ifndef TIKI_DEBUGGUIWINDOW_DEBUGPROP_HPP_INCLUDED__
#define TIKI_DEBUGGUIWINDOW_DEBUGPROP_HPP_INCLUDED__

#include "tiki/debuggui/debugguiwindow.hpp"

#include "tiki/base/array.hpp"
#include "tiki/debuggui/debugguibutton.hpp"
#include "tiki/debuggui/debugguihorizontallayout.hpp"
#include "tiki/debuggui/debugguilabel.hpp"
#include "tiki/debuggui/debugguiminimumlayout.hpp"
#include "tiki/debuggui/debugguispacer.hpp"
#include "tiki/debuggui/debugguiverticallayout.hpp"

namespace tiki
{
	class DebugGuiWindowDebugProp : public DebugGuiWindow
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiWindowDebugProp );

	public:

		void						create( DebugGui& debugGui );
		void						dispose();

	protected:

		virtual bool				processGuiEvent( const DebugGuiEvent& guiEvent );

	private:

		struct TreeFolderNode
		{
			DebugGuiHorizontalLayout	fullLayout;

			DebugGuiVerticalLayout		nodeLayout;
			DebugGuiButton				expandButton;
			DebugGuiLabel				nameLabel;

			DebugGuiVerticalLayout		spaceLayout;
			DebugGuiSpacer				spacer;
			
			DebugGuiHorizontalLayout	chilrenLayout;

			uint						parentIndex;
		};

		struct TreePropNode
		{
			DebugGuiVerticalLayout		nodeLayout;
			DebugGuiLabel				nameLabel;
			DebugGuiLabel				valueLabel;

			uint						parentIndex;
		};

		DebugGuiMinimumLayout		m_baseLayout;
		DebugGuiVerticalLayout		m_nodesLayout;

		Array< TreeFolderNode >		m_folderNodes;
		Array< TreePropNode >		m_propNodes;

	};
}

#endif // TIKI_DEBUGGUIWINDOW_DEBUGPROP_HPP_INCLUDED__
