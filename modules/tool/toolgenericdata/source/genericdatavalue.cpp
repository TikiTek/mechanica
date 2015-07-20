
#include "tiki/toolgenericdata/genericdatavalue.hpp"

#include "tiki/toolgenericdata/genericdataarray.hpp"
#include "tiki/toolgenericdata/genericdataobject.hpp"
#include "tiki/toolgenericdata/genericdatatypestruct.hpp"
#include "tiki/toolgenericdata/genericdatatypevaluetype.hpp"

namespace tiki
{
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_Boolean			== GenericDataTypeValueTypeType_Boolean );

	TIKI_COMPILETIME_ASSERT( GenericDataValueType_SingedInteger8	== GenericDataTypeValueTypeType_SingedInteger8 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_SingedInteger16	== GenericDataTypeValueTypeType_SingedInteger16 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_SingedInteger32	== GenericDataTypeValueTypeType_SingedInteger32 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_SingedInteger64	== GenericDataTypeValueTypeType_SingedInteger64 );

	TIKI_COMPILETIME_ASSERT( GenericDataValueType_UnsingedInteger8	== GenericDataTypeValueTypeType_UnsingedInteger8 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_UnsingedInteger16	== GenericDataTypeValueTypeType_UnsingedInteger16 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_UnsingedInteger32	== GenericDataTypeValueTypeType_UnsingedInteger32 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_UnsingedInteger64	== GenericDataTypeValueTypeType_UnsingedInteger64 );

	TIKI_COMPILETIME_ASSERT( GenericDataValueType_FloatingPoint16	== GenericDataTypeValueTypeType_FloatingPoint16 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_FloatingPoint32	== GenericDataTypeValueTypeType_FloatingPoint32 );
	TIKI_COMPILETIME_ASSERT( GenericDataValueType_FloatingPoint64	== GenericDataTypeValueTypeType_FloatingPoint64 );

	TIKI_COMPILETIME_ASSERT( GenericDataValueType_String			== GenericDataTypeValueTypeType_String );
		
	GenericDataValue::GenericDataValue()
	{
		m_pType		= nullptr;
		m_valueType	= GenericDataValueType_Invalid;
	}

	GenericDataValue::GenericDataValue( const GenericDataType* pType )
	{
		m_pType		= nullptr;
		m_valueType	= GenericDataValueType_Invalid;

		setType( pType );
	}

	GenericDataValue::~GenericDataValue()
	{
	}

	void GenericDataValue::dispose()
	{
		if ( m_valueType == GenericDataValueType_Object )
		{
			TIKI_MEMORY_DELETE_OBJECT( m_value.pObject );
		}
		else if ( m_valueType == GenericDataValueType_Array )
		{
			TIKI_MEMORY_DELETE_OBJECT( m_value.pArray );
		}
	}

	const GenericDataType* GenericDataValue::getType() const
	{
		return m_pType;
	}

	bool GenericDataValue::setUint8( uint8 value, const GenericDataType* pType )
	{
		if ( !setType( pType ) )
		{
			return false;
		}

		m_value.u8 = value;

		return true;
	}

	bool GenericDataValue::setString( const string& value, const GenericDataType* pType )
	{
		if ( !setType( pType ) )
		{
			return false;
		}

		m_text = value;

		return true;
	}

	bool GenericDataValue::setObject( GenericDataObject* pValue )
	{
		if ( pValue == nullptr || !setType( pValue->getType() ) )
		{
			return false;
		}

		m_value.pObject = pValue;

		return true;
	}

	bool GenericDataValue::setType( const GenericDataType* pType )
	{
		if ( pType == nullptr )
		{
			return false;
		}

		if ( m_valueType == GenericDataValueType_Invalid )
		{
			const GenericDataTypeType typeType = pType->getType();
			if ( typeType == GenericDataTypeType_ValueType )
			{
				const GenericDataTypeValueType* pValueType = (const GenericDataTypeValueType*)pType;
				
				m_pType		= pType;
				m_valueType	= (GenericDataValueType)pValueType->getType();

				return true;
			}
			else if ( typeType == GenericDataTypeType_Struct )
			{
				m_pType		= pType;
				m_valueType	= GenericDataValueType_Object;

				return true;
			}
			else if ( typeType == GenericDataTypeType_Array )
			{
				m_pType		= pType;
				m_valueType	= GenericDataValueType_Array;

				return true;
			}
		}
		else if ( m_pType == pType )
		{
			return true;
		}

		return false;
	}

}
