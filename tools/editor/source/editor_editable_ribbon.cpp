#include "editor_editable_ribbon.hpp"

#include "tiki/toolapplication/tool_ui.hpp"

#include "res_editor.hpp"

#include <imgui.h>

namespace tiki
{
	EditorEditableRibbon::EditorEditableRibbon( Editor& editor )
		: EditorRibbon( "File" )
		, m_editor( editor )
		, m_saveEditableIcon( getEditorResource( EditorResources_EditableSave ) )
		, m_closeEditableIcon( getEditorResource( EditorResources_EditableClose ) )
	{
	}

	EditorEditableRibbon::~EditorEditableRibbon()
	{
	}

	void EditorEditableRibbon::doUi()
	{
		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_saveEditableIcon ), "Save" ) )
		{
			m_editor.saveEditable( m_editor.getCurrentEditable() );
		}
		ImGui::SameLine();

		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_closeEditableIcon ), "Close" ) )
		{
			m_editor.closeEditable( m_editor.getCurrentEditable() );
		}
		ImGui::SameLine();
	}
}
