#pragma once
#ifndef TIKI_PACKAGEEDITOR_HPP_INCLUDED
#define TIKI_PACKAGEEDITOR_HPP_INCLUDED

#include "tiki/editorinterface/ifileeditor.hpp"

namespace tiki
{
	class QtRibbonButton;

	class PackageEditor : public IFileEditor
	{
		TIKI_NONCOPYABLE_CLASS( PackageEditor );

	public:

									PackageEditor();
		virtual						~PackageEditor();

		QWidget*					openFile( const QString& fileName ) TIKI_OVERRIDE_FINAL;
		void						closeFile( QWidget* pWidget ) TIKI_OVERRIDE_FINAL;

		QString						getFileType() const TIKI_OVERRIDE_FINAL;
		ArrayView< QtRibbonTab* >	getEditorRibbons() const TIKI_OVERRIDE_FINAL;
		ArrayView< QDockWidget* >	getDockWidgets() const TIKI_OVERRIDE_FINAL;

	private:

		QtRibbonTab*				m_pRibbon;
		QtRibbonButton*				m_pNewPackageButton;
		QtRibbonButton*				m_pOpenPackageButton;
		QtRibbonButton*				m_pSavePackageButton;
		QtRibbonButton*				m_pClosePackageButton;

	};
}

#endif // TIKI_PACKAGEEDITOR_HPP_INCLUDED
