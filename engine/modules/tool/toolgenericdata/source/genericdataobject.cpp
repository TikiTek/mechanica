
#include "tiki/toolgenericdata/genericdataobject.hpp"

#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/genericdatatypecollection.hpp"
#include "tiki/toolgenericdata/genericdatatypestruct.hpp"
#include "tiki/toolgenericdata/genericdatavalue.hpp"

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

		const List< GenericDataStructField >& fields = pType->getFields();
		for (uint i = 0u; i < fields.getCount(); ++i)
		{
			const GenericDataStructField& structField = fields[ i ];

			ObjectField field;
			field.pType = structField.pType;
			field.value = InvalidGenericDataValue;

			m_fields.set( structField.name, field );
		}

		return true;
	}

	void GenericDataObject::dispose()
	{
		for (uint i = 0u; i < m_fields.getCount(); ++i)
		{
			ObjectField& field = m_fields.getValueAt( i );
			field.value.dispose();
		}
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

	GenericDataValue GenericDataObject::getFieldValue( const string& name ) const
	{
		const ObjectField& field = m_fields[ name ];
		if( !field.value.isValid() )
		{
			if( m_pParentObject != nullptr )
			{
				return m_pParentObject->getFieldValue( name );
			}
			else
			{
				return GenericDataValue( field.pType );
			}
		}

		return field.value;
	}

	GenericDataValue GenericDataObject::getFieldValue( uint index ) const
	{
		return m_fields.getValueAt( index ).value;
	}

	bool GenericDataObject::setFieldValue( const string& name, const GenericDataValue& value )
	{
		if ( !hasField( name ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::setFieldValue] Struct of Type '%s' has no field named '%s'.\n", m_pType->getName().cStr(), name.cStr() );
			return false;
		}

		ObjectField& field = m_fields[ name ];
		if( !field.value.setValue( value ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::setFieldValue] Can't assign value to field '%s'!\n", name.cStr() );
			return false;
		}

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
			GenericDataValue value;
			if ( !m_fields.findValue( &field, structField.name ) || !field.value.isValid() )
			{
				if( m_pParentObject != nullptr )
				{
					value = m_pParentObject->getFieldValue( structField.name );
				}
				else
				{
					value = GenericDataValue( field.pType );
				}
			}
			else
			{
				value = field.value;
			}

			if ( value.getValueType() != GenericDataValueType_Invalid )
			{
				TIKI_ASSERT( value.getType() == structField.pType );
				if ( !writeValueToResource( writer, value ) )
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

	const char* GenericDataObject::getElementName() const
	{
		return "field";
	}

	const GenericDataType* GenericDataObject::getParentType() const
	{
		return m_pType;
	}

	bool GenericDataObject::applyElementValue( const XmlReader& reader, const _XmlElement* pElement, const GenericDataValue& value )
	{
		const XmlAttribute* pNameAtt = reader.findAttributeByName( "name", pElement );
		if ( pNameAtt == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::importFromXml] 'field' node needs a 'name' attribute.\n" );
			return false;
		}

		return setFieldValue( pNameAtt->content, value );
	}

	void GenericDataObject::addField( const string& name, const GenericDataType* pType, const GenericDataValue& defaultValue )
	{
		ObjectField field;
		field.pType = pType;
		field.value = defaultValue;

		m_fields.set( name, field );
	}
}