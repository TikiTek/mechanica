#include "tiki/tool_generic_data/generic_data_tag_handler.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/tool_generic_data/generic_data_type_reference.hpp"

namespace tiki
{
	GenericDataTagHandler::GenericDataTagHandler( GenericDataTypeCollection& collection )
		: m_collection( collection )
	{
		static const TypeTag s_aTypeTags[] =
		{
			{ "array",		&GenericDataTagHandler::resolveArrayTypeTag },		// GenericDataTypeTag_Array
			{ "reference",	&GenericDataTagHandler::resolveReferenceTypeTag },	// GenericDataTypeTag_Reference
			{ "pointer",	&GenericDataTagHandler::resolvePointerTypeTag }		// GenericDataTypeTag_Pointer
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aTypeTags ) == GenericDataTypeTag_Count );
		m_typeTags = TypeTagArray( s_aTypeTags, TIKI_COUNT( s_aTypeTags ) );

		static const ValueTag s_aValueTags[] =
		{
			{ "enum",		&GenericDataTagHandler::resolveEnumValueTag },		// GenericDataValueTag_Enum
			{ "bit",		&GenericDataTagHandler::resolveBitValueTag },		// GenericDataValueTag_Bit
			{ "offset",		&GenericDataTagHandler::resolveOffsetValueTag },	// GenericDataValueTag_Offset
			{ "crc",		&GenericDataTagHandler::resolveCrcValueTag }		// GenericDataValueTag_Crc
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aValueTags ) == GenericDataValueTag_Count );
		m_valueTags = ValueTagArray( s_aValueTags, TIKI_COUNT( s_aValueTags ) );
	}

	GenericDataTagHandler::~GenericDataTagHandler()
	{

	}

	const char* GenericDataTagHandler::getTypeTag( GenericDataTypeTag value ) const
	{
		return m_typeTags[ value ].pTag;
	}

	const char* GenericDataTagHandler::getValueTag( GenericDataValueTag value ) const
	{
		return m_valueTags[ value ].pTag;
	}

	const GenericDataType* GenericDataTagHandler::resolveTypeTag( const GenericDataTag* pTag )
	{
		if( pTag->getChildTag() != nullptr )
		{
			TIKI_TRACE_ERROR( "[resolveTypeTag] type tags doesn't support recursion.\n" );
		}

		for( const TypeTag& typeTag : m_typeTags )
		{
			if( !isStringEquals( typeTag.pTag, pTag->getTag().cStr() ) )
			{
				continue;
			}

			return (this->*typeTag.pResolveFunc)(pTag);
		}

		TIKI_TRACE_ERROR( "[resolveTypeTag] Modifier(%s) not supported.\n", pTag->getTag().cStr() );
		return nullptr;
	}

	bool GenericDataTagHandler::resolveValueTag( DynamicString& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType ) const
	{
		if( pTag == nullptr )
		{
			TIKI_TRACE_ERROR( "[resolveValueTag] pTag is null.\n" );
			return false;
		}

		List< const GenericDataTag* > tagTree;
		tagTree.add( pTag );

		const GenericDataTag* pDepestTag = pTag;
		while( pTag->getChildTag() != nullptr )
		{
			pTag = pTag->getChildTag();
			tagTree.add( pTag );
		}

		DynamicString content = pDepestTag->getContent();
		for( uint i = tagTree.getCount() - 1u; i < tagTree.getCount(); --i )
		{
			const GenericDataTag* pCurrentTag = tagTree[ i ];

			if( pCurrentTag->getTag() == "enum" )
			{
				const GenericDataTypeEnum* pEnumType = nullptr;
				const GenericDataEnumValue* pEnumValue = nullptr;

				if( !parseEnum( &pEnumType, &pEnumValue, content ) )
				{
					return false;
				}

				sint64 intValue = 0;
				if( pEnumValue->pValue == nullptr )
				{
					TIKI_TRACE_ERROR( "[resolveValueTag] enum value with name '%s_%s' not found.\n", pEnumType->getName().cStr(), content.cStr() );
					return false;
				}
				else if( !pEnumValue->pValue->getSignedInteger( intValue ) )
				{
					TIKI_TRACE_ERROR( "[resolveValueTag] enum value with name '%s_%s' is not an integer.\n", pEnumType->getName().cStr(), content.cStr() );
					return false;
				}

				content = string_tools::toString( intValue );
			}
			else if( pCurrentTag->getTag() == "bit" )
			{
				const sint64 shift = string_tools::parseSInt64( content.cStr() );
				const sint64 value = 1ll << shift;

				content = string_tools::toString( value );
			}
			else if( pCurrentTag->getTag() == "offset" )
			{
				if( pParentType != nullptr && pParentType->getType() == GenericDataTypeType_Struct )
				{
					const GenericDataTypeStruct* pTypedParent = (const GenericDataTypeStruct*)pParentType;
					const List< GenericDataStructField >& fields = pTypedParent->getFields();

					bool found = false;
					sint64 offset = 0;
					for( uint i = 0u; i < fields.getCount(); ++i )
					{
						const GenericDataStructField& field = fields[ i ];

						offset = alignValue( offset, (sint64)field.pType->getAlignment() );

						if( field.name == content )
						{
							content = string_tools::toString( offset );
							found = true;
							break;
						}

						offset += field.pType->getSize();
					}

					if( !found )
					{
						TIKI_TRACE_ERROR( "[resolveValueTag] field with name '%s' for modifier '%s' not found.\n", content.cStr(), pCurrentTag->getTag().cStr() );
						return false;
					}
				}
				else
				{
					TIKI_TRACE_ERROR( "[resolveValueTag] modifier '%s' must have a struct type. '%s' is not a struct.\n", pCurrentTag->getTag().cStr(), pParentType->getName().cStr() );
					return false;
				}
			}
			else if( pCurrentTag->getTag() == "crc" )
			{
				content = string_tools::toString( crcString( content ) );
			}
			else
			{
				TIKI_TRACE_ERROR( "[resolveValueTag] modifier '%s' not supported.\n", pCurrentTag->getTag().cStr() );
				return false;
			}
		}

		targetContent = content;
		return true;
	}

	bool GenericDataTagHandler::parseEnum( const GenericDataTypeEnum** ppEnumType, DynamicString& enumValue, const DynamicString& content ) const
	{
		const int dotIndex = content.indexOf( '.' );
		if( dotIndex == -1 )
		{
			TIKI_TRACE_ERROR( "[resolveValueTag] Please use {enum TypeName.ValueName} for enum tags.\n" );
			return false;
		}

		const DynamicString enumTypeName = content.subString( 0u, dotIndex );
		const GenericDataType* pEnumType = m_collection.findTypeByName( enumTypeName );
		if( pEnumType == nullptr || pEnumType->getType() != GenericDataTypeType_Enum )
		{
			TIKI_TRACE_ERROR( "[resolveValueTag] '%s' not found or not an enum.\n", enumTypeName.cStr() );
			return false;
		}

		*ppEnumType	= (const GenericDataTypeEnum*)pEnumType;
		enumValue	= content.subString( dotIndex + 1 );
		return true;
	}

	bool GenericDataTagHandler::parseEnum( const GenericDataTypeEnum** ppEnumType, const GenericDataEnumValue** ppEnumValue, const DynamicString& content ) const
	{
		DynamicString enumName;
		if( !parseEnum( ppEnumType, enumName, content ) )
		{
			return false;
		}

		for( const GenericDataEnumValue& value : (*ppEnumType)->getValues() )
		{
			if( value.name == enumName )
			{
				*ppEnumValue = &value;
				return true;
			}
		}

		return false;
	}

	DynamicString GenericDataTagHandler::encodeEnum( const GenericDataTypeEnum* pEnumType, const GenericDataEnumValue& enumValue ) const
	{
		TIKI_ASSERT( pEnumType->isValidValue( enumValue ) );
		return formatDynamicString( "%s.%s", pEnumType->getName().cStr(), enumValue.name.cStr() );
	}

	const GenericDataType* GenericDataTagHandler::resolveArrayTypeTag( const GenericDataTag* pTag )
	{
		const GenericDataType* pType = m_collection.findTypeByName( pTag->getContent() );
		if( pType == nullptr )
		{
			TIKI_TRACE_ERROR( "[resolveTypeTag] Unable to find Type with name '%s'.\n", pTag->getContent().cStr() );
			return nullptr;
		}

		return m_collection.makeArrayType( pType );
	}

	const GenericDataType* GenericDataTagHandler::resolveReferenceTypeTag( const GenericDataTag* pTag )
	{
		const GenericDataType* pType = m_collection.findTypeByName( pTag->getContent() );
		if( pType == nullptr )
		{
			TIKI_TRACE_ERROR( "[resolveTypeTag] Unable to find Type with name '%s'.\n", pTag->getContent().cStr() );
			return nullptr;
		}

		if( pType->getType() != GenericDataTypeType_Resource )
		{
			TIKI_TRACE_ERROR( "[resolveTypeTag] Reference needs a Resource type as base. '%s' is not a resource.\n", pTag->getContent().cStr() );
			return nullptr;
		}

		const GenericDataTypeResource* pTypedType = (const GenericDataTypeResource*)pType;
		return m_collection.makeReferenceType( pTypedType );
	}

	const GenericDataType* GenericDataTagHandler::resolvePointerTypeTag( const GenericDataTag* pTag )
	{
		const GenericDataType* pType = m_collection.findTypeByName( pTag->getContent() );
		if( pType == nullptr )
		{
			TIKI_TRACE_ERROR( "[resolveTypeTag] Unable to find Type with name '%s'.\n", pTag->getContent().cStr() );
			return nullptr;
		}

		if( pType->getType() != GenericDataTypeType_Struct )
		{
			TIKI_TRACE_ERROR( "[resolveTypeTag] Pointer needs a Struct type as base. '%s' is not a resource.\n", pTag->getContent().cStr() );
			return nullptr;
		}

		const GenericDataTypeStruct* pTypedType = (const GenericDataTypeStruct*)pType;
		return m_collection.makePointerType( pTypedType );
	}

	bool GenericDataTagHandler::resolveEnumValueTag( DynamicString& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType )
	{
		const GenericDataTypeEnum* pEnumType = nullptr;
		if( !parseEnum( &pEnumType, targetContent, targetContent ) )
		{
			return false;
		}

		const GenericDataValue* pValue = pEnumType->getValueByName( targetContent );

		if( pValue == nullptr )
		{
			TIKI_TRACE_ERROR( "[resolveValueTag] enum value with name '%s_%s' not found.\n", pEnumType->getName().cStr(), targetContent.cStr() );
			return false;
		}

		sint64 intValue = 0;
		if( !pValue->getSignedInteger( intValue ) )
		{
			TIKI_TRACE_ERROR( "[resolveValueTag] enum value with name '%s_%s' is not an integer.\n", pEnumType->getName().cStr(), targetContent.cStr() );
			return false;
		}

		return true;
	}

	bool GenericDataTagHandler::resolveBitValueTag( DynamicString& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType )
	{
		const sint64 shift = string_tools::parseSInt64( targetContent.cStr() );
		const sint64 value = 1ll << shift;

		targetContent = string_tools::toString( value );
		return true;
	}

	bool GenericDataTagHandler::resolveOffsetValueTag( DynamicString& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType )
	{
		if( pParentType != nullptr && pParentType->getType() == GenericDataTypeType_Struct )
		{
			const GenericDataTypeStruct* pTypedParent = (const GenericDataTypeStruct*)pParentType;
			const List< GenericDataStructField >& fields = pTypedParent->getFields();

			bool found = false;
			sint64 offset = 0;
			for( uint i = 0u; i < fields.getCount(); ++i )
			{
				const GenericDataStructField& field = fields[ i ];

				offset = alignValue( offset, (sint64)field.pType->getAlignment() );

				if( field.name == targetContent )
				{
					targetContent = string_tools::toString( offset );
					found = true;
					break;
				}

				offset += field.pType->getSize();
			}

			if( !found )
			{
				TIKI_TRACE_ERROR( "[resolveValueTag] field with name '%s' for modifier '%s' not found.\n", targetContent.cStr(), pTag->getTag().cStr() );
				return false;
			}
		}
		else
		{
			TIKI_TRACE_ERROR( "[resolveValueTag] modifier '%s' must have a struct type. '%s' is not a struct.\n", pTag->getTag().cStr(), pParentType->getName().cStr() );
			return false;
		}

		return true;
	}

	bool GenericDataTagHandler::resolveCrcValueTag( DynamicString& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType )
	{
		targetContent = string_tools::toString( crcString( targetContent ) );
		return true;
	}
}