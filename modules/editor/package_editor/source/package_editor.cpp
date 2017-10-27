#include "tiki/package_editor/package_editor.hpp"

#include "tiki/editor_interface/ieditorinterface.hpp"
#include "tiki/editor_interface/ifile.hpp"
#include "tiki/package_editor/package_editor_widget.hpp"
#include "tiki/package_editor/package_file_browser_widget.hpp"
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

		m_pFileBrowser = new PackageFileBrowserWidget( pInterface );

		m_globalTabs.insert( m_pRibbon );
		m_globalDocks.insert( m_pFileBrowser );
	}

	PackageEditor::~PackageEditor()
	{
		delete m_pFileBrowser;
		delete m_pRibbon;
	}

	QWidget* PackageEditor::openFile( IFile* pFile )
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

	bool PackageEditor::saveFile( IFile* pFile )
	{
		PackageEditorWidget* pEditorWidget = (PackageEditorWidget*)pFile->getEditWidget();
		pEditorWidget->savePackage();

		return true;
	}

	void PackageEditor::closeFile( IFile* pFile )
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
		return "package";
	}

	QIcon PackageEditor::getFileIcon() const
	{
		return QIcon( ":/package-editor/browser-package.png" );
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
			"Package (*.package)"
		);

		m_pInterface->openFile( fileName );
	}

	void PackageEditor::openClicked()
	{
		QString fileName = QFileDialog::getOpenFileName(
			m_pInterface->getDialogParent(),
			m_pInterface->getDialogTitle(),
			m_pInterface->getContentPath().absolutePath(),
			"Package (*.package)"
		);

		m_pInterface->openFile( fileName );
	}

	void PackageEditor::closeClicked()
	{
		closePackage();
	}

	void PackageEditor::editClicked()
	{
		m_pInterface->openFile( m_pInterface->getContentPath().absoluteFilePath( m_currentPackageName + ".package" ) );
	}

	void PackageEditor::openPackage( IFile* pPackageFile )
	{
		QFileInfo fileInfo( pPackageFile->getFileName() );
		if( m_currentPackageName == fileInfo.baseName() )
		{
			return;
		}

		closePackage();

		m_pFileBrowser->openPackage( fileInfo.baseName() );
		//m_pInterface->addGlobalDockWidget( m_pFileBrowser );

		m_currentPackageName = fileInfo.baseName();
	}

	void PackageEditor::closePackage()
	{
		if( m_currentPackageName.isEmpty() )
		{
			return;
		}

		m_pInterface->closeAllFiles();

		//m_pInterface->removeGlobalDockWidget( m_pFileBrowser );
		m_pFileBrowser->closePakage();

		m_currentPackageName.clear();
	}
}