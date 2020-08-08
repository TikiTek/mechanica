#include "tiki/converter_editor/converter_editor_ribbon.hpp"

#include "tiki/tool_application/tool_ui.hpp"

#include "res_converter_editor.hpp"

namespace tiki
{
	ConverterEditorRibbon::ConverterEditorRibbon()
		: EditorRibbon( "Converter" )
		, m_convertAllIcon( getConverterEditorResource( ConverterEditorResources_RibbonConvertAll ) )
		, m_showConverterIcon( getConverterEditorResource( ConverterEditorResources_RibbonShowConverter ) )
		, m_showLastResultIcon( getConverterEditorResource( ConverterEditorResources_RibbonShowLastResult ) )
	{
	}

	void ConverterEditorRibbon::doUi()
	{
		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_convertAllIcon ), "Convert all" ) )
		{

		}
		ImGui::SameLine();

		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_showConverterIcon ), "Show Converter" ) )
		{

		}
		ImGui::SameLine();

		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_showLastResultIcon ), "Show last result" ) )
		{

		}
		ImGui::SameLine();
	}
}