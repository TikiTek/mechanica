#include "tiki/package_editor/package_editor.hpp"

#include "tiki/editor_interface/editable_file.hpp"
#include "tiki/editor_interface/editor_interface.hpp"
#include "tiki/package_editor/package_file.hpp"

#include "res_package_editor.hpp"

namespace tiki
{
	PackageEditor::PackageEditor( EditorInterface* pInterface )
		: FileEditor( pInterface, getPackageEditorResource( PackageEditorResources_BrowserPackage ), "Package", "package" )
	{
		//m_pRibbon = new QtRibbonTab( "Package" );
		//m_pNewPackageButton		= m_pRibbon->addButton( "New", QIcon( ":/package-editor/ribbon-package-new.png" ) );
		//m_pOpenPackageButton	= m_pRibbon->addButton( "Open", QIcon( ":/package-editor/ribbon-package-open.png" ) );
		//m_pClosePackageButton	= m_pRibbon->addButton( "Close", QIcon( ":/package-editor/ribbon-package-close.png" ) );
		//m_pEditPackageButton	= m_pRibbon->addButton( "Edit", QIcon( ":/package-editor/ribbon-package-edit.png" ) );
		//
		//connect( m_pNewPackageButton, &QtRibbonButton::clicked, this, &PackageEditor::newClicked );
		//connect( m_pOpenPackageButton, &QtRibbonButton::clicked, this, &PackageEditor::openClicked );
		//connect( m_pClosePackageButton, &QtRibbonButton::clicked, this, &PackageEditor::closeClicked );
		//connect( m_pEditPackageButton, &QtRibbonButton::clicked, this, &PackageEditor::editClicked );

		//m_pFileBrowser = new PackageFileBrowserWidget( pInterface );

		m_pInterface->addGlobalRibbon( &m_ribbon );
	}

	PackageEditor::~PackageEditor()
	{
		m_pInterface->removeGlobalRibbon( &m_ribbon );
	}

	EditableFile* PackageEditor::openFile( const Path& fileName )
	{
		PackageFile* pPackage = new PackageFile( fileName, this );
		if( !pPackage->load() )
		{
			delete pPackage;
			return nullptr;
		}

		return pPackage;
	}

	bool PackageEditor::saveEditable( Editable* pEditable )
	{
		PackageFile* pPackage = static_cast< PackageFile* >( pEditable );
		return pPackage->save();
	}

	void PackageEditor::closeEditable( Editable* pEditable )
	{
		PackageFile* pPackage = static_cast< PackageFile* >( pEditable );
		delete pPackage;
	}

	DynamicString PackageEditor::getPackageName() const
	{
		return m_currentPackageName;
	}

	//void PackageEditor::newClicked()
	//{
	//	QString fileName = QFileDialog::getSaveFileName(
	//		m_pInterface->getDialogParent(),
	//		m_pInterface->getDialogTitle(),
	//		m_pInterface->getContentPath().absolutePath(),
	//		"Package (*.package)"
	//	);

	//	m_pInterface->openFile( fileName );
	//}

	//void PackageEditor::openClicked()
	//{
	//	QString fileName = QFileDialog::getOpenFileName(
	//		m_pInterface->getDialogParent(),
	//		m_pInterface->getDialogTitle(),
	//		m_pInterface->getContentPath().absolutePath(),
	//		"Package (*.package)"
	//	);

	//	m_pInterface->openFile( fileName );
	//}

	//void PackageEditor::closeClicked()
	//{
	//	closePackage();
	//}

	//void PackageEditor::editClicked()
	//{
	//	m_pInterface->openFile( m_pInterface->getContentPath().absoluteFilePath( m_currentPackageName + ".package" ) );
	//}

	//void PackageEditor::openPackage( IFile* pPackageFile )
	//{
	//	QFileInfo fileInfo( pPackageFile->getFileName() );
	//	if( m_currentPackageName == fileInfo.baseName() )
	//	{
	//		return;
	//	}

	//	closePackage();

	//	m_pFileBrowser->openPackage( fileInfo.baseName() );
	//	m_pInterface->addGlobalDockWidget( m_pFileBrowser, Qt::LeftDockWidgetArea );

	//	m_currentPackageName = fileInfo.baseName();
	//}

	//void PackageEditor::closePackage()
	//{
	//	if( m_currentPackageName.isEmpty() )
	//	{
	//		return;
	//	}

	//	m_pInterface->closeAll();

	//	m_pInterface->removeGlobalDockWidget( m_pFileBrowser );
	//	m_pFileBrowser->closePakage();

	//	m_currentPackageName.clear();
	//}
}