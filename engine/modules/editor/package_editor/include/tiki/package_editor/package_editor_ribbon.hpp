#pragma once

#include "tiki/editor_interface/editor_ribbon.hpp"

namespace tiki
{
	class PackageEditor;

	class PackageEditorRibbon : public EditorRibbon
	{
		TIKI_NONCOPYABLE_CLASS( PackageEditorRibbon );

	public:

								PackageEditorRibbon( EditorInterface* pInterface, PackageEditor& packageEditor );
		virtual					~PackageEditorRibbon();

		virtual void			doUi() TIKI_OVERRIDE_FINAL;

	private:

		EditorInterface*		m_pInterface;
		PackageEditor&			m_packageEditor;

		ToolImage				m_newPackageIcon;
		ToolImage				m_openPackageIcon;
		ToolImage				m_closePackageIcon;
		ToolImage				m_editPackageIcon;

		DynamicString			m_packageName;

		void					doUiPopups();
	};
}
