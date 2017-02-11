#pragma once
#ifndef TIKI_PACKAGEEDITOR_HPP_INCLUDED
#define TIKI_PACKAGEEDITOR_HPP_INCLUDED

#include "tiki/editorinterface/ifileeditor.hpp"

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
		virtual bool							saveFile( IFile* pFile ) TIKI_OVERRIDE_FINAL;
		virtual void							closeFile( IFile* pFile ) TIKI_OVERRIDE_FINAL;

		virtual QString							getFileTypeName() const TIKI_OVERRIDE_FINAL;
		virtual QString							getFileExtension() const TIKI_OVERRIDE_FINAL;
		virtual QIcon							getFileIcon() const TIKI_OVERRIDE_FINAL;

		virtual const QVector< QtRibbonTab* >&	getRibbonTabs() const TIKI_OVERRIDE_FINAL;
		virtual const QVector< QDockWidget* >&	getDockWidgets() const TIKI_OVERRIDE_FINAL;

		QString									getPackageName() const;

	private	slots:

		void									newClicked();
		void									openClicked();
		void									closeClicked();
		void									editClicked();

	private:

		IEditorInterface*			m_pInterface;

		QtRibbonTab*				m_pRibbon;
		QtRibbonButton*				m_pNewPackageButton;
		QtRibbonButton*				m_pOpenPackageButton;
		QtRibbonButton*				m_pClosePackageButton;
		QtRibbonButton*				m_pEditPackageButton;

		PackageFileBrowserWidget*			m_pFileBrowser;
		QString						m_currentPackageName;

		QVector< QtRibbonTab* >		m_tabs;
		QVector< QDockWidget* >		m_docks;

		void						openPackage( IFile* pPackageFile );
		void						closePackage();
	};
}

#endif // TIKI_PACKAGEEDITOR_HPP_INCLUDED
