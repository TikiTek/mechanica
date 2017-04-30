#pragma once
#ifndef TIKI_GENERIC_DATA_TAG_HANDLER_HPP_INCLUDED
#define TIKI_GENERIC_DATA_TAG_HANDLER_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"
#include "tiki/container/fixedarray.hpp"

namespace tiki
{
	class GenericDataTag;
	class GenericDataType;
	class GenericDataTypeCollection;

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
		GenericDataValueTag_Reference,
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
		bool						resolveValueTag( string& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType ) const;

	private:

		typedef const GenericDataType* (GenericDataTagHandler::*TypeTagHandler)( const GenericDataTag* pTag );
		typedef bool (GenericDataTagHandler::*ValueTagHandler)( string& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType );

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

		bool						resolveEnumValueTag( string& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType );
		bool						resolveReferenceValueTag( string& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType );
		bool						resolveBitValueTag( string& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType );
		bool						resolveOffsetValueTag( string& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType );
		bool						resolveCrcValueTag( string& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType );
	};
}

#endif // TIKI_GENERIC_DATA_TAG_HANDLER_HPP_INCLUDED
