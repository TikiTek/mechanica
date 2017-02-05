
#include "tiki/toolgenericdata/genericdatatype.hpp"

namespace tiki
{
	GenericDataType::GenericDataType( GenericDataTypeCollection& collection, const string& name, GenericDataTypeType type, GenericDataTypeMode mode )
		: m_collection( collection )
		, m_name( name )
		, m_mode( mode )
		, m_type( type )
	{
		m_pDefaultValue = TIKI_NEW( GenericDataValue )( this );
	}

	GenericDataType::~GenericDataType()
	{
		TIKI_DELETE( m_pDefaultValue );
	}

	const string& GenericDataType::getName() const
	{
		return m_name;
	}

	GenericDataTypeMode GenericDataType::getMode() const
	{
		return m_mode;
	}

	GenericDataTypeType GenericDataType::getType() const
	{
		return m_type;
	}

	const string& GenericDataType::getModule() const
	{
		return m_module;
	}

	void GenericDataType::setModule( const string& value )
	{
		m_module = value;
	}

	const GenericDataValue* GenericDataType::getDefaultValue() const
	{
		return m_pDefaultValue;
	}

	bool GenericDataType::isTypeCompatible( const GenericDataType* pType ) const
	{
		return this == pType;
	}



	bool GenericDataType::loadValueFromXml( GenericDataValue* pTargetValue, const XmlReader& reader, const _XmlElement* pElement, const GenericDataType* pParentType ) const
	{
		const XmlAttribute* pValueAtt = reader.findAttributeByName( "value", pElement );
		if ( pValueAtt != nullptr )
		{
			return m_collection.parseValue( pTargetValue, pValueAtt->content, this, pParentType );
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
				TIKI_TRACE_ERROR( "[GenericDataType::importFromXml] '%s' node needs a 'value' attribute or a child node named 'object' or 'array'.\n", pElement->name );
				return false;
			}

			const XmlAttribute* pChildTypeAtt = reader.findAttributeByName( "type", pChildElement );
			if ( pChildTypeAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataType::importFromXml] child node needs a 'type' attribute.\n" );
				return false;
			}

			const GenericDataType* pChildType = m_collection.parseType( pChildTypeAtt->content );
			if ( pChildType == nullptr )
			{
				return false;
			}

			//if( pChildType != this)
			//{
			//	TIKI_TRACE_ERROR( "[GenericDataType::importFromXml] Child type must be the same('%s' != '%s').\n", pChildType->getName().cStr(), getName().cStr() );
			//	return false;
			//}

			const string childName = pChildElement->name;
			if ( childName == "object" )
			{
				if ( pChildType->getType() != GenericDataTypeType_Struct )
				{
					TIKI_TRACE_ERROR( "[GenericDataType::importFromXml] type of a object needs to be a struct. '%s' is not a struct.\n", pChildType->getName().cStr() );
					return false;
				}

				const GenericDataTypeStruct* pTypedChildType = (const GenericDataTypeStruct*)pChildType;
				GenericDataObject* pObject = TIKI_NEW( GenericDataObject )( m_collection );
				if ( !pObject->create( pTypedChildType, nullptr ) )
				{
					TIKI_DELETE( pObject );
					return false;
				}

				if ( !pObject->importFromXml( reader, pChildElement ) )
				{
					TIKI_DELETE( pObject );
					return false;
				}

				pTargetValue->setObject( pObject );
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
				GenericDataArray* pArray = TIKI_NEW( GenericDataArray )( m_collection );
				if ( !pArray->create( pTypedChildType ) )
				{
					TIKI_DELETE( pArray );
					return false;
				}

				if ( !pArray->importFromXml( reader, pChildElement ) )
				{
					TIKI_DELETE( pArray );
					return false;
				}

				pTargetValue->setArray( pArray );
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