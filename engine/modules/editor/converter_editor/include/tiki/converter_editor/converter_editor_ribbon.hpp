#pragma  once

#include "tiki/editor_interface/editor_ribbon.hpp"

#include "tiki/toolapplication/tool_image.hpp"

namespace tiki
{
	class ConverterEditorRibbon : public EditorRibbon
	{
	public:

						ConverterEditorRibbon();

		virtual void	doUi() TIKI_OVERRIDE_FINAL;

	private:

		ToolImage		m_convertAllIcon;
		ToolImage		m_showConverterIcon;
		ToolImage		m_showLastResultIcon;
	};
}