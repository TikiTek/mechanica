#include "tiki/toolgenericdata/genericdatacontainer.hpp"

#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/genericdataarray.hpp"
#include "tiki/toolgenericdata/genericdataobject.hpp"
#include "tiki/toolgenericdata/genericdatatypearray.hpp"
#include "tiki/toolgenericdata/genericdatatypecollection.hpp"
#include "tiki/toolgenericdata/genericdatatypeenum.hpp"
#include "tiki/toolgenericdata/genericdatatypepointer.hpp"
#include "tiki/toolgenericdata/genericdatatypestruct.hpp"
#include "tiki/toolgenericdata/genericdatavalue.hpp"

#if TIKI_ENABLED( TIKI_GENERICDATA_CONVERTER )
#	include "tiki/converterbase/converterhelper.hpp"
#	include "tiki/converterbase/resourcewriter.hpp"
#endif

namespace tiki
{
	GenericDataContainer::GenericDataContainer( GenericDataTypeCollection& collection )
		: m_collection( collection )
	{
	}

	GenericDataContainer::~GenericDataContainer()
	{
	}

	bool GenericDataContainer::importFromXml( const XmlReader& reader, const _XmlElement* pObjectRootNode )
	{
		bool result = true;

		const char* pElementName = getElementName();
		const XmlElement* pElement = reader.findFirstChild( pElementName, pObjectRootNode );
		while ( pElement )
		{
			const XmlAttribute* pTypeAtt = reader.findAttributeByName( "type", pElement );

			if ( pTypeAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataContainer::importFromXml] '%s' node needs a 'type' attribute.\n", pElementName );
				result = false;
			}
			else
			{
				const GenericDataType* pElementType = m_collection.parseType( pTypeAtt->content );
				if ( pElementType == nullptr )
				{
					result = false;
				}
				else
				{
					GenericDataValue* pValue = addElementValue( reader, pElement );
					if( pValue == nullptr )
					{
						TIKI_TRACE_ERROR( "[GenericDataContainer::importFromXml] addElementValue failed.\n" );
					}
					else if ( !pElementType->loadValueFromXml( pValue, reader, pElement, getParentType() ) )
					{
						result = false;
					}
				}
			}

			pElement = reader.findNext( pElementName, pElement );
		}

		return true;
	}

	bool GenericDataContainer::writeValueToResource( ResourceWriter& writer, const GenericDataValue& value ) const
	{
#if TIKI_ENABLED( TIKI_GENERICDATA_CONVERTER )
		const GenericDataType* pType = value.getType();

		writer.writeAlignment( pType->getAlignment() );

		switch ( value.getValueType() )
		{
		case GenericDataValueType_Boolean:
			{
				bool b;
				if ( value.getBoolean( b ) )
				{
					writer.writeUInt8( b );
					return true;
				}
			}
			break;

		case GenericDataValueType_SingedInteger8:
		case GenericDataValueType_SingedInteger16:
		case GenericDataValueType_SingedInteger32:
		case GenericDataValueType_SingedInteger64:
			{
				sint64 s;
				if ( value.getSignedValue( s ) )
				{
					switch ( value.getValueType() )
					{
					case GenericDataValueType_SingedInteger8:
						{
							sint8 s8;
							if ( rangeCheckCast( s8, s ) )
							{
								writer.writeSInt8( s8 );
								return true;
							}
						}
						break;

					case GenericDataValueType_SingedInteger16:
						{
							sint16 s16;
							if ( rangeCheckCast( s16, s ) )
							{
								writer.writeSInt16( s16 );
								return true;
							}
						}
						break;

					case GenericDataValueType_SingedInteger32:
						{
							sint32 s32;
							if ( rangeCheckCast( s32, s ) )
							{
								writer.writeSInt32( s32 );
								return true;
							}
						}
						break;

					case GenericDataValueType_SingedInteger64:
						{
							sint64 s64;
							if ( rangeCheckCast( s64, s ) )
							{
								writer.writeSInt64( s64 );
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

		case GenericDataValueType_UnsingedInteger8:
		case GenericDataValueType_UnsingedInteger16:
		case GenericDataValueType_UnsingedInteger32:
		case GenericDataValueType_UnsingedInteger64:
			{
				uint64 u;
				if ( value.getUnsignedValue( u ) )
				{
					switch ( value.getValueType() )
					{
					case GenericDataValueType_UnsingedInteger8:
						{
							uint8 u8;
							if ( rangeCheckCast( u8, u ) )
							{
								writer.writeUInt8( u8 );
								return true;
							}
						}
						break;

					case GenericDataValueType_UnsingedInteger16:
						{
							uint16 u16;
							if ( rangeCheckCast( u16, u ) )
							{
								writer.writeUInt16( u16 );
								return true;
							}
						}
						break;

					case GenericDataValueType_UnsingedInteger32:
						{
							uint32 u32;
							if ( rangeCheckCast( u32, u ) )
							{
								writer.writeUInt32( u32 );
								return true;
							}
						}
						break;

					case GenericDataValueType_UnsingedInteger64:
						{
							uint64 u64;
							if ( rangeCheckCast( u64, u ) )
							{
								writer.writeUInt64( u64 );
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

		case GenericDataValueType_FloatingPoint16:
		case GenericDataValueType_FloatingPoint32:
		case GenericDataValueType_FloatingPoint64:
			{
				float64 f;
				if ( value.getFloatingPoint( f ) )
				{
					switch ( value.getValueType() )
					{
					case GenericDataValueType_FloatingPoint16:
						{
							float16 f16;
							if ( rangeCheckCast( f16, f ) )
							{
								writer.writeUInt16( f16 );
								return true;
							}
						}
						break;

					case GenericDataValueType_FloatingPoint32:
						{
							float32 f32;
							if ( rangeCheckCast( f32, f ) )
							{
								writer.writeFloat( f32 );
								return true;
							}
						}
						break;

					case GenericDataValueType_FloatingPoint64:
						{
							float64 f64;
							if ( rangeCheckCast( f64, f ) )
							{
								writer.writeDouble( f64 );
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

		case GenericDataValueType_String:
			{
				string t;
				if ( value.getString( t ) )
				{
					ReferenceKey key = writer.addString( StringType_UTF8, t );
					writer.writeReference( &key );
					return true;
				}

			}
			break;

		case GenericDataValueType_Object:
			{
				const GenericDataObject* pObject = nullptr;
				if ( value.getObject( (GenericDataObject*&)pObject ) )
				{
					if( pObject == nullptr )
					{
						if( pType->getType() != GenericDataTypeType_Struct )
						{
							return false;
						}
						const GenericDataTypeStruct* pStructType = (const GenericDataTypeStruct*)pType;
						pObject = pStructType->getDefaultObject();
					}

					if ( pObject != nullptr && pObject->writeToResource( nullptr, writer ) )
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			break;

		case GenericDataValueType_Array:
			{
				GenericDataArray* pArray = nullptr;
				if ( value.getArray( pArray ) )
				{
					ReferenceKey key;
					if ( pArray != nullptr && pArray->writeToResource( key, writer ) )
					{
						writer.writeReference( &key );
						writer.writeUInt64( pArray->getCount() );
						return true;
					}
					else
					{
						writer.writeReference( nullptr );
						writer.writeUInt64( 0u );

						if ( pArray == nullptr )
						{
							TIKI_TRACE_WARNING( "[GenericDataContainer::writeValueToResource] Write empty array. This can be indended but is no good idea.\n" );
							return true;
						}

						return false;
					}
				}
			}
			break;

		case GenericDataValueType_Enum:
			{
				string enumName;
				sint64 enumValue;
				if ( value.getEnum( enumName, enumValue ) )
				{
					const GenericDataTypeEnum* pEnumType = (const GenericDataTypeEnum*)value.getType();
					const GenericDataTypeValueType* pValueType = (const GenericDataTypeValueType*)pEnumType->getBaseType();

					GenericDataValue writeValue( pValueType );
					if ( pValueType->isSignedInteger() )
					{
						if ( writeValue.setSignedValue( enumValue, pValueType ) )
						{
							return writeValueToResource( writer, writeValue );
						}
					}
					else if ( pValueType->isUnsignedInteger() )
					{
						if ( writeValue.setUnsignedValue( (uint64)enumValue, pValueType ) )
						{
							return writeValueToResource( writer, writeValue );
						}
					}
				}
			}
			break;

		case GenericDataValueType_Reference:
			{
				string refText;
				if ( value.getReference( refText ) )
				{
					if( refText.isEmpty() )
					{
						writer.writeReference( nullptr );
						return true;
					}

					ReferenceKey key;
					if ( readResourceReference( writer, refText, key ) )
					{
						writer.writeReference( &key );
						return true;
					}
				}
			}
			break;

		case GenericDataValueType_Pointer:
			{
				if( pType->getType() != GenericDataTypeType_Pointer )
				{
					return false;
				}

				GenericDataObject* pObject = nullptr;
				if( !value.getPointer( pObject ) )
				{
					return false;
				}

				ReferenceKey dataKey;
				bool ok = false;
				if( pObject != nullptr )
				{
					ok = pObject->writeToResource( &dataKey, writer );
				}

				writer.writeReference( ok ? &dataKey : nullptr );
				return ok;
			}
			break;
		}
#endif

		return false;
	}
}