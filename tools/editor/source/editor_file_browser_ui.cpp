#include "editor_file_browser_ui.hpp"

namespace tiki
{
	EditorFileBrowserUi::EditorFileBrowserUi( EditorInterface* pInterface )
		: m_pInterface( pInterface )
	{
		TIKI_VERIFY( m_folderIcon.createFromFile( "package-editor/browser-folder.png" ) );
		TIKI_VERIFY( m_unknownIcon.createFromFile( "package-editor/browser-file-unknown.png" ) );
	}

	EditorFileBrowserUi::~EditorFileBrowserUi()
	{
	}

	void EditorFileBrowserUi::openPackage( const DynamicString& packageName )
	{
		closePakage();
		m_packageName = packageName;

		//m_pTreeModel->setHorizontalHeaderLabels( { "Filename" } );

		//QStandardItem* pPackageItem = new QStandardItem( QIcon( ":/package-editor/browser-package.png" ), packageName );
		//pPackageItem->setEditable( false );

		//QDir contentDir( m_pInterface->getContentPath() );
		//contentDir.cd( m_packageName );
		//addFiles( pPackageItem, contentDir );

		//m_pTreeModel->appendRow( pPackageItem );
		//m_pTreeView->expand( m_pTreeModel->indexFromItem( pPackageItem ) );
	}

	void EditorFileBrowserUi::closePakage()
	{
		//if( m_packageName.isEmpty() )
		//{
		//	return;
		//}

		//m_pTreeModel->clear();
		m_packageName.clear();
	}

	//void EditorFileBrowserUi::doubleClicked( const QModelIndex &index )
	//{
	//	QStandardItem* pItem = m_pTreeModel->itemFromIndex( index );
	//	if( pItem != nullptr )
	//	{
	//		m_pInterface->openFile( pItem->data().toString() );
	//	}
	//}

	//void EditorFileBrowserUi::addFiles( QStandardItem* pParentItem, QDir dir )
	//{
	//	foreach( QString entry, dir.entryList( QDir::NoFilter, QDir::DirsFirst ) )
	//	{
	//		if( entry[ 0u ] == '.' )
	//		{
	//			continue;
	//		}

	//		QString filePath = dir.absoluteFilePath( entry );
	//		QFileInfo fileInfo( filePath );


	//		const bool isDir = fileInfo.isDir();
	//		QIcon icon = (isDir ? m_folderIcon : m_unknownIcon);

	//		IFileEditor* pEditor = m_pInterface->findEditorForFile( filePath );
	//		if( pEditor != nullptr )
	//		{
	//			icon = pEditor->getEditableIcon();
	//		}

	//		QStandardItem* pItem = new QStandardItem( icon, entry );
	//		pItem->setData( filePath );
	//		pItem->setEditable( false );

	//		pParentItem->appendRow( pItem );

	//		if( isDir )
	//		{
	//			QDir subDir( dir );
	//			subDir.cd( entry );
	//			addFiles( pItem, subDir );

	//			m_pTreeView->expand( m_pTreeModel->indexFromItem( pItem ) );
	//		}
	//	}
	//}
}
