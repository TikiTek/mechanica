
#include "tiki/toolgenericdata/genericdataarray.hpp"

#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/genericdataobject.hpp"
#include "tiki/toolgenericdata/genericdatatypearray.hpp"
#include "tiki/toolgenericdata/genericdatatypecollection.hpp"
#include "tiki/toolgenericdata/genericdatatypestruct.hpp"
#include "tiki/toolgenericdata/genericdatavalue.hpp"

namespace tiki
{
	GenericDataArray::GenericDataArray( GenericDataTypeCollection& collection )
		: m_collection( collection )
	{
		m_pType = nullptr;
	}

	GenericDataArray::~GenericDataArray()
	{
	}

	bool GenericDataArray::create( const GenericDataTypeArray* pType )
	{
		if ( pType == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataArray::createFromType] Null type not allowed.\n" );
			return false;
		}

		if ( m_pType != nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataArray::create] Dublicate create is not allowed.\n" );
			return false;
		}
	

		m_pType	= pType;

		return true;
	}

	void GenericDataArray::dispose()
	{

	}

	const GenericDataTypeArray* GenericDataArray::getType() const
	{
		return m_pType;
	}


	uint GenericDataArray::getCount() const
	{
		return m_array.getCount();
	}

	GenericDataValue& GenericDataArray::getElement( uint index )
	{
		return m_array[ index ];
	}

	const GenericDataValue& GenericDataArray::getElement( uint index ) const
	{
		return m_array[ index ];
	}

	bool GenericDataArray::addElement( const GenericDataValue& value )
	{
		m_array.add();

		return setElement( m_array.getCount() - 1u, value );
	}

	bool GenericDataArray::setElement( uint index, const GenericDataValue& value )
	{
		if ( index >= m_array.getCount() )
		{
			TIKI_TRACE_ERROR( "[GenericDataArray::setElement] Index is greater as the size(%i > %i).\n", index, m_array.getCount() );
			return false;
		}

		if ( value.getType() != m_pType->getBaseType() )
		{
			TIKI_TRACE_ERROR( "[GenericDataArray::setElement] Can't assign value with different type('%s' != '%s')!\n", value.getType()->getName().cStr(), m_pType->getBaseType()->getName().cStr() );
			return false;
		}

		m_array[ index ] = value;

		return true;
	}

	bool GenericDataArray::removeElement( uint index )
	{
		if ( index >= m_array.getCount() )
		{
			TIKI_TRACE_ERROR( "[GenericDataArray::removeElement] Index is greater as the size(%i > %i).\n", index, m_array.getCount() );
			return false;
		}

		m_array[ index ].dispose();
		m_array.removeSortedAtIndex( index );

		return true;
	}

	bool GenericDataArray::importFromXml( XmlReader& reader, const _XmlElement* pObjectRootNode )
	{
		bool result = true;

		const XmlElement* pElement = reader.findFirstChild( "element", pObjectRootNode );
		while ( pElement )
		{
			const XmlAttribute* pTypeAtt = reader.findAttributeByName( "type", pElement );

			if ( pTypeAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataArray::importFromXml] 'element' node needs a 'type' attribute.\n" );
				result = false;
				continue;
			}

			const GenericDataType* pElementType = m_collection.parseType( pTypeAtt->content );
			if ( pElementType == nullptr )
			{
				result = false;
				continue;
			}

			GenericDataValue value;
			const XmlAttribute* pValueAtt = reader.findAttributeByName( "value", pElement );
			if ( pValueAtt != nullptr )
			{
				if ( !m_collection.parseValue( value, pValueAtt->content, pElementType ) )
				{
					result = false;
					continue;
				}
			}
			else
			{
				const XmlElement* pChildElement = pElement->elements;
				if ( pChildElement == nullptr )
				{
					TIKI_TRACE_ERROR( "[GenericDataArray::importFromXml] 'element' node needs a 'value' attribute or a child node named 'object' or 'array'.\n" );
					result = false;
					continue;
				}

				const XmlAttribute* pChildTypeAtt = reader.findAttributeByName( "type", pChildElement );
				if ( pChildTypeAtt == nullptr )
				{
					TIKI_TRACE_ERROR( "[GenericDataArray::importFromXml] child node needs a 'type' attribute.\n" );
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
						TIKI_TRACE_ERROR( "[GenericDataArray::importFromXml] type of a object needs to be a struct. '%s' is not a struct.\n", pChildType->getName().cStr() );
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
					TIKI_TRACE_ERROR( "[GenericDataArray::importFromXml] child node has a invalid name(%s).\n", pChildElement->name );
					result = false;
					continue;
				}
			}

			pElement = reader.findNext( "element", pElement );
		}

		return true; 
	}

	bool GenericDataArray::exportToResource( ResourceWriter& writer ) const
	{
		return true;
	}
}