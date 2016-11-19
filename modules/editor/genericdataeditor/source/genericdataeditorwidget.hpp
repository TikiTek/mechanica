#pragma once
#ifndef TIKI_GENERICDATAEDITORWIDGET_HPP_INCLUDED
#define TIKI_GENERICDATAEDITORWIDGET_HPP_INCLUDED

#include <QWidget>

#include "tiki/toolgenericdata/genericdatadocument.hpp"

class QHBoxLayout;
class QStandardItem;
class QStandardItemModel;
class QTreeView;

namespace tiki
{
	class GenericDataArray;

	class GenericDataEditorWidget : public QWidget
	{
		Q_OBJECT

	public:

		explicit			GenericDataEditorWidget( GenericDataTypeCollection& typeCollection );
		virtual				~GenericDataEditorWidget();

		bool				openFile( IFile* pFile );
		bool				saveFile();
		void				closeFile();

	private:

		GenericDataTypeCollection&	m_typeCollection;

		IFile*						m_pFile;
		GenericDataDocument			m_document;

		QHBoxLayout*				m_pLayout;
		QTreeView*					m_pTreeView;
		QStandardItemModel*			m_pTreeModel;
		QStandardItem*				m_pTreeRootKeyItem;
		QStandardItem*				m_pTreeRootValueItem;
		QStandardItem*				m_pTreeRootTypeItem;

		void						generateItemsForValue( const GenericDataValue* pValue, QStandardItem* pParentItem );
		void						generateItemsForObject( GenericDataObject* pObject, QStandardItem* pParentItem );
		void						generateItemsForArray( GenericDataArray* pArray, QStandardItem* pParentItem );

		QStandardItem*				createItemForValueType( const QString& key, const GenericDataType* pType, GenericDataValue* pValue, const GenericDataValue* pDefaultValue, QStandardItem* pParentItem );
		QWidget*					createWidgetForValueType( const GenericDataType* pType, GenericDataValue* pValue, const GenericDataValue* pDefaultValue );
	};
}

#endif // TIKI_GENERICDATAEDITORWIDGET_HPP_INCLUDED
