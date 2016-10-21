#pragma once
#ifndef TIKI_DEBUGGUIWINDOW_DEBUGPROP_HPP_INCLUDED__
#define TIKI_DEBUGGUIWINDOW_DEBUGPROP_HPP_INCLUDED__

#include "tiki/debuggui/debugguiwindow.hpp"

#include "tiki/container/array.hpp"
#include "tiki/debuggui/debugguialignment.hpp"
#include "tiki/debuggui/debugguibutton.hpp"
#include "tiki/debuggui/debugguihorizontallayout.hpp"
#include "tiki/debuggui/debugguilabel.hpp"
#include "tiki/debuggui/debugguiminimumlayout.hpp"
#include "tiki/debuggui/debugguispacer.hpp"
#include "tiki/debuggui/debugguiverticallayout.hpp"

namespace tiki
{
	class DebugProp;

	class DebugGuiWindowDebugProp : public DebugGuiWindow
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiWindowDebugProp );

	public:

		void						create( DebugGui& debugGui );
		void						dispose();

		virtual void				update( double elapsedTime ) TIKI_OVERRIDE_FINAL;
		virtual void				render( ImmediateRenderer& renderer ) TIKI_OVERRIDE_FINAL;

	protected:

		virtual bool				processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state ) TIKI_OVERRIDE_FINAL;
		virtual bool				processGuiEvent( const DebugGuiEvent& guiEvent ) TIKI_OVERRIDE_FINAL;

	private:

		enum TreeNodeType
		{
			TreeNodeType_Invalid = -1,

			TreeNodeType_Folder,
			TreeNodeType_Property
		};

		enum InputAction
		{
			InputAction_Invalid = -1,

			InputAction_MoveUp,
			InputAction_MoveDown,
			InputAction_Increse,
			InputAction_Decrese
		};

		struct TreeNode
		{
			uint						id;
			TreeNodeType				type;

			DebugGuiHorizontalLayout	nodeLayout;
			DebugGuiLabel				nameLabel;

			uint						parentIndex;
		};

		struct TreeFolderNode : public TreeNode
		{
			DebugGuiVerticalLayout		fullLayout;

			DebugGuiButton				expandButton;

			DebugGuiHorizontalLayout	spaceLayout;
			DebugGuiSpacer				spacer;
			
			DebugGuiVerticalLayout		chilrenLayout;
		};

		struct TreePropNode : public TreeNode
		{
			DebugGuiAlignment			valueAlignment;
			DebugGuiLabel				valueLabel;

			DebugProp*					pProperty;
		};

		DebugGuiVerticalLayout		m_baseLayout;

		Array< TreeFolderNode >		m_folderNodes;
		Array< TreePropNode >		m_propNodes;

		TreeNode*					m_pSelectedNode;

		InputAction					m_inputAction;
		double						m_inputTimer;
		double						m_holdTimer;
		TreeNode*					m_pInputNode;
		
		void						setLayoutParameters( DebugGuiHorizontalLayout& layout );
		void						setLayoutParameters( DebugGuiVerticalLayout& layout );

		void						expandFolderNode( TreeFolderNode& parentNode );
		void						collapseFolderNode( TreeFolderNode& parentNode );
		void						changePropNode( TreePropNode& node, bool increase );

		void						setDebugPropText( DebugGuiLabel& targetLabel, const DebugProp& prop );

		TreeNode*					findNearestNode( const Vector2& position, bool up, const TreeNode* pExcludeNode );
		bool						isNodeNearest( const TreeNode& node, float& nearestAbsDistance, float sourcePositionY, bool up ) const;

	};
}

#endif // TIKI_DEBUGGUIWINDOW_DEBUGPROP_HPP_INCLUDED__
