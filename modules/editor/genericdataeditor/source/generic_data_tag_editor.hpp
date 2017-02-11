#pragma once
#ifndef TIKI_GENERICDATAVALUETAGEDITOR_HPP_INCLUDED
#define TIKI_GENERICDATAVALUETAGEDITOR_HPP_INCLUDED

#include <QWindow>

class QMenu;
class QPushButton;
class QTreeView;

namespace tiki
{
	class GenericDataTag;

	class GenericDataTagEditor : public QWindow
	{
		Q_OBJECT

	public:

		GenericDataTagEditor( QWindow* pParent, GenericDataValue* pValue );
		~GenericDataTagEditor();

	private:

		GenericDataValue*	m_pValue;
		GenericDataTag*		m_pTag;
		GenericDataTag*		m_pWorkingTag;

		QPushButton*		m_pOkButton;
		QPushButton*		m_pCancelButton;

		QMenu*				m_pItemMenu;
		QPushButton*		m_pAddButton;

		QTreeView*			m_pTreeView;
		QStandardItemModel*	m_pTreeModel;
		QStandardItem*		m_pTreeRootTagItem;
		QStandardItem*		m_pTreeRootValueItem;
	};
}

#endif // TIKI_GENERICDATAVALUETAGEDITOR_HPP_INCLUDED
