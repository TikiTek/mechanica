#pragma once
#ifndef TIKI_PACKAGE_FILE_BROWSER_WIDGET_HPP_INCLUDED
#define TIKI_PACKAGE_FILE_BROWSER_WIDGET_HPP_INCLUDED

#include <QDir>
#include <QDockWidget>

class QStandardItem;
class QStandardItemModel;
class QTreeView;

namespace tiki
{
	class IEditorInterface;

	class PackageFileBrowserWidget : public QDockWidget
	{
		Q_OBJECT

	public:

		explicit			PackageFileBrowserWidget( IEditorInterface* pInterface );
		virtual				~PackageFileBrowserWidget();

		void				openPackage( const QString& packageName );
		void				closePakage();

	private slots:

		void 				doubleClicked( const QModelIndex &index );

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

#endif // TIKI_PACKAGE_FILE_BROWSER_WIDGET_HPP_INCLUDED
