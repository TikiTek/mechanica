#include "tiki/packageeditor/packageeditor.hpp"

#include "tiki/editorinterface/ieditorfile.hpp"
#include "tiki/editorinterface/ieditorinterface.hpp"
#include "tiki/packageeditor/packageeditorwidget.hpp"
#include "tiki/packageeditor/packagefilebrowser.hpp"
#include "tiki/qtapplication/qtribbonbutton.hpp"
#include "tiki/qtapplication/qtribbontab.hpp"

#include <QFileDialog>
#include <QResource>

namespace tiki
{
	PackageEditor::PackageEditor( IEditorInterface* pInterface )
		: m_pInterface( pInterface )
	{
		QResource::registerResource( "packageeditor.rcc" );

		m_pRibbon = new QtRibbonTab( "Package" );
		m_pNewPackageButton		= m_pRibbon->addButton( "New", QIcon( ":/package-editor/ribbon-package-new.png" ) );
		m_pOpenPackageButton	= m_pRibbon->addButton( "Open", QIcon( ":/package-editor/ribbon-package-open.png" ) );
		m_pClosePackageButton	= m_pRibbon->addButton( "Close", QIcon( ":/package-editor/ribbon-package-close.png" ) );
		m_pEditPackageButton	= m_pRibbon->addButton( "Edit", QIcon( ":/package-editor/ribbon-package-edit.png" ) );

		connect( m_pNewPackageButton, &QtRibbonButton::clicked, this, &PackageEditor::newClicked );
		connect( m_pOpenPackageButton, &QtRibbonButton::clicked, this, &PackageEditor::openClicked );
		connect( m_pClosePackageButton, &QtRibbonButton::clicked, this, &PackageEditor::closeClicked );
		connect( m_pEditPackageButton, &QtRibbonButton::clicked, this, &PackageEditor::editClicked );

		pInterface->addGlobalRibbonTab( m_pRibbon );

		m_pFileBrowser = new PackageFileBrowser( pInterface );
	}

	PackageEditor::~PackageEditor()
	{
		m_pInterface->removeGlobalRibbonTab( m_pRibbon );
	}

	QWidget* PackageEditor::openFile( IEditorFile* pFile )
	{
		openPackage( pFile );

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

	QString PackageEditor::getFileTypeName() const
	{
		return "Package";
	}

	QString PackageEditor::getFileExtension() const
	{
		return "tikipackage";
	}

	QIcon PackageEditor::getFileIcon() const
	{
		return QIcon( ":/package-editor/browser-package.png" );
	}

	const QVector< QtRibbonTab* >& PackageEditor::getRibbonTabs() const
	{
		return m_tabs;
	}

	const QVector< QDockWidget* >& PackageEditor::getDockWidgets() const
	{
		return m_docks;
	}

	QString PackageEditor::getPackageName() const
	{
		return m_currentPackageName;
	}

	void PackageEditor::newClicked()
	{
		QString fileName = QFileDialog::getSaveFileName(
			m_pInterface->getDialogParent(),
			m_pInterface->getDialogTitle(),
			m_pInterface->getContentPath().absolutePath(),
			"Package (*.tikipackage)"
		);

		m_pInterface->openFile( fileName );
	}

	void PackageEditor::openClicked()
	{
		QString fileName = QFileDialog::getOpenFileName(
			m_pInterface->getDialogParent(),
			m_pInterface->getDialogTitle(),
			m_pInterface->getContentPath().absolutePath(),
			"Package (*.tikipackage)"
		);

		m_pInterface->openFile( fileName );
	}

	void PackageEditor::closeClicked()
	{
		closePackage();
	}

	void PackageEditor::editClicked()
	{
		m_pInterface->openFile( m_pInterface->getContentPath().absoluteFilePath( m_currentPackageName + ".tikipackage" ) );
	}

	void PackageEditor::openPackage( IEditorFile* pPackageFile )
	{
		QFileInfo fileInfo( pPackageFile->getFileName() );
		if( m_currentPackageName == fileInfo.baseName() )
		{
			return;
		}

		closePackage();

		m_pFileBrowser->openPackage( fileInfo.baseName() );
		m_pInterface->addGlobalDockWidget( m_pFileBrowser );

		m_currentPackageName = fileInfo.baseName();
	}

	void PackageEditor::closePackage()
	{
		if( m_currentPackageName.isEmpty() )
		{
			return;
		}

		m_pInterface->closeAllFiles();

		m_pInterface->removeGlobalDockWidget( m_pFileBrowser );
		m_pFileBrowser->closePakage();

		m_currentPackageName.clear();
	}
}