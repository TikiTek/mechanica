
#include "tiki/toolgenericdata/generic_data_value.hpp"

#include "tiki/base/numberlimits.hpp"
#include "tiki/toolgenericdata/generic_data_array.hpp"
#include "tiki/toolgenericdata/generic_data_object.hpp"
#include "tiki/toolgenericdata/generic_data_type_array.hpp"
#include "tiki/toolgenericdata/generic_data_type_enum.hpp"
#include "tiki/toolgenericdata/genericdatatypestruct.hpp"
#include "tiki/toolgenericdata/genericdatatypevaluetype.hpp"

namespace tiki
{
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_Boolean == (int)GenericDataTypeValueTypeType_Boolean );

	TIKI_COMPILETIME_ASSERT( GenericDataValueType_SingedInteger8 == (int)GenericDataTypeValueTypeType_SingedInteger8 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_SingedInteger16 == (int)GenericDataTypeValueTypeType_SingedInteger16 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_SingedInteger32 == (int)GenericDataTypeValueTypeType_SingedInteger32 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_SingedInteger64 == (int)GenericDataTypeValueTypeType_SingedInteger64 );

	TIKI_COMPILETIME_ASSERT( GenericDataValueType_UnsingedInteger8 == (int)GenericDataTypeValueTypeType_UnsingedInteger8 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_UnsingedInteger16 == (int)GenericDataTypeValueTypeType_UnsingedInteger16 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_UnsingedInteger32 == (int)GenericDataTypeValueTypeType_UnsingedInteger32 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_UnsingedInteger64 == (int)GenericDataTypeValueTypeType_UnsingedInteger64 );

	TIKI_COMPILETIME_ASSERT( GenericDataValueType_FloatingPoint16 == (int)GenericDataTypeValueTypeType_FloatingPoint16 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_FloatingPoint32 == (int)GenericDataTypeValueTypeType_FloatingPoint32 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_FloatingPoint64 == (int)GenericDataTypeValueTypeType_FloatingPoint64 );

	TIKI_COMPILETIME_ASSERT( GenericDataValueType_String == (int)GenericDataTypeValueTypeType_String );

	GenericDataValue::GenericDataValue( const GenericDataType* pType )
	{
		TIKI_ASSERT( pType != nullptr );

		m_pNode		= nullptr;
		m_pType		= pType;
		m_valueType	= getValueType( pType );
		m_pValueTag	= nullptr;

		memory::zero( m_value );
	}

	GenericDataValue::~GenericDataValue()
	{
		if( (m_valueType == GenericDataValueType_Object || m_valueType == GenericDataValueType_Pointer) && m_value.pObject != nullptr )
		{
			m_value.pObject->dispose();
			TIKI_DELETE( m_value.pObject );
			m_value.pObject = nullptr;
		}
		else if( m_valueType == GenericDataValueType_Array && m_value.pArray != nullptr )
		{
			m_value.pArray->dispose();
			TIKI_DELETE( m_value.pArray );
			m_value.pArray = nullptr;
		}

		if( m_pValueTag != nullptr )
		{
			TIKI_DELETE( m_pValueTag );
			m_pValueTag = nullptr;
		}
	}

	const GenericDataType* GenericDataValue::getType() const
	{
		return m_pType;
	}

	GenericDataValueType GenericDataValue::getValueType() const
	{
		return m_valueType;
	}

	const GenericDataTag* GenericDataValue::getValueTag() const
	{
		return m_pValueTag;
	}

	GenericDataTag* GenericDataValue::getValueTag()
	{
		return m_pValueTag;
	}

	void GenericDataValue::setValueTag( GenericDataTag* pValueTag )
	{
		if( m_pValueTag != nullptr )
		{
			TIKI_DELETE( m_pValueTag );
		}

		m_pValueTag = pValueTag;
	}

	bool GenericDataValue::isValid() const
	{
		return m_valueType != GenericDataValueType_Invalid;
	}

	string GenericDataValue::toString() const
	{
		switch( m_valueType )
		{
		case GenericDataValueType_Boolean:
			return m_value.b ? "true" : "false";

		case GenericDataValueType_SingedInteger8:
			return string_tools::toString( m_value.s8 );

		case GenericDataValueType_SingedInteger16:
			return string_tools::toString( m_value.s16 );

		case GenericDataValueType_SingedInteger32:
			return string_tools::toString( m_value.s32 );

		case GenericDataValueType_SingedInteger64:
			return string_tools::toString( m_value.s64 );

		case GenericDataValueType_UnsingedInteger8:
			return string_tools::toString( m_value.u8 );

		case GenericDataValueType_UnsingedInteger16:
			return string_tools::toString( m_value.u16 );

		case GenericDataValueType_UnsingedInteger32:
			return string_tools::toString( m_value.u32 );

		case GenericDataValueType_UnsingedInteger64:
			return string_tools::toString( m_value.u64 );

		case GenericDataValueType_FloatingPoint16:
			return string_tools::toString( m_value.f16 );

		case GenericDataValueType_FloatingPoint32:
			return string_tools::toString( m_value.f32 );

		case GenericDataValueType_FloatingPoint64:
			return string_tools::toString( m_value.f64 );

		case GenericDataValueType_String:
			return m_text;

		case GenericDataValueType_Object:
			return m_value.pObject->getType()->getName();

		case GenericDataValueType_Array:
			return m_value.pArray->getType()->getName();

		case GenericDataValueType_Enum:
			return m_text;

		case GenericDataValueType_Reference:
			return m_text;

		case GenericDataValueType_Pointer:
			return m_value.pObject->getType()->getName();

		case GenericDataValueType_Invalid:
		case GenericDataValueType_Count:
			break;
		}

		return "Invalid";
	}

	bool GenericDataValue::getBoolean( bool& value ) const
	{
		if( m_valueType == GenericDataValueType_Boolean )
		{
			value = m_value.b;
			return true;
		}

		return false;
	}

	bool GenericDataValue::setBoolean( bool value, const GenericDataType* pType /*= nullptr*/ )
	{
		if( !checkType( pType ) )
		{
			return false;
		}

		m_value.b = value;

		return true;
	}

	bool GenericDataValue::getSignedValue( sint64& value ) const
	{
		switch( m_valueType )
		{
		case GenericDataValueType_SingedInteger8:
			value = m_value.s8;
			return true;

		case GenericDataValueType_SingedInteger16:
			value = m_value.s16;
			return true;

		case GenericDataValueType_SingedInteger32:
			value = m_value.s32;
			return true;

		case GenericDataValueType_SingedInteger64:
			value = m_value.s64;
			return true;

		case GenericDataValueType_UnsingedInteger8:
			return rangeCheckCast( value, m_value.u8 );

		case GenericDataValueType_UnsingedInteger16:
			return rangeCheckCast( value, m_value.u16 );

		case GenericDataValueType_UnsingedInteger32:
			return rangeCheckCast( value, m_value.u32 );

		case GenericDataValueType_UnsingedInteger64:
			return rangeCheckCast( value, m_value.u64 );

		default:
			break;
		}

		return false;
	}

	bool GenericDataValue::setSignedValue( sint64 value, const GenericDataType* pType /*= nullptr*/ )
	{
		if( !checkType( pType ) )
		{
			return false;
		}

		switch( pType->getSize() )
		{
		case 1:
			return rangeCheckCast( m_value.s8, value );

		case 2:
			return rangeCheckCast( m_value.s16, value );

		case 4:
			return rangeCheckCast( m_value.s32, value );

		case 8:
			return rangeCheckCast( m_value.s64, value );

		default:
			break;
		}

		return false;
	}

	bool GenericDataValue::getUnsignedValue( uint64& value ) const
	{
		switch( m_valueType )
		{
		case GenericDataValueType_UnsingedInteger8:
			value = m_value.u8;
			return true;

		case GenericDataValueType_UnsingedInteger16:
			value = m_value.u16;
			return true;

		case GenericDataValueType_UnsingedInteger32:
			value = m_value.u32;
			return true;

		case GenericDataValueType_UnsingedInteger64:
			value = m_value.u64;
			return true;

		case GenericDataValueType_SingedInteger8:
			return rangeCheckCast( value, m_value.s8 );

		case GenericDataValueType_SingedInteger16:
			return rangeCheckCast( value, m_value.s16 );

		case GenericDataValueType_SingedInteger32:
			return rangeCheckCast( value, m_value.s32 );

		case GenericDataValueType_SingedInteger64:
			return rangeCheckCast( value, m_value.s64 );

		default:
			break;
		}

		return false;
	}

	bool GenericDataValue::setUnsignedValue( uint64 value, const GenericDataType* pType /*= nullptr*/ )
	{
		if( !checkType( pType ) )
		{
			return false;
		}

		switch( pType->getSize() )
		{
		case 1:
			return rangeCheckCast( m_value.u8, value );

		case 2:
			return rangeCheckCast( m_value.u16, value );

		case 4:
			return rangeCheckCast( m_value.u32, value );

		case 8:
			return rangeCheckCast( m_value.u64, value );

		default:
			break;
		}

		return false;
	}

	bool GenericDataValue::getFloatingPoint( float64& value ) const
	{
		switch( m_valueType )
		{
		case GenericDataValueType_FloatingPoint16:
			value = m_value.f16;
			return true;

		case GenericDataValueType_FloatingPoint32:
			value = m_value.f32;
			return true;

		case GenericDataValueType_FloatingPoint64:
			value = m_value.f64;
			return true;

		default:
			break;
		}

		return false;
	}

	bool GenericDataValue::setFloatingPoint( float64 value, const GenericDataType* pType /*= nullptr*/ )
	{
		if( !checkType( pType ) )
		{
			return false;
		}

		switch( pType->getSize() )
		{

		case 2:
			return rangeCheckCast( m_value.f16, value );

		case 4:
			return rangeCheckCast( m_value.f32, value );

		case 8:
			return rangeCheckCast( m_value.f64, value );

		default:
			break;
		}

		return false;
	}

	bool GenericDataValue::getString( string& value ) const
	{
		if( m_valueType == GenericDataValueType_String )
		{
			value = m_text;
			return true;
		}

		return false;
	}

	bool GenericDataValue::setString( const string& value, const GenericDataType* pType /*= nullptr*/ )
	{
		if( !checkType( pType ) )
		{
			return false;
		}

		m_text = value;

		return true;
	}

	bool GenericDataValue::getObject( GenericDataObject*& pValue ) const
	{
		if( m_valueType == GenericDataValueType_Object )
		{
			pValue = m_value.pObject;
			return true;
		}

		return false;
	}

	bool GenericDataValue::setObject( GenericDataObject* pValue )
	{
		if( pValue == nullptr || !checkType( pValue->getType() ) )
		{
			return false;
		}

		m_value.pObject = pValue;

		return true;
	}

	bool GenericDataValue::getArray( GenericDataArray*& pValue ) const
	{
		if( m_valueType == GenericDataValueType_Array )
		{
			pValue = m_value.pArray;
			return true;
		}

		return false;
	}

	bool GenericDataValue::setArray( GenericDataArray* pValue )
	{
		if( pValue == nullptr || !checkType( pValue->getType() ) )
		{
			return false;
		}

		m_value.pArray = pValue;

		return true;
	}

	bool GenericDataValue::getEnum( string& enumName, sint64& enumValue ) const
	{
		if( m_valueType == GenericDataValueType_Enum )
		{
			enumName	= m_text;
			enumValue	= m_value.s64;
			return true;
		}

		return false;
	}

	bool GenericDataValue::setEnum( const string& valueName, const GenericDataType* pType /*= nullptr*/ )
	{
		if( !checkType( pType ) )
		{
			return false;
		}

		const GenericDataTypeEnum* pTypedEnumType = (const GenericDataTypeEnum*)pType;
		const GenericDataValue* pValue = pTypedEnumType->getValueByName( valueName );
		if( pValue == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataValue::setEnum] enum value with name '%s' not found.\n", valueName.cStr() );
			return false;
		}

		m_text	= valueName;
		m_value	= pValue->m_value;

		return true;
	}

	bool GenericDataValue::getReference( string& refText ) const
	{
		if( m_valueType == GenericDataValueType_Reference )
		{
			refText	= m_text;
			return true;
		}

		return false;
	}

	bool GenericDataValue::setReference( const string& refText, const GenericDataType* pType /*= nullptr*/ )
	{
		if( !checkType( pType ) )
		{
			return false;
		}

		m_text = refText;

		return true;
	}

	bool GenericDataValue::getPointer( GenericDataObject*& pValue ) const
	{
		if( m_valueType == GenericDataValueType_Pointer )
		{
			pValue = m_value.pObject;
			return true;
		}

		return false;
	}

	bool GenericDataValue::setPointer( GenericDataObject* pValue )
	{
		if( pValue == nullptr || !checkType( pValue->getType() ) )
		{
			return false;
		}

		m_value.pObject = pValue;
		return true;
	}

	bool GenericDataValue::importFromXml( XmlElement* pNode, const GenericDataType* pType, const GenericDataContainer* pParent, GenericDataTypeCollection& collection )
	{
		const XmlAttribute* pValueAtt = pNode->findAttribute( "value" );
		if( pValueAtt != nullptr )
		{
			return collection.parseValue( this, pValueAtt->getValue(), pType, pParent->getParentType() );
		}
		else
		{
			XmlElement* pChildNode = pNode->getFirstChild();
			while( pChildNode->getNextSibling() != nullptr && pChildNode->getName() == nullptr )
			{
				pChildNode = pChildNode->getNextSibling();
			}

			if( pChildNode == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataType::importFromXml] '%s' node needs a 'value' attribute or a child node named 'object' or 'array'.\n", pNode->getName() );
				return false;
			}

			const XmlAttribute* pChildTypeAtt = pChildNode->findAttribute( "type" );
			if( pChildTypeAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataType::importFromXml] child node needs a 'type' attribute.\n" );
				return false;
			}

			const GenericDataType* pChildType = collection.parseType( pChildTypeAtt->getValue() );
			if( pChildType == nullptr )
			{
				return false;
			}

			if( pChildNode->isName( "array" ) )
			{
				pChildType = collection.makeArrayType( pChildType );
			}

			if( !pType->isTypeCompatible( pChildType ) )
			{
				TIKI_TRACE_ERROR( "[GenericDataType::importFromXml] Child type '%s' is not compatible with '%s'.\n", pChildType->getName().cStr(), pType->getName().cStr() );
				return false;
			}

			if( pChildNode->isName( "object" ) )
			{
				if( pChildType->getType() != GenericDataTypeType_Struct )
				{
					TIKI_TRACE_ERROR( "[GenericDataType::importFromXml] type of a object needs to be a struct. '%s' is not a struct.\n", pChildType->getName().cStr() );
					return false;
				}

				const GenericDataTypeStruct* pTypedChildType = (const GenericDataTypeStruct*)pChildType;
				GenericDataObject* pObject = TIKI_NEW( GenericDataObject )( collection );
				if( !pObject->create( pTypedChildType, nullptr ) )
				{
					TIKI_DELETE( pObject );
					return false;
				}

				if( !pObject->importFromXml( pChildNode ) )
				{
					TIKI_DELETE( pObject );
					return false;
				}

				if( !setObject( pObject ) )
				{
					TIKI_DELETE( pObject );
					return false;
				}
			}
			else if( pChildNode->isName( "array" ) )
			{
				TIKI_ASSERT( pChildType->getType() == GenericDataTypeType_Array );

				const GenericDataTypeArray* pTypedChildType = (const GenericDataTypeArray*)pChildType;
				GenericDataArray* pArray = TIKI_NEW( GenericDataArray )( collection );
				if( !pArray->create( pTypedChildType ) )
				{
					TIKI_DELETE( pArray );
					return false;
				}

				if( !pArray->importFromXml( pChildNode ) )
				{
					TIKI_DELETE( pArray );
					return false;
				}

				if( !setArray( pArray ) )
				{
					TIKI_DELETE( pArray );
					return false;
				}
			}
			else
			{
				TIKI_TRACE_ERROR( "[GenericDataContainer::importFromXml] child node has a invalid name: '%s'.\n", pChildNode->getName() );
				return false;
			}
		}

		return true;
	}

	bool GenericDataValue::exportToXml( XmlElement* pParentNode, const GenericDataContainer* pParent, GenericDataTypeCollection& collection )
	{
		if( m_valueType == GenericDataValueType_Invalid || m_pType == nullptr )
		{
			return false;
		}

		if( m_pNode == nullptr )
		{
			m_pNode = m_pType->createXmlElement( pParentNode->getDocument(), pParent->getElementName() );
			if( !pParent->initializeXmlElementForValue( m_pNode, this ) )
			{
				pParentNode->getDocument().destroyElement( m_pNode );
				m_pNode = nullptr;

				return false;
			}

			pParentNode->appendChild( m_pNode );
		}

		if( m_valueType == GenericDataValueType_Array )
		{
			return m_value.pArray->exportToXml( m_pNode );
		}
		else if( m_valueType == GenericDataValueType_Object || m_valueType == GenericDataValueType_Pointer )
		{
			return m_value.pObject->exportToXml( m_pNode );
		}
		else if( m_pValueTag != nullptr )
		{
			m_pNode->setAttribute( "value", m_pValueTag->writeTagString().cStr() );
		}
		else
		{
			m_pNode->setAttribute( "value", toString().cStr() );
		}

		return true;
	}

	GenericDataValueType GenericDataValue::getValueType( const GenericDataType* pType )
	{
		TIKI_ASSERT( pType != nullptr );

		GenericDataValueType valueType = GenericDataValueType_Invalid;

		const GenericDataTypeType typeType = pType->getType();
		if( typeType == GenericDataTypeType_ValueType )
		{
			const GenericDataTypeValueType* pValueType = (const GenericDataTypeValueType*)pType;
			valueType	= (GenericDataValueType)pValueType->getValueType();
		}
		else if( typeType == GenericDataTypeType_Struct )
		{
			valueType	= GenericDataValueType_Object;
		}
		else if( typeType == GenericDataTypeType_Array )
		{
			valueType	= GenericDataValueType_Array;
		}
		else if( typeType == GenericDataTypeType_Enum )
		{
			valueType	= GenericDataValueType_Enum;
		}
		else if( typeType == GenericDataTypeType_Reference )
		{
			valueType	= GenericDataValueType_Reference;
		}
		else if( typeType == GenericDataTypeType_Pointer )
		{
			valueType	= GenericDataValueType_Pointer;
		}

		return valueType;
	}

	bool GenericDataValue::checkType( const GenericDataType* pType )
	{
		if( pType == nullptr || m_pType->getType() == pType->getType() )
		{
			return true;
		}

		return m_pType->isTypeCompatible( pType );
	}
}
