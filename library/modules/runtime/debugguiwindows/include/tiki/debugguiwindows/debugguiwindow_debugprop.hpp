#pragma once
#ifndef TIKI_DEBUGGUIWINDOW_DEBUGPROP_HPP_INCLUDED__
#define TIKI_DEBUGGUIWINDOW_DEBUGPROP_HPP_INCLUDED__

#include "tiki/debuggui/debugguiwindow.hpp"

#include "tiki/base/array.hpp"
#include "tiki/debuggui/debugguialignment.hpp"
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
			DebugGuiVerticalLayout		fullLayout;

			DebugGuiHorizontalLayout	nodeLayout;
			DebugGuiButton				expandButton;
			DebugGuiLabel				nameLabel;

			DebugGuiHorizontalLayout	spaceLayout;
			DebugGuiSpacer				spacer;
			
			DebugGuiVerticalLayout		chilrenLayout;

			uint						parentIndex;
		};

		struct TreePropNode
		{
			DebugGuiHorizontalLayout	nodeLayout;
			DebugGuiLabel				nameLabel;
			DebugGuiAlignment			valueAlignment;
			DebugGuiLabel				valueLabel;

			uint						parentIndex;
		};

		DebugGuiHorizontalLayout	m_baseLayout;

		Array< TreeFolderNode >		m_folderNodes;
		Array< TreePropNode >		m_propNodes;

		void						setLayoutParameters( DebugGuiHorizontalLayout& layout );
		void						setLayoutParameters( DebugGuiVerticalLayout& layout );

	};
}

#endif // TIKI_DEBUGGUIWINDOW_DEBUGPROP_HPP_INCLUDED__
