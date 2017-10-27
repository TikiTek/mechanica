#pragma once
#ifndef TIKI_PACKAGE_EDITOR_HPP_INCLUDED
#define TIKI_PACKAGE_EDITOR_HPP_INCLUDED

#include "tiki/editor_interface/ifileeditor.hpp"

#include <QObject>

namespace tiki
{
	class IEditorInterface;
	class PackageFileBrowserWidget;
	class QtRibbonButton;

	class PackageEditor : public QObject, public IFileEditor
	{
		Q_OBJECT
		TIKI_NONCOPYABLE_CLASS( PackageEditor );

	public:

												PackageEditor( IEditorInterface* pInterface );
		virtual									~PackageEditor();

		virtual QWidget*						openFile( IFile* pFile ) TIKI_OVERRIDE_FINAL;
		virtual bool							saveEditable( IEditable* pEditable ) TIKI_OVERRIDE_FINAL;
		virtual void							closeEditable( IEditable* pEditable ) TIKI_OVERRIDE_FINAL;

		virtual QString							getFileTypeName() const TIKI_OVERRIDE_FINAL;
		virtual QString							getFileExtension() const TIKI_OVERRIDE_FINAL;
		virtual QIcon							getEditableIcon() const TIKI_OVERRIDE_FINAL;

		QString									getPackageName() const;

	private	slots:

		void									newClicked();
		void									openClicked();
		void									closeClicked();
		void									editClicked();

	private:

		IEditorInterface*			m_pInterface;

		QIcon						m_icon;

		QtRibbonTab*				m_pRibbon;
		QtRibbonButton*				m_pNewPackageButton;
		QtRibbonButton*				m_pOpenPackageButton;
		QtRibbonButton*				m_pClosePackageButton;
		QtRibbonButton*				m_pEditPackageButton;

		PackageFileBrowserWidget*			m_pFileBrowser;
		QString						m_currentPackageName;

		void						openPackage( IFile* pPackageFile );
		void						closePackage();
	};
}

#endif // TIKI_PACKAGE_EDITOR_HPP_INCLUDED
