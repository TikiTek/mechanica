
#include "tiki/toolgenericdata/genericdataobject.hpp"

#include "tiki/toolgenericdata/genericdatatypestruct.hpp"
#include "tiki/toolgenericdata/genericdatavalue.hpp"

namespace tiki
{
	GenericDataObject::GenericDataObject()
	{
	}

	GenericDataObject::~GenericDataObject()
	{
	}

	bool GenericDataObject::createFromType( const GenericDataTypeStruct* pType )
	{
		if ( pType == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::createFromType] Null type not allowed.\n" );
			return false;
		}

		const List< GenericDataStructField >& fields = pType->getFields();
		for (uint i = 0u; i < fields.getCount(); ++i)
		{
			const GenericDataStructField& field = fields[ i ];
			m_fields.set( field.name, GenericDataValue( field.pType ) );
		}

		return true;
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
		GenericDataValue& currentValue = m_fields[ name ];
		if ( currentValue.getType() != value.getType() )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::setFieldValue] Can't assign value with different type!\n" );
			return false;
		}

		currentValue = value;
		return true;
	}

	bool GenericDataObject::importFromXml( XmlReader& reader, const _XmlElement* pObjectRootNode )
	{
		return true; 
	}

	bool GenericDataObject::exportToResource( ResourceWriter& writer ) const
	{
		return true;
	}
}