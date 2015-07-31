
#include "tiki/toolgenericdata/genericdatacontainer.hpp"

#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/genericdataarray.hpp"
#include "tiki/toolgenericdata/genericdataobject.hpp"
#include "tiki/toolgenericdata/genericdatatypearray.hpp"
#include "tiki/toolgenericdata/genericdatatypecollection.hpp"
#include "tiki/toolgenericdata/genericdatatypestruct.hpp"
#include "tiki/toolgenericdata/genericdatavalue.hpp"

namespace tiki
{
	GenericDataContainer::GenericDataContainer( GenericDataTypeCollection& collection )
		: m_collection( collection )
	{
	}

	GenericDataContainer::~GenericDataContainer()
	{
	}

	bool GenericDataContainer::importFromXml( XmlReader& reader, const _XmlElement* pObjectRootNode )
	{
		bool result = true;

		const char* pElementName = getElementName();
		const XmlElement* pElement = reader.findFirstChild( pElementName, pObjectRootNode );
		while ( pElement )
		{
			const XmlAttribute* pTypeAtt = reader.findAttributeByName( "type", pElement );

			if ( pTypeAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataContainer::importFromXml] '%s' node needs a 'type' attribute.\n", pElementName );
				result = false;
			}
			else
			{
				const GenericDataType* pElementType = m_collection.parseType( pTypeAtt->content );
				if ( pElementType == nullptr )
				{
					result = false;
				}
				else
				{
					GenericDataValue value;
					if ( !loadValueFromXml( value, reader, pElement, pElementType ) )
					{
						result = false;
					}
					else
					{
						applyElementValue( reader, pElement, value );
					}
				}
			}

			pElement = reader.findNext( pElementName, pElement );
		}

		return true; 
	}

	bool GenericDataContainer::writeValueToResource( ResourceWriter& writer, const GenericDataValue& value ) const
	{
		const GenericDataType* pType = value.getType();

		writer.writeAlignment( pType->getAlignment() );

		switch (value.getValueType())
		{
		case GenericDataValueType_Boolean:
			{
				bool b;
				if ( value.getBoolean( b ) )
				{
					writer.writeUInt8( b );
					return true;
				}
			}
			break;
		}

		return false;
	}

	bool GenericDataContainer::loadValueFromXml( GenericDataValue& outValue, XmlReader& reader, const _XmlElement* pElement, const GenericDataType* pElementType )
	{
		const XmlAttribute* pValueAtt = reader.findAttributeByName( "value", pElement );
		if ( pValueAtt != nullptr )
		{
			return m_collection.parseValue( outValue, pValueAtt->content, pElementType );
		}
		else
		{
			const XmlElement* pChildElement = pElement->elements;
			while ( pChildElement->next != nullptr && pChildElement->name == nullptr )
			{
				pChildElement = pChildElement->next;
			}

			if ( pChildElement == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataContainer::importFromXml] '%s' node needs a 'value' attribute or a child node named 'object' or 'array'.\n", pElement->name );
				return false;
			}

			const XmlAttribute* pChildTypeAtt = reader.findAttributeByName( "type", pChildElement );
			if ( pChildTypeAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataContainer::importFromXml] child node needs a 'type' attribute.\n" );
				return false;
			}

			const GenericDataType* pChildType = m_collection.parseType( pChildTypeAtt->content );
			if ( pChildType == nullptr )
			{
				return false;
			}

			const string childName = pChildElement->name;
			if ( childName == "object" )
			{
				if ( pChildType->getType() != GenericDataTypeType_Struct )
				{
					TIKI_TRACE_ERROR( "[GenericDataContainer::importFromXml] type of a object needs to be a struct. '%s' is not a struct.\n", pChildType->getName().cStr() );
					return false;
				}

				const GenericDataTypeStruct* pTypedChildType = (const GenericDataTypeStruct*)pChildType;
				GenericDataObject* pObject = TIKI_MEMORY_NEW_OBJECT( GenericDataObject )( m_collection );
				if ( !pObject->create( pTypedChildType ) )
				{
					TIKI_MEMORY_DELETE_OBJECT( pObject );
					return false;
				}

				if ( !pObject->importFromXml( reader, pChildElement ) )
				{
					TIKI_MEMORY_DELETE_OBJECT( pObject );
					return false;
				}

				outValue.setObject( pObject );
			}
			else if ( childName == "array" )
			{
				pChildType = m_collection.makeArrayType( pChildType );
				if ( pChildType->getType() != GenericDataTypeType_Array )
				{
					TIKI_TRACE_ERROR( "[GenericDataContainer::importFromXml] type of a array needs to be a array. '%s' is not an array.\n", pChildType->getName().cStr() );
					return false;
				}

				const GenericDataTypeArray* pTypedChildType = (const GenericDataTypeArray*)pChildType;
				GenericDataArray* pArray = TIKI_MEMORY_NEW_OBJECT( GenericDataArray )( m_collection );
				if ( !pArray->create( pTypedChildType ) )
				{
					TIKI_MEMORY_DELETE_OBJECT( pArray );
					return false;
				}

				if ( !pArray->importFromXml( reader, pChildElement ) )
				{
					TIKI_MEMORY_DELETE_OBJECT( pArray );
					return false;
				}

				outValue.setArray( pArray );
			}
			else 
			{
				TIKI_TRACE_ERROR( "[GenericDataContainer::importFromXml] child node has a invalid name(%s).\n", pChildElement->name );
				return false;
			}
		}

		return true;
	}
}