
#include "tiki/toolgenericdata/generic_data_value.hpp"

#include "tiki/base/numberlimits.hpp"
#include "tiki/toolgenericdata/genericdataarray.hpp"
#include "tiki/toolgenericdata/genericdataobject.hpp"
#include "tiki/toolgenericdata/genericdatatypearray.hpp"
#include "tiki/toolgenericdata/genericdatatypeenum.hpp"
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
			return "Reference";

		case GenericDataValueType_Pointer:
			return "Pointer";

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

	bool GenericDataValue::setBoolean( bool value, const GenericDataType* pType )
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

	bool GenericDataValue::setSignedValue( sint64 value, const GenericDataType* pType )
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

	bool GenericDataValue::setUnsignedValue( uint64 value, const GenericDataType* pType )
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

	bool GenericDataValue::setFloatingPoint( float64 value, const GenericDataType* pType )
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

	bool GenericDataValue::setString( const string& value, const GenericDataType* pType )
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

	bool GenericDataValue::setEnum( const string& valueName, const GenericDataType* pType )
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

	bool GenericDataValue::setReference( const string& refText, const GenericDataType* pType )
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
		if( m_pType->getType() == pType->getType() )
		{
			return true;
		}

		return m_pType->isTypeCompatible( pType );
	}
}
