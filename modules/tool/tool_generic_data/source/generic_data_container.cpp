#include "tiki/tool_generic_data/generic_data_container.hpp"

#include "tiki/tool_generic_data/generic_data_array.hpp"
#include "tiki/tool_generic_data/generic_data_object.hpp"
#include "tiki/tool_generic_data/generic_data_tag.hpp"
#include "tiki/tool_generic_data/generic_data_type_array.hpp"
#include "tiki/tool_generic_data/generic_data_type_collection.hpp"
#include "tiki/tool_generic_data/generic_data_type_enum.hpp"
#include "tiki/tool_generic_data/generic_data_type_pointer.hpp"
#include "tiki/tool_generic_data/generic_data_type_struct.hpp"
#include "tiki/tool_generic_data/generic_data_value.hpp"
#include "tiki/tool_xml/xml_attribute.hpp"
#include "tiki/tool_xml/xml_document.hpp"
#include "tiki/tool_xml/xml_element.hpp"

#if TIKI_ENABLED( TIKI_GENERIC_DATA_CONVERTER )
#	include "tiki/converter_base/converter_helper.hpp"
#	include "tiki/converter_base/resource_writer.hpp"
#endif

namespace tiki
{
	GenericDataContainer::GenericDataContainer( GenericDataTypeCollection& collection )
		: m_collection( collection )
		, m_pObjectNode( nullptr )
	{
	}

	GenericDataContainer::~GenericDataContainer()
	{
	}

	bool GenericDataContainer::importFromXml( XmlElement* pObjectNode, bool isType )
	{
		m_pObjectNode = pObjectNode;

		bool result = true;
		const char* pElementName = getElementName();
		XmlElement* pChildNode = pObjectNode->findFirstChild( pElementName );
		while ( pChildNode != nullptr)
		{
			const XmlAttribute* pTypeAtt = pChildNode->findAttribute( "type" );
			if ( pTypeAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataContainer::importFromXml] '%s' node needs a 'type' attribute.\n", pElementName );
				result = false;
			}
			else
			{
				const GenericDataType* pElementType = m_collection.parseType( pTypeAtt->getValue() );
				if ( pElementType == nullptr )
				{
					result = false;
				}
				else
				{
					GenericDataValue* pValue = addElementValue( pChildNode );
					if( pValue == nullptr )
					{
						TIKI_TRACE_ERROR( "[GenericDataContainer::importFromXml] addElementValue failed.\n" );
					}
					else if ( !pValue->importFromXml( pChildNode, pElementType, this, m_collection, isType ) )
					{
						result = false;
					}
				}
			}

			pChildNode = pChildNode->findNextSibling( pElementName );
		}

		return true;
	}

	bool GenericDataContainer::exportToXml( XmlElement* pParentNode )
	{
		if( m_pObjectNode == nullptr )
		{
			m_pObjectNode = getParentType()->createXmlElement( pParentNode->getDocument(), getNodeName() );
			pParentNode->appendChild( m_pObjectNode );
		}

		bool ok = true;
		const uint count = getElementCount();
		for( uint i = 0u; i < count; ++i )
		{
			GenericDataValue* pCurrentValue = getElementValue( i );
			if( pCurrentValue == nullptr )
			{
				continue;
			}

			ok &= pCurrentValue->exportToXml( m_pObjectNode, this, m_collection );
		}

		return ok;
	}

#if TIKI_ENABLED( TIKI_GENERIC_DATA_CONVERTER )
	bool GenericDataContainer::writeValueToResource( ResourceSectionWriter& sectionWriter, const GenericDataType* pTargetType, const GenericDataValue& value ) const
	{
		//const GenericDataType* pType = value.getType();
		//pType->isTypeCompatible( pTargetType );

		sectionWriter.writeAlignment( pTargetType->getAlignment() );

		//const GenericDataTag* pTag = value.getValueTag();
		//if( pTag != nullptr &&
		//	pTag->getTag() == "enum" )
		//{
		//	const GenericDataTypeEnum* pEnumType = nullptr;
		//	const GenericDataEnumValue* pEnumValue = nullptr;
		//	if( !m_collection.getTagHandler().parseEnum( &pEnumType, &pEnumValue, pTag->getContent() ) )
		//	{
		//		TIKI_TRACE_ERROR( "[GenericDataContainer::writeValueToResource] Failed to parse enum tag '%s'.\n", pTag->getContent().cStr() );
		//		return false;
		//	}

		//	if( pEnumValue != nullptr &&
		//		pEnumValue->hasValue )
		//	{
		//		return writeValueToResource( sectionWriter, pTargetType, *pEnumValue->pValue );
		//	}
		//}

		switch( pTargetType->getType() )
		{
		case GenericDataTypeType_Enum:
			{
				string enumName;
				sint64 enumValue;
				if( value.getEnum( enumName, &enumValue ) )
				{
					const GenericDataTypeEnum* pEnumType = (const GenericDataTypeEnum*)value.getType();
					if( pEnumType != pTargetType )
					{
						TIKI_TRACE_ERROR( "Enum type '%s' doesn't match value type '%s'. Can't wrote enum value.\n", pEnumType->getName(), pTargetType->getName() );
						return false;
					}

					const GenericDataTypeValueType* pValueType = (const GenericDataTypeValueType*)pEnumType->getBaseType();

					GenericDataValue writeValue( pValueType );
					if( pValueType->isSignedInteger() )
					{
						if( writeValue.setSignedValue( enumValue, pValueType ) )
						{
							return writeValueToResource( sectionWriter, pValueType, writeValue );
						}
					}
					else if( pValueType->isUnsignedInteger() )
					{
						if( writeValue.setUnsignedValue( (uint64)enumValue, pValueType ) )
						{
							return writeValueToResource( sectionWriter, pValueType, writeValue );
						}
					}
				}
			}
			break;

		case GenericDataTypeType_Struct:
			{
				const GenericDataObject* pObject = nullptr;
				if( !value.getObject( pObject ) ||
					pObject == nullptr )
				{
					const GenericDataTypeStruct* pStructType = (const GenericDataTypeStruct*)pTargetType;
					pObject = pStructType->getDefaultObject();
				}

				if( pObject != nullptr &&
					pObject->writeToResource( nullptr, sectionWriter ) )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;

		case GenericDataTypeType_ValueType:
			{
				const GenericDataTypeValueType* pValueType = (const GenericDataTypeValueType*)pTargetType;

				switch( pValueType->getValueType() )
				{
				case GenericDataTypeValueTypeType_Boolean:
					{
						bool b;
						if( value.getBoolean( b ) )
						{
							sectionWriter.writeUInt8( b );
							return true;
						}
					}
					break;

				case GenericDataTypeValueTypeType_SingedInteger8:
				case GenericDataTypeValueTypeType_SingedInteger16:
				case GenericDataTypeValueTypeType_SingedInteger32:
				case GenericDataTypeValueTypeType_SingedInteger64:
					{
						sint64 s;
						if( value.getSignedValue( s ) )
						{
							switch( pValueType->getValueType() )
							{
							case GenericDataTypeValueTypeType_SingedInteger8:
								{
									sint8 s8;
									if( rangeCheckCast( s8, s ) )
									{
										sectionWriter.writeSInt8( s8 );
										return true;
									}
								}
								break;

							case GenericDataTypeValueTypeType_SingedInteger16:
								{
									sint16 s16;
									if( rangeCheckCast( s16, s ) )
									{
										sectionWriter.writeSInt16( s16 );
										return true;
									}
								}
								break;

							case GenericDataTypeValueTypeType_SingedInteger32:
								{
									sint32 s32;
									if( rangeCheckCast( s32, s ) )
									{
										sectionWriter.writeSInt32( s32 );
										return true;
									}
								}
								break;

							case GenericDataTypeValueTypeType_SingedInteger64:
								{
									sint64 s64;
									if( rangeCheckCast( s64, s ) )
									{
										sectionWriter.writeSInt64( s64 );
										return true;
									}
								}
								break;

							default:
								break;
							}
						}
					}
					break;

				case GenericDataTypeValueTypeType_UnsingedInteger8:
				case GenericDataTypeValueTypeType_UnsingedInteger16:
				case GenericDataTypeValueTypeType_UnsingedInteger32:
				case GenericDataTypeValueTypeType_UnsingedInteger64:
					{
						uint64 u;
						if( value.getUnsignedValue( u ) )
						{
							switch( pValueType->getValueType() )
							{
							case GenericDataTypeValueTypeType_UnsingedInteger8:
								{
									uint8 u8;
									if( rangeCheckCast( u8, u ) )
									{
										sectionWriter.writeUInt8( u8 );
										return true;
									}
								}
								break;

							case GenericDataTypeValueTypeType_UnsingedInteger16:
								{
									uint16 u16;
									if( rangeCheckCast( u16, u ) )
									{
										sectionWriter.writeUInt16( u16 );
										return true;
									}
								}
								break;

							case GenericDataTypeValueTypeType_UnsingedInteger32:
								{
									uint32 u32;
									if( rangeCheckCast( u32, u ) )
									{
										sectionWriter.writeUInt32( u32 );
										return true;
									}
								}
								break;

							case GenericDataTypeValueTypeType_UnsingedInteger64:
								{
									uint64 u64;
									if( rangeCheckCast( u64, u ) )
									{
										sectionWriter.writeUInt64( u64 );
										return true;
									}
								}
								break;

							default:
								break;
							}
						}
					}
					break;

				case GenericDataTypeValueTypeType_FloatingPoint16:
				case GenericDataTypeValueTypeType_FloatingPoint32:
				case GenericDataTypeValueTypeType_FloatingPoint64:
					{
						float64 f;
						if( value.getFloatingPoint( f ) )
						{
							switch( pValueType->getValueType() )
							{
							case GenericDataTypeValueTypeType_FloatingPoint16:
								{
									float16 f16;
									if( rangeCheckCast( f16, f ) )
									{
										sectionWriter.writeUInt16( f16 );
										return true;
									}
								}
								break;

							case GenericDataTypeValueTypeType_FloatingPoint32:
								{
									float32 f32;
									if( rangeCheckCast( f32, f ) )
									{
										sectionWriter.writeFloat( f32 );
										return true;
									}
								}
								break;

							case GenericDataTypeValueTypeType_FloatingPoint64:
								{
									float64 f64;
									if( rangeCheckCast( f64, f ) )
									{
										sectionWriter.writeDouble( f64 );
										return true;
									}
								}
								break;

							default:
								break;
							}
						}
					}
					break;

				case GenericDataTypeValueTypeType_String:
					{
						string text;
						if( value.getString( text ) )
						{
							const ReferenceKey key = sectionWriter.addString( text );
							sectionWriter.writeReference( &key );
							return true;
						}

					}
					break;

					case GenericDataTypeValueTypeType_Invalid:
					case GenericDataTypeValueTypeType_Count:
						break;
				}
			}
			break;

		case GenericDataTypeType_Resource:
			break;

		case GenericDataTypeType_Array:
			{
				const GenericDataArray* pArray = nullptr;
				if( !value.getArray( pArray ) )
				{
					TIKI_TRACE_ERROR( "[GenericDataContainer::writeValueToResource] Value is not an array.\n" );
					return false;
				}

				ReferenceKey key;
				if( pArray != nullptr && pArray->writeToResource( key, sectionWriter.getResourceWriter() ) )
				{
					sectionWriter.writeReference( &key );
					sectionWriter.writeUInt64( pArray->getCount() );
					return true;
				}
				else
				{
					sectionWriter.writeReference( nullptr );
					sectionWriter.writeUInt64( 0u );

					if( pArray == nullptr )
					{
						TIKI_TRACE_WARNING( "[GenericDataContainer::writeValueToResource] Write empty array. This can be indended but is no good idea.\n" );
						return true;
					}

					return false;
				}
			}
			break;

		case GenericDataTypeType_Reference:
			{
				string refText;
				if( !value.getReference( refText ) )
				{
					TIKI_TRACE_ERROR( "[GenericDataContainer::writeValueToResource] Value is not a reference.\n" );
					return false;
				}

				if( refText.isEmpty() )
				{
					sectionWriter.writeReference( nullptr );
					return true;
				}

				ReferenceKey key;
				if( readResourceReference( sectionWriter, refText, key ) )
				{
					sectionWriter.writeReference( &key );
					return true;
				}
			}
			break;

		case GenericDataTypeType_Pointer:
			{
				const GenericDataObject* pObject = nullptr;
				if( !value.getPointer( pObject ) )
				{
					return false;
				}

				ReferenceKey dataKey;
				bool ok = false;
				if( pObject != nullptr )
				{
					ok = pObject->writeToResource( &dataKey, sectionWriter );
				}

				sectionWriter.writeReference( ok ? &dataKey : nullptr );
				return ok;
			}
			break;

		case GenericDataTypeType_Invalid:
		case GenericDataTypeType_Count:
			TIKI_TRACE_ERROR( "[GenericDataContainer::writeValueToResource] Unsupported type.\n" );
			return false;
		}

		return false;
	}
#endif
}