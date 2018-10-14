#pragma once

#include "tiki/editor_interface/file_editor.hpp"

#include "tiki/package_editor/package_editor_ribbon.hpp"

namespace tiki
{
	class EditorInterface;

	class PackageEditor : public FileEditor
	{
		TIKI_NONCOPYABLE_CLASS( PackageEditor );

	public:

								PackageEditor( EditorInterface* pInterface );
		virtual					~PackageEditor();

		virtual EditableFile*	openFile( const Path& fileName ) TIKI_OVERRIDE_FINAL;
		virtual bool			saveEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void			closeEditable( Editable* pEditable ) TIKI_OVERRIDE_FINAL;

		DynamicString			getPackageName() const;

	private:

		PackageEditorRibbon		m_ribbon;

		//QtRibbonTab*				m_pRibbon;
		//QtRibbonButton*				m_pNewPackageButton;
		//QtRibbonButton*				m_pOpenPackageButton;
		//QtRibbonButton*				m_pClosePackageButton;
		//QtRibbonButton*				m_pEditPackageButton;

		//PackageFileBrowserWidget*			m_pFileBrowser;
		DynamicString				m_currentPackageName;

		void						openPackage( EditableFile* pPackageFile );
		void						closePackage();

		void									newClicked();
		void									openClicked();
		void									closeClicked();
		void									editClicked();
	};
}
