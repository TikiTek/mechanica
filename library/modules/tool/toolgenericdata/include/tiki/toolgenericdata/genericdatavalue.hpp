#pragma once
#ifndef TIKI_GENERICDATAVALUE_HPP_INCLUDED__
#define TIKI_GENERICDATAVALUE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/basicstring.hpp"

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

		bool					setUint8( uint8 value, const GenericDataType* pType );

		bool					setString( const string& value, const GenericDataType* pType );

		bool					setObject( GenericDataObject* pValue );
		bool					setArray( GenericDataArray* pValue );

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

		bool					setType( const GenericDataType* pType );

	};
}

#endif // TIKI_GENERICDATAVALUE_HPP_INCLUDED__
