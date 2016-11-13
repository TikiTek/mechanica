#pragma once
#ifndef TIKI_PACKAGEEDITOR_HPP_INCLUDED
#define TIKI_PACKAGEEDITOR_HPP_INCLUDED

#include "tiki/editorinterface/ifileeditor.hpp"

namespace tiki
{
	class IEditorInterface;
	class QtRibbonButton;

	class PackageEditor : public IFileEditor
	{
		TIKI_NONCOPYABLE_CLASS( PackageEditor );

	public:

												PackageEditor( IEditorInterface* pInterface );
		virtual									~PackageEditor();

		virtual QWidget*						openFile( const QString& fileName ) TIKI_OVERRIDE_FINAL;
		virtual void							saveFile( QWidget* pWidget ) TIKI_OVERRIDE_FINAL;
		virtual void							closeFile( QWidget* pWidget ) TIKI_OVERRIDE_FINAL;

		virtual QString							getFileExtension() const TIKI_OVERRIDE_FINAL;
		virtual const QVector< QtRibbonTab* >&	getRibbonTabs() const TIKI_OVERRIDE_FINAL;
		virtual const QVector< QDockWidget* >&	getDockWidgets() const TIKI_OVERRIDE_FINAL;

	private:

		IEditorInterface*			m_pInterface;

		QtRibbonTab*				m_pRibbon;
		QtRibbonButton*				m_pNewPackageButton;
		QtRibbonButton*				m_pOpenPackageButton;
		QtRibbonButton*				m_pSavePackageButton;
		QtRibbonButton*				m_pClosePackageButton;

		QVector< QtRibbonTab* >		m_tabs;
		QVector< QDockWidget* >		m_docks;
	};
}

#endif // TIKI_PACKAGEEDITOR_HPP_INCLUDED
