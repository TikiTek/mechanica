
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
		m_pType = nullptr;
	}

	GenericDataObject::~GenericDataObject()
	{
	}

	bool GenericDataObject::create( const GenericDataTypeStruct* pType )
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

		m_pType = pType;

		const List< GenericDataStructField >& fields = pType->getFields();
		for (uint i = 0u; i < fields.getCount(); ++i)
		{
			const GenericDataStructField& field = fields[ i ];
			m_fields.set( field.name, field.defaultValue );
		}

		return true;
	}

	void GenericDataObject::dispose()
	{
		for (uint i = 0u; i < m_fields.getCount(); ++i)
		{
			Map< string, GenericDataValue >::Pair& kvp = m_fields.getPairAt( i );
			kvp.value.dispose();
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

	GenericDataValue& GenericDataObject::getFieldValue( const string& name )
	{
		return m_fields[ name ];
	}

	const GenericDataValue& GenericDataObject::getFieldValue( const string& name ) const
	{
		return m_fields[ name ];
	}

	bool GenericDataObject::setFieldValue( const string& name, const GenericDataValue& value )
	{
		if ( !hasField( name ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::setFieldValue] Struct of Type '%s' has no field named '%s'.\n", m_pType->getName().cStr(), name.cStr() );
			return false;
		}

		GenericDataValue& currentValue = m_fields[ name ];
		if( !currentValue.setValue( value ) )
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
			const GenericDataStructField& field = fields[ i ];
						
			GenericDataValue value;
			if ( !m_fields.findValue( &value, field.name ) )
			{
				value = field.defaultValue;
			}

			if ( value.getValueType() != GenericDataValueType_Invalid )
			{
				if ( !writeValueToResource( writer, value ) )
				{
					TIKI_TRACE_ERROR( "[GenericDataObject::writeToResource] Unable to write value for Field '%s'\n", field.name.cStr() );
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
}