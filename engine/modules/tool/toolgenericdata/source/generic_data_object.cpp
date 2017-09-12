#include "tiki/toolgenericdata/generic_data_object.hpp"

#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/generic_data_type_collection.hpp"
#include "tiki/toolgenericdata/generic_data_type_struct.hpp"
#include "tiki/toolgenericdata/generic_data_value.hpp"

#if TIKI_ENABLED( TIKI_GENERICDATA_CONVERTER )
#	include "tiki/converterbase/resourcewriter.hpp"
#endif

namespace tiki
{
	GenericDataObject::GenericDataObject( GenericDataTypeCollection& collection )
		: GenericDataContainer( collection )
	{
		m_pType			= nullptr;
		m_pParentObject	= nullptr;
	}

	GenericDataObject::~GenericDataObject()
	{
	}

	bool GenericDataObject::create( const GenericDataTypeStruct* pType, const GenericDataObject* pParentObject )
	{
		if ( pType == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::create] Null type not allowed.\n" );
			return false;
		}

		if ( m_pType != nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::create] Dublicate create is not allowed.\n" );
			return false;
		}

		m_pType			= pType;
		m_pParentObject = pParentObject;
		if( m_pParentObject == nullptr )
		{
			m_pParentObject = pType->getDefaultObject();
		}

		return true;
	}

	void GenericDataObject::dispose()
	{
		for (uint i = 0u; i < m_fields.getCount(); ++i)
		{
			ObjectField& field = m_fields.getValueAt( i );
			TIKI_DELETE( field.pValue );
		}
		m_fields.clear();
	}

	const GenericDataTypeStruct* GenericDataObject::getType() const
	{
		return m_pType;
	}

	bool GenericDataObject::hasField( const string& name ) const
	{
		return m_fields.hasKey( name );
	}

	const string& GenericDataObject::getFieldName( uint index ) const
	{
		return m_fields.getPairAt( index ).key;
	}

	const GenericDataType* GenericDataObject::getFieldType( uint index ) const
	{
		return m_fields.getValueAt( index ).pType;
	}

	GenericDataValue* GenericDataObject::getFieldValue( const string& name, bool createMissing )
	{
		if( m_fields.hasKey( name ) )
		{
			return m_fields[ name ].pValue;
		}
		else if( createMissing )
		{
			const GenericDataType* pFieldType = m_pType->getFieldTypeByName( name );
			if( pFieldType == nullptr )
			{
				return nullptr;
			}

			ObjectField field;
			field.pType		= pFieldType;
			field.pValue	= TIKI_NEW( GenericDataValue )( pFieldType );
			m_fields.set( name, field );

			return field.pValue;
		}
		else
		{
			return nullptr;
		}
	}

	const GenericDataValue* GenericDataObject::getFieldOrDefaultValue( const string& name ) const
	{
		ObjectField field;
		field.pValue = nullptr;

		if( (!m_fields.findValue( &field, name ) || field.pValue == nullptr) && m_pParentObject != nullptr )
		{
			return m_pParentObject->getFieldOrDefaultValue( name );
		}

		return field.pValue;
	}

	GenericDataValue* GenericDataObject::getFieldValue( uint index ) const
	{
		return m_fields.getValueAt( index ).pValue;
	}

	bool GenericDataObject::removeField( const string& name )
	{
		if( !m_fields.hasKey( name ) )
		{
			return false;
		}

		ObjectField& field = m_fields[ name ];
		if( field.pValue != nullptr )
		{
			TIKI_DELETE( field.pValue );
		}

		m_fields.remove( name );
		return true;
	}

	bool GenericDataObject::writeToResource( ReferenceKey* pDataKey, ResourceWriter& writer ) const
	{
#if TIKI_ENABLED( TIKI_GENERICDATA_CONVERTER )
		if ( pDataKey != nullptr )
		{
			writer.openDataSection( 0u, AllocatorType_MainMemory, getType()->getAlignment() );
			*pDataKey = writer.addDataPoint();
		}

		bool ok = true;

		const List< GenericDataStructField >& fields = m_pType->getFields();
		for (uint i = 0u; i < fields.getCount(); ++i)
		{
			const GenericDataStructField& structField = fields[ i ];

			ObjectField field;
			const GenericDataValue* pValue = nullptr;
			if ( !m_fields.findValue( &field, structField.name ) )
			{
				if( m_pParentObject != nullptr )
				{
					pValue = m_pParentObject->getFieldOrDefaultValue( structField.name );
				}
			}
			else
			{
				TIKI_ASSERT( field.pValue != nullptr );
				pValue = field.pValue;
			}

			if ( pValue != nullptr )
			{
				TIKI_ASSERT( pValue->getType() == structField.pType );
				if ( !writeValueToResource( writer, *pValue ) )
				{
					TIKI_TRACE_ERROR( "[GenericDataObject::writeToResource] Unable to write value for Field '%s'\n", structField.name.cStr() );
					ok = false;
				}
			}
			else
			{
				ok = false;
			}
		}

		if ( pDataKey != nullptr )
		{
			writer.closeDataSection();
		}

		return ok;
#else
		return false;
#endif
	}

	bool GenericDataObject::initializeXmlElementForValue( XmlElement* pElement, const GenericDataValue* pValue ) const
	{
		for( uint i = 0u; i < m_fields.getCount(); ++i )
		{
			const FieldMap::Pair& kvp = m_fields.getPairAt( i );
			if( kvp.value.pValue != pValue )
			{
				continue;
			}

			pElement->setAttribute( "name", kvp.key.cStr() );
			return true;
		}

		TIKI_TRACE_ERROR( "[GenericDataObject::initializeXmlElementForValue] Field for value '%s' not found.\n", pValue->toString().cStr() );
		return false;
	}

	const char* GenericDataObject::getNodeName() const
	{
		return "object";
	}

	const char* GenericDataObject::getElementName() const
	{
		return "field";
	}

	const GenericDataType* GenericDataObject::getParentType() const
	{
		return m_pType;
	}

	GenericDataValue* GenericDataObject::addElementValue( const XmlElement* pNode )
	{
		const XmlAttribute* pNameAtt = pNode->findAttribute( "name" );
		if ( pNameAtt == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::importFromXml] 'field' node needs a 'name' attribute.\n" );
			return false;
		}

		return getFieldValue( pNameAtt->getValue(), true );
	}

	GenericDataValue* GenericDataObject::getElementValue( uint index )
	{
		if( index >= m_fields.getCount() )
		{
			return nullptr;
		}

		return getFieldValue( index );
	}
}