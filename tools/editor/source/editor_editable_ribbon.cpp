#include "editor_editable_ribbon.hpp"

#include "tiki/tool_application/tool_ui.hpp"

#include "res_editor.hpp"

#include <imgui.h>

namespace tiki
{
	EditorEditableRibbon::EditorEditableRibbon( Editor& editor )
		: EditorRibbon( "File" )
		, m_editor( editor )
		, m_openEditableIcon( getEditorResource( EditorResources_EditableOpen ) )
		, m_saveEditableIcon( getEditorResource( EditorResources_EditableSave ) )
		, m_closeEditableIcon( getEditorResource( EditorResources_EditableClose ) )
	{
	}

	EditorEditableRibbon::~EditorEditableRibbon()
	{
	}

	void EditorEditableRibbon::doUi()
	{
		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_openEditableIcon ), "Open" ) )
		{
			//m_editor.openEditable( m_editor.getCurrentEditable() );
		}

		if( m_editor.getCurrentEditable() != nullptr )
		{
			ImGui::SameLine();

			if( ImGui::ImageButtonTextButton( ImGui::Tex( m_saveEditableIcon ), "Save" ) )
			{
				m_editor.saveEditable( m_editor.getCurrentEditable() );
			}
			ImGui::SameLine();

			if( ImGui::ImageButtonTextButton( ImGui::Tex( m_closeEditableIcon ), "Close" ) )
			{
				m_editor.closeEditable( m_editor.getCurrentEditable() );
			}
		}
	}
}
