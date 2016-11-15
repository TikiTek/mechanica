#pragma once
#ifndef TIKI_PACKAGEFILEBROWSER_HPP_INCLUDED
#define TIKI_PACKAGEFILEBROWSER_HPP_INCLUDED

#include <QDir>
#include <QDockWidget>

class QStandardItem;
class QStandardItemModel;
class QTreeView;

namespace tiki
{
	class IEditorInterface;

	class PackageFileBrowser : public QDockWidget
	{
		Q_OBJECT

	public:

		explicit		PackageFileBrowser( IEditorInterface* pInterface );
		virtual			~PackageFileBrowser();

		void			openPackage( const QString& packageName );
		void			closePakage();

	private:

		IEditorInterface*	m_pInterface;
		QString				m_packageName;

		QTreeView*			m_pTreeView;
		QStandardItemModel*	m_pTreeModel;

		QIcon				m_folderIcon;
		QIcon				m_unknownIcon;

		void				addFiles( QStandardItem* pParentItem, QDir dir );
	};
}

#endif // TIKI_PACKAGEFILEBROWSER_HPP_INCLUDED
