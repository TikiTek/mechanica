#include "tiki/packageeditor/packagefilebrowser.hpp"

#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>

namespace tiki
{
	PackageFileBrowser::PackageFileBrowser( IEditorInterface* pInterface )
		: QDockWidget( "Asset Browser" )
		, m_pInterface( pInterface )
		, m_folderIcon( ":/package-editor/browser-folder.png" )
		, m_unknownIcon( ":/package-editor/browser-file-unknown.png" )
	{
		m_pTreeModel = new QStandardItemModel();

		m_pTreeView = new QTreeView();
		m_pTreeView->setModel( m_pTreeModel );

		setWidget( m_pTreeView );
	}

	PackageFileBrowser::~PackageFileBrowser()
	{
		delete m_pTreeView;
	}

	void PackageFileBrowser::openPackage( const QString& packageName )
	{
		closePakage();
		m_packageName = packageName;

		m_pTreeModel->setHorizontalHeaderLabels( { "Filename" } );

		QStandardItem* pPacketItem = new QStandardItem( QIcon( ":/package-editor/browser-package.png" ), packageName );

		QDir contentDir( m_pInterface->getContentPath() );
		contentDir.cd( m_packageName );
		addFiles( pPacketItem, contentDir );

		m_pTreeModel->appendRow( pPacketItem );
		m_pTreeView->expand( m_pTreeModel->indexFromItem( pPacketItem ) );
	}

	void PackageFileBrowser::closePakage()
	{
		if( m_packageName.isEmpty() )
		{
			return;
		}

		m_pTreeModel->clear();
	}

	void PackageFileBrowser::addFiles( QStandardItem* pParentItem, QDir dir )
	{
		foreach( QString entry, dir.entryList( QDir::NoFilter, QDir::DirsFirst ) )
		{
			if( entry[ 0u ] == '.' )
			{
				continue;
			}

			QString filePath = dir.absoluteFilePath( entry );
			QFileInfo fileInfo( filePath );


			const bool isDir = fileInfo.isDir();
			QIcon icon = (isDir ? m_folderIcon : m_unknownIcon);

			IFileEditor* pEditor = m_pInterface->findEditorForFile( filePath );
			if( pEditor != nullptr )
			{
				icon = pEditor->getFileIcon();
			}

			QStandardItem* pItem = new QStandardItem( icon, entry );
			pParentItem->appendRow( pItem );

			if( isDir )
			{
				QDir subDir( dir );
				subDir.cd( entry );
				addFiles( pItem, subDir );
			}
		}
	}
}
