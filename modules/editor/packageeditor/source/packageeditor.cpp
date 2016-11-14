#include "tiki/packageeditor/packageeditor.hpp"

#include "tiki/editorinterface/ieditorfile.hpp"
#include "tiki/editorinterface/ieditorinterface.hpp"
#include "tiki/packageeditor/packageeditorwidget.hpp"
#include "tiki/packageeditor/packagefilebrowser.hpp"
#include "tiki/qtapplication/qtribbonbutton.hpp"
#include "tiki/qtapplication/qtribbontab.hpp"

#include <QFileDialog>

namespace tiki
{
	PackageEditor::PackageEditor( IEditorInterface* pInterface )
		: m_pInterface( pInterface )
		, m_pCurrentPackageFile( nullptr )
	{
		m_pRibbon = new QtRibbonTab( "Package" );
		m_pNewPackageButton		= m_pRibbon->addButton( "New" );
		m_pOpenPackageButton	= m_pRibbon->addButton( "Open" );
		m_pSavePackageButton	= m_pRibbon->addButton( "Save" );
		m_pClosePackageButton	= m_pRibbon->addButton( "Close" );

		connect( m_pNewPackageButton, &QtRibbonButton::clicked, this, &PackageEditor::newClicked );
		connect( m_pOpenPackageButton, &QtRibbonButton::clicked, this, &PackageEditor::openClicked );
		connect( m_pSavePackageButton, &QtRibbonButton::clicked, this, &PackageEditor::saveClicked );
		connect( m_pClosePackageButton, &QtRibbonButton::clicked, this, &PackageEditor::closeClicked );

		pInterface->addGlobalRibbonTab( m_pRibbon );

		m_pFileBrowser = new PackageFileBrowser();
	}

	PackageEditor::~PackageEditor()
	{
		m_pInterface->removeGlobalRibbonTab( m_pRibbon );
	}

	QWidget* PackageEditor::openFile( IEditorFile* pFile )
	{
		if( pFile != m_pCurrentPackageFile )
		{
			openPackage( pFile );
		}

		PackageEditorWidget* pEditorWidget = new PackageEditorWidget();
		if( !pEditorWidget->openPackage( pFile ) )
		{
			delete pEditorWidget;
			return nullptr;
		}

		return pEditorWidget;
	}

	bool PackageEditor::saveFile( IEditorFile* pFile )
	{
		PackageEditorWidget* pEditorWidget = (PackageEditorWidget*)pFile->getEditWidget();
		pEditorWidget->savePackage();

		return true;
	}

	void PackageEditor::closeFile( IEditorFile* pFile )
	{
		PackageEditorWidget* pEditorWidget = (PackageEditorWidget*)pFile->getEditWidget();
		delete pEditorWidget;
	}

	bool PackageEditor::openPackage( IEditorFile* pPackageFile )
	{
		closePackage();

		m_pFileBrowser->openPackage( pPackageFile );
		m_pInterface->addGlobalDockWidget( m_pFileBrowser );

		m_pCurrentPackageFile = pPackageFile;
		return true;
	}

	void PackageEditor::closePackage()
	{
		if( m_pCurrentPackageFile == nullptr )
		{
			return;
		}

		m_pInterface->closeAllFiles();

		m_pInterface->removeGlobalDockWidget( m_pFileBrowser );
		m_pFileBrowser->closePakage();

		m_pCurrentPackageFile = nullptr;
	}

	QString PackageEditor::getFileExtension() const
	{
		return "tikipackage";
	}

	const QVector< QtRibbonTab* >& PackageEditor::getRibbonTabs() const
	{
		return m_tabs;
	}

	const QVector< QDockWidget* >& PackageEditor::getDockWidgets() const
	{
		return m_docks;
	}

	void PackageEditor::newClicked()
	{
		QString fileName = QFileDialog::getSaveFileName(
			m_pInterface->getDialogParent(),
			m_pInterface->getDialogTitle(),
			m_pInterface->getContentPath(),
			"Package (*.tikipackage)"
		);

		m_pInterface->openFile( fileName );
	}

	void PackageEditor::openClicked()
	{
		QString fileName = QFileDialog::getOpenFileName(
			m_pInterface->getDialogParent(),
			m_pInterface->getDialogTitle(),
			m_pInterface->getContentPath(),
			"Package (*.tikipackage)"
		);

		m_pInterface->openFile( fileName );
	}

	void PackageEditor::saveClicked()
	{
		m_pInterface->saveFile( m_pCurrentPackageFile );
	}

	void PackageEditor::closeClicked()
	{

		m_pInterface->closeFile( m_pCurrentPackageFile );
		closePackage();
	}
}