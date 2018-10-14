#pragma once

namespace tiki
{
	class GenericDataTag;

	class GenericDataTagEditor
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTagEditor );

	public:

		GenericDataTagEditor( GenericDataValue* pValue );
		~GenericDataTagEditor();

	private:

		GenericDataValue*	m_pValue;
		GenericDataTag*		m_pTag;
		GenericDataTag*		m_pWorkingTag;

		//QPushButton*		m_pOkButton;
		//QPushButton*		m_pCancelButton;

		//QMenu*				m_pItemMenu;
		//QPushButton*		m_pAddButton;

		//QTreeView*			m_pTreeView;
		//QStandardItemModel*	m_pTreeModel;
		//QStandardItem*		m_pTreeRootTagItem;
		//QStandardItem*		m_pTreeRootValueItem;
	};
}
