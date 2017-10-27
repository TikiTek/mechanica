#include "tiki/package_editor/package_file_browser_widget.hpp"

#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>

namespace tiki
{
	PackageFileBrowserWidget::PackageFileBrowserWidget( IEditorInterface* pInterface )
		: QDockWidget( "Asset Browser" )
		, m_pInterface( pInterface )
		, m_folderIcon( ":/package-editor/browser-folder.png" )
		, m_unknownIcon( ":/package-editor/browser-file-unknown.png" )
	{
		m_pTreeModel = new QStandardItemModel();

		m_pTreeView = new QTreeView();
		m_pTreeView->setModel( m_pTreeModel );

		connect( m_pTreeView, &QAbstractItemView::doubleClicked, this, &PackageFileBrowserWidget::doubleClicked );

		setWidget( m_pTreeView );
	}

	PackageFileBrowserWidget::~PackageFileBrowserWidget()
	{
		delete m_pTreeView;
	}

	void PackageFileBrowserWidget::openPackage( const QString& packageName )
	{
		closePakage();
		m_packageName = packageName;

		m_pTreeModel->setHorizontalHeaderLabels( { "Filename" } );

		QStandardItem* pPackageItem = new QStandardItem( QIcon( ":/package-editor/browser-package.png" ), packageName );
		pPackageItem->setEditable( false );

		QDir contentDir( m_pInterface->getContentPath() );
		contentDir.cd( m_packageName );
		addFiles( pPackageItem, contentDir );

		m_pTreeModel->appendRow( pPackageItem );
		m_pTreeView->expand( m_pTreeModel->indexFromItem( pPackageItem ) );
	}

	void PackageFileBrowserWidget::closePakage()
	{
		if( m_packageName.isEmpty() )
		{
			return;
		}

		m_pTreeModel->clear();
	}

	void PackageFileBrowserWidget::doubleClicked( const QModelIndex &index )
	{
		QStandardItem* pItem = m_pTreeModel->itemFromIndex( index );
		if( pItem != nullptr )
		{
			m_pInterface->openFile( pItem->data().toString() );
		}
	}

	void PackageFileBrowserWidget::addFiles( QStandardItem* pParentItem, QDir dir )
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
				icon = pEditor->getEditableIcon();
			}

			QStandardItem* pItem = new QStandardItem( icon, entry );
			pItem->setData( filePath );
			pItem->setEditable( false );

			pParentItem->appendRow( pItem );

			if( isDir )
			{
				QDir subDir( dir );
				subDir.cd( entry );
				addFiles( pItem, subDir );

				m_pTreeView->expand( m_pTreeModel->indexFromItem( pItem ) );
			}
		}
	}
}
