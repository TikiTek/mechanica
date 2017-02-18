#include "tiki/toolgenericdata/generic_data_tag_handler.hpp"

namespace tiki
{
	GenericDataTagHandler::GenericDataTagHandler( GenericDataTypeCollection& collection )
		: m_collection( collection )
	{
		//m_typeTags =
		//{
		//	{ "array", &GenericDataTagHandler::resolveArrayTypeTag }
		//};
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

		const GenericDataType* pType = nullptr;
		if( pTag->getTag() == "array" )
		{
			if( pType == nullptr )
			{
				pType = m_collection.findTypeByName( pTag->getContent() );
				if( pType == nullptr )
				{
					TIKI_TRACE_ERROR( "[resolveTypeTag] Unable to find Type with name '%s'.\n", pTag->getContent().cStr() );
					return nullptr;
				}
			}

			pType = m_collection.makeArrayType( pType );
		}
		else if( pTag->getTag() == "reference" )
		{
			const GenericDataTypeResource* pTypedType = nullptr;
			if( pType == nullptr )
			{
				pType = m_collection.findTypeByName( pTag->getContent() );
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

				pTypedType = (const GenericDataTypeResource*)pType;
			}

			pType = m_collection.makeReferenceType( pTypedType );
		}
		else if( pTag->getTag() == "pointer" )
		{
			const GenericDataTypeStruct* pTypedType = nullptr;
			if( pType == nullptr )
			{
				pType = m_collection.findTypeByName( pTag->getContent() );
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

				pTypedType = (const GenericDataTypeStruct*)pType;
			}

			pType = m_collection.makePointerType( pTypedType );
		}
		else
		{
			TIKI_TRACE_ERROR( "[resolveTypeTag] Modifier(%s) not supported.\n", pTag->getTag().cStr() );
			return nullptr;
		}

		return pType;
	}

	bool GenericDataTagHandler::resolveValueTag( string& targetContent, const GenericDataTag* pTag, const GenericDataType* pParentType ) const
	{
		List< const GenericDataTag* > tagTree;
		tagTree.add( pTag );

		const GenericDataTag* pDepestTag = pTag;
		while( pTag->getChildTag() != nullptr )
		{
			pTag = pTag->getChildTag();
			tagTree.add( pTag );
		}

		string content = pDepestTag->getContent();
		for( uint i = tagTree.getCount() - 1u; i < tagTree.getCount(); --i )
		{
			const GenericDataTag* pCurrentTag = tagTree[ i ];

			if( pCurrentTag->getTag() == "enum" )
			{
				const int dotIndex = content.indexOf( '.' );
				if( dotIndex == -1 )
				{
					TIKI_TRACE_ERROR( "[resolveValueTag] Please use {enum TypeName.ValueName} for enum tags.\n" );
					return false;
				}

				const string enumTypeName = content.subString( 0u, dotIndex );
				const GenericDataType* pEnumType = m_collection.findTypeByName( enumTypeName );
				if( pEnumType == nullptr || pEnumType->getType() != GenericDataTypeType_Enum )
				{
					TIKI_TRACE_ERROR( "[resolveValueTag] '%s' not found or not an enum.\n", enumTypeName.cStr() );
					return false;
				}

				content = content.subString( dotIndex + 1 );

				const GenericDataTypeEnum* pTypedEnumType = (const GenericDataTypeEnum*)pEnumType;
				const GenericDataValue* pValue = pTypedEnumType->getValueByName( content );

				sint64 intValue = 0;
				if( pValue == nullptr )
				{
					TIKI_TRACE_ERROR( "[resolveValueTag] enum value with name '%s_%s' not found.\n", pEnumType->getName().cStr(), content.cStr() );
					return false;
				}
				else if( !pValue->getSignedValue( intValue ) )
				{
					TIKI_TRACE_ERROR( "[resolveValueTag] enum value with name '%s_%s' is not an integer.\n", pEnumType->getName().cStr(), content.cStr() );
					return false;
				}
			}
			else if( pCurrentTag->getTag() == "reference" )
			{
				TIKI_NOT_IMPLEMENTED;
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
}