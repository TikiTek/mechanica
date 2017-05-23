
#include "tiki/toolgenericdata/genericdatatypeenum.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/genericdatatypecollection.hpp"

namespace tiki
{
	GenericDataTypeEnum::GenericDataTypeEnum( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataTypeValueType* pBaseType )
		: GenericDataType( collection, name, GenericDataTypeType_Enum, mode )
		, m_pBaseType( pBaseType )
	{
		if( m_pBaseType == nullptr )
		{
			m_pBaseType = m_collection.getEnumDefaultBaseType();
		}
	}

	GenericDataTypeEnum::~GenericDataTypeEnum()
	{
		for( uint i = 0u; i < m_values.getCount(); ++i )
		{
			TIKI_DELETE( m_values[ i ].pValue );
		}
	}

	bool GenericDataTypeEnum::loadFromXml( const XmlReader& reader, const XmlElement* pTypeRoot )
	{
		if ( pTypeRoot == nullptr )
		{
			return false;
		}

		if ( !isStringEquals( pTypeRoot->name, "enum" ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] node has a wrong tag('%s' != 'enum') \n", getName().cStr(), pTypeRoot->name );
			return false;
		}

		const XmlElement* pChildElement = pTypeRoot->elements;
		sint64 currentValue = 0;
		while ( pChildElement != nullptr )
		{
			const bool isValue = isStringEquals( pChildElement->name, "value" );
			if ( isValue )
			{
				const XmlAttribute* pNameAtt = reader.findAttributeByName( "name", pChildElement );
				const XmlAttribute* pModeAtt = reader.findAttributeByName( "mode", pChildElement );
				const XmlAttribute* pValueAtt = reader.findAttributeByName( "value", pChildElement );

				if ( pNameAtt )
				{
					GenericDataEnumValue& field = m_values.add();
					field.name		= pNameAtt->content;
					field.mode		= GenericDataTypeMode_ToolAndRuntime;
					field.hasValue	= false;
					field.pValue	= TIKI_NEW( GenericDataValue )( m_pBaseType );

					if ( pModeAtt != nullptr )
					{
						GenericDataTypeMode mode = m_collection.findModeByName( pModeAtt->content );
						if ( mode == GenericDataTypeMode_Invalid )
						{
							TIKI_TRACE_WARNING( "[GenericDataStruct(%s)::readFromXml] field or array with name '%s' has a invalid mode attribute. '%s' is not a valid mode.\n", getName().cStr(), pNameAtt->content, pModeAtt->content );
						}
						else
						{
							field.mode = mode;
						}
					}

					if ( pValueAtt != nullptr )
					{
						if( !m_collection.parseValue( field.pValue, pValueAtt->content, m_pBaseType, this ) )
						{
							TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] unable to parse value for enum value.\n", getName().cStr() );
							return false;
						}
						field.hasValue = true;

						if( !field.pValue->getSignedValue( currentValue ) )
						{
							TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] enum value is not an integer value.\n", getName().cStr() );
							return false;
						}
						currentValue++;
					}
					else
					{
						if( !field.pValue->setSignedValue( currentValue, m_pBaseType ) )
						{
							TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] unable to set default enum value.\n", getName().cStr() );
						}
					}
				}
				else
				{
					TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] enum value requires an name attribute.\n", getName().cStr() );
					return false;
				}
			}

			pChildElement = pChildElement->next;
		}

		return true;
	}

	bool GenericDataTypeEnum::exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const
	{
		static const char* s_pBaseFormat			= "\n"
													  "\tenum %s %s\n"
													  "\t{\n"
													  "%s"
													  "\n"
													  "\t\t%s_Count,\n"
													  "\t\t%s_Invalid = %s\n"
													  "\t};\n";

		static const char* s_pValueFormat			= "\t\t%s_%s,\n";
		static const char* s_pValueFormatWithValue	= "\t\t%s_%s = %d,\n";

		string valuesCode;
		for (uint i = 0u; i < m_values.getCount(); ++i)
		{
			const GenericDataEnumValue& value = m_values[ i ];

			if ( value.hasValue )
			{
				sint64 intValue = 0u;
				TIKI_VERIFY( value.pValue->getSignedValue( intValue ) );

				valuesCode += formatDynamicString( s_pValueFormatWithValue, getName().cStr(), value.name.cStr(), intValue );
			}
			else
			{
				valuesCode += formatDynamicString( s_pValueFormat, getName().cStr(), value.name.cStr() );
			}
		}

		string invalidValue = "-1";
		if( m_pBaseType->isUnsignedInteger() )
		{
			const uint64 maxValue = (1ull << (m_pBaseType->getSize() * 8u)) - 1ull;

			string format = formatDynamicString( "0x%%0%ix", m_pBaseType->getSize() * 2u );
			invalidValue = formatDynamicString( format.cStr(), maxValue );
		}

		string baseString = "";
		if( m_pBaseType != m_collection.getEnumDefaultBaseType() )
		{
			baseString = ": " + m_pBaseType->getExportName();
		}

		targetData.code += formatDynamicString(
			s_pBaseFormat,
			getExportName().cStr(),
			baseString.cStr(),
			valuesCode.cStr(),
			getExportName().cStr(),
			getExportName().cStr(),
			invalidValue.cStr()
		);

		return true;
	}

	uint GenericDataTypeEnum::getAlignment() const
	{
		if( m_pBaseType == nullptr )
		{
			return 4u;
		}

		return m_pBaseType->getAlignment();
	}

	uint GenericDataTypeEnum::getSize() const
	{
		if( m_pBaseType == nullptr )
		{
			return 4u;
		}

		return m_pBaseType->getSize();
	}

	string GenericDataTypeEnum::getExportName() const
	{
		return getName();
	}

	crc32 GenericDataTypeEnum::getTypeCrc() const
	{
		string typeString;
		for (size_t i = 0u; i < m_values.getCount(); ++i)
		{
			typeString += m_values[ i ].name;
		}

		return crcString( typeString );
	}

	const GenericDataType* GenericDataTypeEnum::getBaseType() const
	{
		return m_pBaseType;
	}

	void GenericDataTypeEnum::addValue( const string& name, sint64 value, GenericDataTypeMode mode /*= GenericDataTypeMode_ToolAndRuntime */ )
	{
		GenericDataEnumValue& field = m_values.add();
		field.name		= name;
		field.hasValue	= true;
		field.pValue	= TIKI_NEW( GenericDataValue )( m_pBaseType );
		field.mode		= mode;
	}

	void GenericDataTypeEnum::removeValue( const string& name )
	{
		for (uint i = 0u; i < m_values.getCount(); ++i)
		{
			if ( m_values[ i ].name == name )
			{
				m_values.removeSortedAtIndex( i );
				return;
			}
		}
	}

	GenericDataValue* GenericDataTypeEnum::getValueByName( const string& name )
	{
		for( uint i = 0u; i < m_values.getCount(); ++i )
		{
			if( m_values[ i ].name == name )
			{
				return m_values[ i ].pValue;
			}
		}

		return nullptr;
	}

	const GenericDataValue* GenericDataTypeEnum::getValueByName( const string& name ) const
	{
		return const_cast<GenericDataTypeEnum*>(this)->getValueByName( name );
	}

	const List< GenericDataEnumValue >& GenericDataTypeEnum::getValues() const
	{
		return m_values;
	}

	bool GenericDataTypeEnum::isValidValue( const GenericDataEnumValue& enumValue ) const
	{
		for( const GenericDataEnumValue& value : m_values )
		{
			if( value.name == enumValue.name )
			{
				return true;
			}
		}

		return false;
	}

}
