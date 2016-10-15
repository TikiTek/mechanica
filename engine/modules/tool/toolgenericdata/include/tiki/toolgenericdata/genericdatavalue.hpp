#pragma once
#ifndef TIKI_GENERICDATAVALUE_HPP_INCLUDED__
#define TIKI_GENERICDATAVALUE_HPP_INCLUDED__

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolgenericdata/genericdatatypetype.hpp"

namespace tiki
{
	class GenericDataArray;
	class GenericDataObject;
	class GenericDataType;

	enum GenericDataValueType
	{
		GenericDataValueType_Invalid = -1,

		GenericDataValueType_Boolean,

		GenericDataValueType_SingedInteger8,
		GenericDataValueType_SingedInteger16,
		GenericDataValueType_SingedInteger32,
		GenericDataValueType_SingedInteger64,

		GenericDataValueType_UnsingedInteger8,
		GenericDataValueType_UnsingedInteger16,
		GenericDataValueType_UnsingedInteger32,
		GenericDataValueType_UnsingedInteger64,

		GenericDataValueType_FloatingPoint16,
		GenericDataValueType_FloatingPoint32,
		GenericDataValueType_FloatingPoint64,

		GenericDataValueType_String,

		GenericDataValueType_Object,
		GenericDataValueType_Array,

		GenericDataValueType_Enum,

		GenericDataValueType_Reference,
		GenericDataValueType_Pointer,

		GenericDataValueType_Count
	};

	class GenericDataValue
	{
	public:

								GenericDataValue();
								GenericDataValue( const GenericDataType* pType );
								~GenericDataValue();

		void					dispose();

		const GenericDataType*	getType() const;
		GenericDataValueType	getValueType() const;

		bool					getBoolean( bool& value ) const;
		bool					setBoolean( bool value, const GenericDataType* pType );

		bool					getSignedValue( sint64& value ) const;
		bool					setSignedValue( sint64 value, const GenericDataType* pType );
		bool					getUnsignedValue( uint64& value ) const;
		bool					setUnsignedValue( uint64 value, const GenericDataType* pType );
		bool					getFloatingPoint( float64& value ) const;
		bool					setFloatingPoint( float64 value, const GenericDataType* pType );

		bool					getString( string& value ) const;
		bool					setString( const string& value, const GenericDataType* pType );

		bool					getObject( GenericDataObject*& pValue ) const;
		bool					setObject( GenericDataObject* pValue );
		bool					getArray( GenericDataArray*& pValue ) const;
		bool					setArray( GenericDataArray* pValue );

		bool					getEnum( string& enumName, sint64& enumValue ) const;
		bool					setEnum( const string& valueName, const GenericDataType* pType );

		bool					getReference( string& refText ) const;
		bool					setReference( const string& refText, const GenericDataType* pType );

	private:

		union Values
		{
			bool				b;

			uint8				u8;
			uint16				u16;
			uint32				u32;
			uint64				u64;

			sint8				s8;
			sint16				s16;
			sint32				s32;
			sint64				s64;

			float16				f16;
			float				f32;
			double				f64;

			GenericDataObject*	pObject;
			GenericDataArray*	pArray;
		};

		const GenericDataType*	m_pType;
		GenericDataValueType	m_valueType;

		Values					m_value;
		string					m_text;

		bool					setType( const GenericDataType* pType, GenericDataTypeType expectedType );

	};
}

#endif // TIKI_GENERICDATAVALUE_HPP_INCLUDED__
