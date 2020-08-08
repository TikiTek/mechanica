#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/container/fixed_array.hpp"

namespace tiki
{
	class GenericDataTag;
	class GenericDataType;
	class GenericDataTypeCollection;
	class GenericDataTypeEnum;
	struct GenericDataEnumValue;

	enum GenericDataTypeTag
	{
		GenericDataTypeTag_Array,
		GenericDataTypeTag_Reference,
		GenericDataTypeTag_Pointer,

		GenericDataTypeTag_Count
	};

	enum GenericDataValueTag
	{
		GenericDataValueTag_Enum,
		GenericDataValueTag_Bit,
		GenericDataValueTag_Offset,
		GenericDataValueTag_Crc,

		GenericDataValueTag_Count
	};

	class GenericDataTagHandler
	{
	public:

		GenericDataTagHandler( GenericDataTypeCollection& collection );
		~GenericDataTagHandler();

		const char*					getTypeTag( GenericDataTypeTag value ) const;
		const char*					getValueTag( GenericDataValueTag value ) const;

		const GenericDataType*		resolveTypeTag( const GenericDataTag* pTag );
		bool						resolveValueTag( DynamicString& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType ) const;

		bool						parseEnum( const GenericDataTypeEnum** ppEnumType, DynamicString& enumValue, const DynamicString& content ) const;
		bool						parseEnum( const GenericDataTypeEnum** ppEnumType, const GenericDataEnumValue** ppEnumValue, const DynamicString& content ) const;
		DynamicString				encodeEnum( const GenericDataTypeEnum* pEnumType, const GenericDataEnumValue& enumValue ) const;

	private:

		typedef const GenericDataType* (GenericDataTagHandler::*TypeTagHandler)( const GenericDataTag* pTag );
		typedef bool (GenericDataTagHandler::*ValueTagHandler)( DynamicString& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType );

		struct TypeTag
		{
			const char*				pTag;
			TypeTagHandler			pResolveFunc;
		};
		typedef ArrayView< TypeTag > TypeTagArray;

		struct ValueTag
		{
			const char*				pTag;
			ValueTagHandler			pResolveFunc;
		};
		typedef ArrayView< ValueTag > ValueTagArray;

		GenericDataTypeCollection&	m_collection;

		TypeTagArray				m_typeTags;
		ValueTagArray				m_valueTags;

		const GenericDataType*		resolveArrayTypeTag( const GenericDataTag* pTag );
		const GenericDataType*		resolveReferenceTypeTag( const GenericDataTag* pTag );
		const GenericDataType*		resolvePointerTypeTag( const GenericDataTag* pTag );

		bool						resolveEnumValueTag( DynamicString& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType );
		bool						resolveBitValueTag( DynamicString& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType );
		bool						resolveOffsetValueTag( DynamicString& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType );
		bool						resolveCrcValueTag( DynamicString& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType );
	};
}
