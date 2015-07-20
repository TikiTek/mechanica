
#include "tiki/toolgenericdata/genericdataobject.hpp"

#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/genericdatatypecollection.hpp"
#include "tiki/toolgenericdata/genericdatatypestruct.hpp"
#include "tiki/toolgenericdata/genericdatavalue.hpp"

namespace tiki
{
	GenericDataObject::GenericDataObject( GenericDataTypeCollection& collection )
		: m_collection( collection )
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

	bool GenericDataObject::importFromXml( XmlReader& reader, const _XmlElement* pObjectRootNode )
	{
		bool result = true;

		const XmlElement* pFieldElement = reader.findFirstChild( "field", pObjectRootNode );
		while ( pFieldElement )
		{
			const XmlAttribute* pNameAtt = reader.findAttributeByName( "name", pFieldElement );
			const XmlAttribute* pTypeAtt = reader.findAttributeByName( "type", pFieldElement );

			if ( pNameAtt == nullptr || pTypeAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataObject::importFromXml] 'field' node needs 'name' and 'type' attribute.\n" );
				result = false;
				continue;
			}

			const GenericDataType* pFieldType = m_collection.parseType( pTypeAtt->content );
			if ( pFieldType == nullptr )
			{
				result = false;
				continue;
			}

			GenericDataValue value;
			const XmlAttribute* pValueAtt = reader.findAttributeByName( "value", pFieldElement );
			if ( pValueAtt != nullptr )
			{
				if ( !m_collection.parseValue( value, pValueAtt->content, pFieldType ) )
				{
					result = false;
					continue;
				}
			}
			else
			{
				const XmlElement* pChildElement = pFieldElement->elements;
				if ( pChildElement == nullptr )
				{
					TIKI_TRACE_ERROR( "[GenericDataObject::importFromXml] 'field' node needs a 'value' attribute or a child node named 'object' or 'array'. Fieldname: %s\n", pNameAtt->content );
					result = false;
					continue;
				}

				const XmlAttribute* pChildTypeAtt = reader.findAttributeByName( "type", pChildElement );
				if ( pChildTypeAtt == nullptr )
				{
					TIKI_TRACE_ERROR( "[GenericDataObject::importFromXml] child node needs a 'type' attribute.\n" );
					result = false;
					continue;
				}

				const GenericDataType* pChildType = m_collection.parseType( pChildTypeAtt->content );
				if ( pChildType == nullptr )
				{
					result = false;
					continue;
				}

				const string childName = pChildElement->name;
				if ( childName == "object" )
				{
					if ( pChildType->getType() != GenericDataTypeType_Struct )
					{
						TIKI_TRACE_ERROR( "[GenericDataObject::importFromXml] type of a object needs to be a struct. '%s' is not a struct.\n", pChildType->getName().cStr() );
						result = false;
						continue;
					}

					const GenericDataTypeStruct* pTypedChildType = (const GenericDataTypeStruct*)pChildType;
					GenericDataObject* pObject = TIKI_MEMORY_NEW_OBJECT( GenericDataObject )( m_collection );
					if ( !pObject->create( pTypedChildType ) )
					{
						TIKI_MEMORY_DELETE_OBJECT( pObject );
						result = false;
						continue;
					}

					if ( !pObject->importFromXml( reader, pChildElement ) )
					{
						TIKI_MEMORY_DELETE_OBJECT( pObject );
						result = false;
						continue;
					}

					value.setObject( pObject );
				}
				else if ( childName == "array" )
				{

				}
				else 
				{
					TIKI_TRACE_ERROR( "[GenericDataObject::importFromXml] child node has a invalid name(%s).\n", pChildElement->name );
					result = false;
					continue;
				}
			}

			pFieldElement = reader.findNext( "field", pFieldElement );
		}

		return true; 
	}

	bool GenericDataObject::exportToResource( ResourceWriter& writer ) const
	{
		return true;
	}
}