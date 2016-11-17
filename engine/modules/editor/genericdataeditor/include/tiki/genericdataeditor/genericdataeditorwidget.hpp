#pragma once
#ifndef TIKI_GENERICDATAEDITORWIDGET_HPP_INCLUDED
#define TIKI_GENERICDATAEDITORWIDGET_HPP_INCLUDED

#include <QWidget>

class QHBoxLayout;
class QStandardItem;
class QStandardItemModel;
class QTreeView;

namespace tiki
{
	class GenericDataEditorWidget : public QWidget
	{
		Q_OBJECT

	public:

		explicit			GenericDataEditorWidget();
		virtual				~GenericDataEditorWidget();

		bool				openFile( IFile* pFile );
		bool				saveFile();
		void				closeFile();

	private:

		QHBoxLayout*		m_pLayout;
		QTreeView*			m_pTreeView;
		QStandardItemModel*	m_pTreeModel;
		QStandardItem*		m_pTreeRootItem;
	};
}

#endif // TIKI_GENERICDATAEDITORWIDGET_HPP_INCLUDED
