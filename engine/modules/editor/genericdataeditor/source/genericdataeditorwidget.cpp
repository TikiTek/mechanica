#include "genericdataeditorwidget.hpp"

#include "tiki/toolgenericdata/generic_data_array.hpp"
#include "tiki/toolgenericdata/generic_data_object.hpp"
#include "tiki/toolgenericdata/generic_data_type_array.hpp"
#include "tiki/toolgenericdata/generic_data_type_resource.hpp"
#include "tiki/toolgenericdata/genericdatatypestruct.hpp"

#include "genericdataboolvaluewidget.hpp"
#include "generic_data_value_widget.hpp"

#include <QComboBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QSettings>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>

namespace tiki
{
	GenericDataEditorWidget::GenericDataEditorWidget( GenericDataTypeCollection& typeCollection )
		: m_typeCollection( typeCollection )
		, m_document( typeCollection )
	{
		m_pTreeRootKeyItem		= nullptr;
		m_pTreeRootValueItem	= nullptr;
		m_pTreeRootTypeItem		= nullptr;

		m_pTreeModel = new QStandardItemModel();
		m_pTreeModel->setHorizontalHeaderLabels( { "Key", "Value", "Type" } );

		m_pTreeView = new QTreeView();
		m_pTreeView->setModel( m_pTreeModel );
		m_pTreeView->header()->setStretchLastSection( false );

		m_pLayout = new QHBoxLayout();
		m_pLayout->addWidget( m_pTreeView );
		setLayout( m_pLayout );

		QSettings settings( "TikiTek", "editor" );
		m_pTreeView->header()->restoreState( settings.value( "generic_data/geometry" ).toByteArray() );
	}

	GenericDataEditorWidget::~GenericDataEditorWidget()
	{
		QSettings settings( "TikiTek", "editor" );
		settings.setValue( "generic_data/geometry", m_pTreeView->header()->saveState() );

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
		return m_document.exportToFile( m_pFile->getFileName() .toUtf8().data() );
	}

	void GenericDataEditorWidget::closeFile()
	{
		m_pTreeModel->clear();

		delete m_pTreeRootKeyItem;
		delete m_pTreeRootValueItem;
		delete m_pTreeRootTypeItem;
		m_pTreeRootKeyItem		= nullptr;
		m_pTreeRootValueItem	= nullptr;
		m_pTreeRootTypeItem		= nullptr;

		m_document.dispose();
		m_pFile = nullptr;
	}

	void GenericDataEditorWidget::generateItemsForValue( const GenericDataValue* pValue, QStandardItem* pParentItem )
	{
		if( pValue->getValueType() == GenericDataValueType_Object )
		{
			GenericDataObject* pChildObject = nullptr;
			TIKI_VERIFY( pValue->getObject( pChildObject ) );

			if( pChildObject != nullptr )
			{
				generateItemsForObject( pChildObject, pParentItem );
			}
		}
		else if( pValue->getValueType() == GenericDataValueType_Array )
		{
			GenericDataArray* pChildArray = nullptr;
			TIKI_VERIFY( pValue->getArray( pChildArray ) );

			if( pChildArray != nullptr )
			{
				generateItemsForArray( pChildArray, pParentItem );
			}
		}
		else if( pValue->getValueType() == GenericDataValueType_Pointer )
		{
			GenericDataObject* pChildObject = nullptr;
			TIKI_VERIFY( pValue->getPointer( pChildObject ) );

			if( pChildObject != nullptr )
			{
				generateItemsForObject( pChildObject, pParentItem );
			}
		}
	}

	void GenericDataEditorWidget::generateItemsForObject( GenericDataObject* pObject, QStandardItem* pParentItem )
	{
		const GenericDataTypeStruct* pObjectType = pObject->getType();
		const List< GenericDataStructField >& fields = pObjectType->getFields();
		for( uint i = 0u; i < fields.getCount(); ++i )
		{
			const GenericDataStructField& field = fields[ i ];

			GenericDataValue* pValue = pObject->getFieldValue( field.name, false );
			const GenericDataValue* pDefaultValue = pValue;
			if( pDefaultValue == nullptr )
			{
				pDefaultValue = pObject->getFieldOrDefaultValue( field.name );
			}

			if( pDefaultValue == nullptr )
			{
				pDefaultValue = field.pType->getDefaultValue();
			}

			QStandardItem* pItem = createItemForValueType( field.name.cStr(), field.pType, pValue, pDefaultValue, pParentItem );
			generateItemsForValue( pDefaultValue, pItem );
		}
	}

	void GenericDataEditorWidget::generateItemsForArray( GenericDataArray* pArray, QStandardItem* pParentItem )
	{
		for( uint i = 0u; i < pArray->getCount(); ++i )
		{
			GenericDataValue* pValue = pArray->getElement( i );

			QStandardItem* pItem = createItemForValueType( QString( "%0" ).arg( i ), pValue->getType(), pValue, nullptr, pParentItem );
			generateItemsForValue( pValue, pItem );
		}
	}

	QStandardItem* GenericDataEditorWidget::createItemForValueType( const QString& key, const GenericDataType* pType, GenericDataValue* pValue, const GenericDataValue* pDefaultValue, QStandardItem* pParentItem )
	{
		QStandardItem* pKeyItem = new QStandardItem( key );
		QStandardItem* pValueItem = new QStandardItem();
		QStandardItem* pTypeItem = new QStandardItem( pType->getName().cStr() );
		pParentItem->appendRow( { pKeyItem, pValueItem, pTypeItem } );

		QWidget* pWidget = createWidgetForValueType( pType, pValue, pDefaultValue );
		if( pWidget != nullptr )
		{
			m_pTreeView->setIndexWidget( m_pTreeModel->indexFromItem( pValueItem ), pWidget );
		}

		return pKeyItem;
	}

	QWidget* GenericDataEditorWidget::createWidgetForValueType( const GenericDataType* pType, GenericDataValue* pValue, const GenericDataValue* pDefaultValue )
	{
		if( pValue == nullptr )
		{
			return nullptr;
		}

		TIKI_ASSERT( pValue->getValueType() != GenericDataValueType_Invalid );
		switch( pValue->getValueType() )
		{
		case GenericDataValueType_Boolean:
			return new GenericDataBoolValueWidget( m_pFile, pValue );

		case GenericDataValueType_SingedInteger8:
		case GenericDataValueType_SingedInteger16:
		case GenericDataValueType_SingedInteger32:
		case GenericDataValueType_SingedInteger64:
		case GenericDataValueType_UnsingedInteger8:
		case GenericDataValueType_UnsingedInteger16:
		case GenericDataValueType_UnsingedInteger32:
		case GenericDataValueType_UnsingedInteger64:
		case GenericDataValueType_FloatingPoint16:
		case GenericDataValueType_FloatingPoint32:
		case GenericDataValueType_FloatingPoint64:
			return new GenericDataValueWidget( m_typeCollection, m_pFile, pValue );

		case GenericDataValueType_String:
			return nullptr;

		case GenericDataValueType_Enum:
			return nullptr;

		case GenericDataValueType_Reference:
			return nullptr;

		case GenericDataValueType_Pointer:
		case GenericDataValueType_Object:
		case GenericDataValueType_Array:
		case GenericDataValueType_Invalid:
		case GenericDataValueType_Count:
			return nullptr;
		}

		return nullptr;
	}
}