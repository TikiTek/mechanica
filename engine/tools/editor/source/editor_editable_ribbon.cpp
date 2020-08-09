#include "editor_editable_ribbon.hpp"

#include "tiki/tool_application/tool_ui.hpp"

#include "res_editor_project.hpp"

#include <imgui.h>

namespace tiki
{
	EditorEditableRibbon::EditorEditableRibbon( Editor& editor )
		: EditorRibbon( "File" )
		, m_editor( editor )
		, m_openEditableIcon(getEditorProjectResource(EditorProjectResources_EditableOpen ) )
		, m_saveEditableIcon(getEditorProjectResource(EditorProjectResources_EditableSave ) )
		, m_closeEditableIcon(getEditorProjectResource( EditorProjectResources_EditableClose ) )
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
				m_editor.saveEditable( *m_editor.getCurrentEditable() );
			}
			ImGui::SameLine();

			if( ImGui::ImageButtonTextButton( ImGui::Tex( m_closeEditableIcon ), "Close" ) )
			{
				m_editor.closeEditable( *m_editor.getCurrentEditable() );
			}
		}
	}
}
