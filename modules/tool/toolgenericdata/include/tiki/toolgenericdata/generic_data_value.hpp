#pragma once
#ifndef TIKI_GENERIC_DATA_VALUE_HPP_INCLUDED
#define TIKI_GENERIC_DATA_VALUE_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolgenericdata/generic_data_type_type.hpp"

namespace tiki
{
	class GenericDataArray;
	class GenericDataContainer;
	class GenericDataObject;
	class GenericDataTag;
	class GenericDataType;
	class GenericDataTypeCollection;
	class XmlElement;

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
		TIKI_NONCOPYABLE_CLASS( GenericDataValue );

	public:

		explicit					GenericDataValue( const GenericDataType* pType );
									~GenericDataValue();

		const GenericDataType*		getType() const;
		GenericDataValueType		getValueType() const;

		const GenericDataTag*		getValueTag() const;
		GenericDataTag*				getValueTag();
		void						setValueTag( GenericDataTag* pValueTag );

		bool						isValid() const;

		string						toString() const;

		bool						getBoolean( bool& value ) const;
		bool						setBoolean( bool value, const GenericDataType* pType = nullptr );

		bool						getSignedValue( sint64& value ) const;
		bool						setSignedValue( sint64 value, const GenericDataType* pType = nullptr);
		bool						getUnsignedValue( uint64& value ) const;
		bool						setUnsignedValue( uint64 value, const GenericDataType* pType = nullptr);
		bool						getFloatingPoint( float64& value ) const;
		bool						setFloatingPoint( float64 value, const GenericDataType* pType = nullptr);

		bool						getString( string& value ) const;
		bool						setString( const string& value, const GenericDataType* pType = nullptr);

		bool						getObject( GenericDataObject*& pValue );
		bool						getObject( const GenericDataObject*& pValue ) const;
		bool						setObject( GenericDataObject* pValue );
		bool						getArray( GenericDataArray*& pValue );
		bool						getArray( const GenericDataArray*& pValue ) const;
		bool						setArray( GenericDataArray* pValue );

		bool						getEnum( string& enumName, sint64* pEnumValue = nullptr ) const;
		bool						setEnum( const string& valueName, const GenericDataType* pType = nullptr);

		bool						getReference( string& refText ) const;
		bool						setReference( const string& refText, const GenericDataType* pType = nullptr);

		bool						getPointer( GenericDataObject*& pValue );
		bool						getPointer( const GenericDataObject*& pValue ) const;
		bool						setPointer( GenericDataObject* pValue );

		bool						importFromXml( XmlElement* pNode, const GenericDataType* pType, const GenericDataContainer* pParent, GenericDataTypeCollection& collection );
		bool						exportToXml( XmlElement* pParentNode, const GenericDataContainer* pParent, GenericDataTypeCollection& collection );

		bool						setCopyFromValue( GenericDataTypeCollection& collection, const GenericDataValue* pValue );

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

		XmlElement*				m_pNode;

		const GenericDataType*	m_pType;
		GenericDataValueType	m_valueType;

		Values					m_value;
		string					m_text;

		GenericDataTag*			m_pValueTag;

		GenericDataValueType	getValueType( const GenericDataType* pType );
		bool					checkType( const GenericDataType* pType);
	};
}

#endif // TIKI_GENERIC_DATA_VALUE_HPP_INCLUDED
