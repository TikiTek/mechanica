#include "tiki/genericdataeditor/genericdataeditorwidget.hpp"

#include "tiki/toolgenericdata/genericdataarray.hpp"
#include "tiki/toolgenericdata/genericdataobject.hpp"
#include "tiki/toolgenericdata/genericdatatyperesource.hpp"

#include <QComboBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>

namespace tiki
{
	GenericDataEditorWidget::GenericDataEditorWidget( GenericDataTypeCollection& typeCollection )
		: m_typeCollection( typeCollection )
		, m_document( typeCollection )
	{
		m_pTreeModel = new QStandardItemModel();
		m_pTreeModel->setHorizontalHeaderLabels( { "Key", "Value", "Type" } );

		m_pTreeView = new QTreeView();
		m_pTreeView->setModel( m_pTreeModel );
		m_pTreeView->header()->setStretchLastSection( false );
		m_pTreeView->header()->setSectionResizeMode( 0, QHeaderView::Interactive );
		m_pTreeView->header()->setSectionResizeMode( 1, QHeaderView::Interactive );

		m_pLayout = new QHBoxLayout();
		m_pLayout->addWidget( m_pTreeView );
		setLayout( m_pLayout );
	}

	GenericDataEditorWidget::~GenericDataEditorWidget()
	{
		delete m_pTreeRootKeyItem;
		delete m_pTreeRootValueItem;
		delete m_pTreeRootTypeItem;
		delete m_pTreeModel;
		delete m_pTreeView;
	}

	bool GenericDataEditorWidget::openFile( IFile* pFile )
	{
		m_pFile = pFile;

		if( !m_document.importFromFile( pFile->getFileName().toUtf8().data() ) )
		{
			return false;
		}

		QFileInfo fileInfo( pFile->getFileName() );

		m_pTreeRootKeyItem = new QStandardItem( fileInfo.baseName() );
		m_pTreeRootValueItem = new QStandardItem();
		m_pTreeRootTypeItem = new QStandardItem( m_document.getType()->getName().cStr() );

		m_pTreeModel->appendRow( { m_pTreeRootKeyItem, m_pTreeRootValueItem, m_pTreeRootTypeItem } );

		generateItemsForObject( m_document.getObject(), m_pTreeRootKeyItem );
		m_pTreeView->expandAll();

		//m_pTreeView->setIndexWidget( m_pTreeModel->indexFromItem( m_pTreeRootItem ), new QComboBox() );

		return true;
	}

	bool GenericDataEditorWidget::saveFile()
	{
		return true;
	}

	void GenericDataEditorWidget::closeFile()
	{

	}

	void GenericDataEditorWidget::generateItemsForValue( const GenericDataValue& value, QStandardItem* pParentItem )
	{
		if( value.getValueType() == GenericDataValueType_Object )
		{
			GenericDataObject* pChildObject = nullptr;
			TIKI_VERIFY( value.getObject( pChildObject ) );

			generateItemsForObject( pChildObject, pParentItem );
		}
		else if( value.getValueType() == GenericDataValueType_Array )
		{
			GenericDataArray* pChildArray = nullptr;
			TIKI_VERIFY( value.getArray( pChildArray ) );

			generateItemsForArray( pChildArray, pParentItem );
		}
		else if( value.getValueType() == GenericDataValueType_Pointer )
		{
			GenericDataObject* pChildObject = nullptr;
			TIKI_VERIFY( value.getPointer( pChildObject ) );

			generateItemsForObject( pChildObject, pParentItem );
		}
	}

	void GenericDataEditorWidget::generateItemsForObject( GenericDataObject* pObject, QStandardItem* pParentItem )
	{
		for( uint i = 0u; i < pObject->getFieldCount(); ++i )
		{
			const string& key = pObject->getFieldName( i );
			const GenericDataType* pType = pObject->getFieldType( i );
			GenericDataValue value = pObject->getFieldValue( i );

			QStandardItem* pKeyItem = new QStandardItem( key.cStr() );
			QStandardItem* pValueItem = new QStandardItem( "Test" );
			QStandardItem* pTypeItem = new QStandardItem( pType->getName().cStr() );

			generateItemsForValue( value, pKeyItem );

			pParentItem->appendRow( { pKeyItem, pValueItem, pTypeItem } );
		}
	}

	void GenericDataEditorWidget::generateItemsForArray( GenericDataArray* pArray, QStandardItem* pParentItem )
	{
		for( uint i = 0u; i < pArray->getCount(); ++i )
		{
			GenericDataValue value = pArray->getElement( i );

			QStandardItem* pKeyItem = new QStandardItem( QString( "%0" ).arg( i ) );
			QStandardItem* pValueItem = new QStandardItem( "Test" );
			QStandardItem* pTypeItem = new QStandardItem( value.getType()->getName().cStr() );

			generateItemsForValue( value, pKeyItem );

			pParentItem->appendRow( { pKeyItem, pValueItem, pTypeItem } );
		}
	}
}