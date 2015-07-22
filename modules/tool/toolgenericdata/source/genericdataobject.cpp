
#include "tiki/toolgenericdata/genericdataobject.hpp"

#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/genericdatatypecollection.hpp"
#include "tiki/toolgenericdata/genericdatatypestruct.hpp"
#include "tiki/toolgenericdata/genericdatavalue.hpp"

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
			m_fields.set( field.name, GenericDataValue( field.pType ) );
		}

		return true;
	}

	void GenericDataObject::dispose()
	{

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
			TIKI_TRACE_ERROR( "[GenericDataObject::setFieldValue] Struct of Type '%s' has no field named '&s'.\n", m_pType->getName().cStr(), name.cStr() );
			return false;
		}

		GenericDataValue& currentValue = m_fields[ name ];
		if ( currentValue.getType() != value.getType() )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::setFieldValue] Can't assign value with different type('%s' != '%s') to field '%s'!\n", value.getType()->getName().cStr(), currentValue.getType()->getName().cStr(), name.cStr() );
			return false;
		}

		currentValue = value;
		return true;
	}

	bool GenericDataObject::exportToResource( ResourceWriter& writer ) const
	{
		return true;
	}

	const char* GenericDataObject::getElementName() const
	{
		return "field";
	}

	bool GenericDataObject::applyElementValue( XmlReader& reader, const _XmlElement* pElement, const GenericDataValue& value )
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