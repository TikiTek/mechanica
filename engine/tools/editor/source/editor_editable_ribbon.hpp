#pragma once

#include "tiki/editor_interface/editor_ribbon.hpp"

namespace tiki
{
	class Editor;

	class EditorEditableRibbon : public EditorRibbon
	{
		TIKI_NONCOPYABLE_CLASS( EditorEditableRibbon );

	public:

						EditorEditableRibbon( Editor& editor );
		virtual			~EditorEditableRibbon();

		virtual void	doUi() TIKI_OVERRIDE_FINAL;

	private:

		Editor&			m_editor;

		ToolImage		m_saveEditableIcon;
		ToolImage		m_closeEditableIcon;
	};
}
