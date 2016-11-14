#pragma once
#ifndef TIKI_PACKAGEEDITOR_HPP_INCLUDED
#define TIKI_PACKAGEEDITOR_HPP_INCLUDED

#include "tiki/editorinterface/ifileeditor.hpp"

#include <QObject>

namespace tiki
{
	class IEditorInterface;
	class PackageFileBrowser;
	class QtRibbonButton;

	class PackageEditor : public QObject, public IFileEditor
	{
		Q_OBJECT
		TIKI_NONCOPYABLE_CLASS( PackageEditor );

	public:

												PackageEditor( IEditorInterface* pInterface );
		virtual									~PackageEditor();

		virtual QWidget*						openFile( IEditorFile* pFile ) TIKI_OVERRIDE_FINAL;
		virtual bool							saveFile( IEditorFile* pFile ) TIKI_OVERRIDE_FINAL;
		virtual void							closeFile( IEditorFile* pFile ) TIKI_OVERRIDE_FINAL;

		bool									openPackage( IEditorFile* pPackageFile );
		void									closePackage();

		virtual QString							getFileExtension() const TIKI_OVERRIDE_FINAL;
		virtual const QVector< QtRibbonTab* >&	getRibbonTabs() const TIKI_OVERRIDE_FINAL;
		virtual const QVector< QDockWidget* >&	getDockWidgets() const TIKI_OVERRIDE_FINAL;

	private	slots:

		void									newClicked();
		void									openClicked();
		void									saveClicked();
		void									closeClicked();

	private:

		IEditorInterface*			m_pInterface;

		QtRibbonTab*				m_pRibbon;
		QtRibbonButton*				m_pNewPackageButton;
		QtRibbonButton*				m_pOpenPackageButton;
		QtRibbonButton*				m_pSavePackageButton;
		QtRibbonButton*				m_pClosePackageButton;

		PackageFileBrowser*			m_pFileBrowser;

		IEditorFile*				m_pCurrentPackageFile;

		QVector< QtRibbonTab* >		m_tabs;
		QVector< QDockWidget* >		m_docks;
	};
}

#endif // TIKI_PACKAGEEDITOR_HPP_INCLUDED
