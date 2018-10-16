#include "tiki/converter_editor/converter_editor_ribbon.hpp"

#include "tiki/toolapplication/tool_ui.hpp"

namespace tiki
{
	ConverterEditorRibbon::ConverterEditorRibbon()
		: EditorRibbon( "Converter" )
		, m_convertAllIcon( "converter_editor/ribbon-convert-all.png" )
		, m_showConverterIcon( "converter_editor/ribbon-show-converter.png" )
		, m_showLastResultIcon( "converter_editor/ribbon-show-last-result.png" )
	{
	}

	void ConverterEditorRibbon::doUi()
	{
		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_convertAllIcon ), "Convert all" ) )
		{

		}

		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_showConverterIcon ), "Show Converter" ) )
		{

		}

		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_showLastResultIcon ), "Show last result" ) )
		{

		}
	}
}