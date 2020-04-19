#include "tiki/tool_generic_data/generic_data_type_enum.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/tool_generic_data/generic_data_type_collection.hpp"

namespace tiki
{
	GenericDataTypeEnum::GenericDataTypeEnum( GenericDataTypeCollection& collection, const DynamicString& name, const DynamicString& filename, GenericDataTypeMode mode, const GenericDataTypeValueType* pBaseType )
		: GenericDataType( collection, name, filename, GenericDataTypeType_Enum, mode )
		, m_pBaseType( pBaseType )
	{
		if( m_pBaseType == nullptr )
		{
			m_pBaseType = m_collection.getEnumDefaultBaseType();
		}
	}

	GenericDataTypeEnum::~GenericDataTypeEnum()
	{
		for( uintreg i = 0u; i < m_values.getCount(); ++i )
		{
			TIKI_DELETE( m_values[ i ].pValue );
		}
	}

	bool GenericDataTypeEnum::loadFromXml( XmlElement* pTypeNode )
	{
		if ( pTypeNode == nullptr )
		{
			return false;
		}

		if ( !pTypeNode->isName( "enum" ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] node has a wrong tag('%s' != 'enum') \n", getName().cStr(), pTypeNode->getName() );
			return false;
		}

		const XmlElement* pChildNode = pTypeNode->getFirstChild();
		sint64 currentValue = 0;
		while ( pChildNode != nullptr )
		{
			const bool isValue = pChildNode->isName( "value" );
			if ( isValue )
			{
				const XmlAttribute* pNameAtt = pChildNode->findAttribute( "name" );
				const XmlAttribute* pModeAtt = pChildNode->findAttribute( "mode" );
				const XmlAttribute* pValueAtt = pChildNode->findAttribute( "value" );

				if ( pNameAtt )
				{
					GenericDataEnumValue& field = m_values.add();
					field.name		= pNameAtt->getValue();
					field.mode		= GenericDataTypeMode_ToolAndRuntime;
					field.hasValue	= false;
					field.pValue	= TIKI_NEW( GenericDataValue )( m_pBaseType );

					if ( pModeAtt != nullptr )
					{
						GenericDataTypeMode mode = m_collection.findModeByName( pModeAtt->getValue() );
						if ( mode == GenericDataTypeMode_Invalid )
						{
							TIKI_TRACE_WARNING( "[GenericDataStruct(%s)::readFromXml] field or array with name '%s' has a invalid mode attribute. '%s' is not a valid mode.\n", getName().cStr(), pNameAtt->getValue(), pModeAtt->getValue() );
						}
						else
						{
							field.mode = mode;
						}
					}

					if ( pValueAtt != nullptr )
					{
						if( !m_collection.parseValue( field.pValue, pValueAtt->getValue(), m_pBaseType, this ) )
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
					}
					else
					{
						if( !field.pValue->setSignedValue( currentValue, m_pBaseType ) )
						{
							TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] unable to set default enum value.\n", getName().cStr() );
						}
					}

					currentValue++;
				}
				else
				{
					TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] enum value requires an name attribute.\n", getName().cStr() );
					return false;
				}
			}

			pChildNode = pChildNode->getNextSibling();
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

		DynamicString valuesCode;
		for( uintreg i = 0u; i < m_values.getCount(); ++i )
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

		DynamicString invalidValue = "-1";
		if( m_pBaseType->isUnsignedInteger() )
		{
			const uint64 maxValue = (1ull << (m_pBaseType->getSize() * 8u)) - 1ull;

			DynamicString format = formatDynamicString( "0x%%0%ix", m_pBaseType->getSize() * 2u );
			invalidValue = formatDynamicString( format.cStr(), maxValue );
		}

		DynamicString baseString = "";
		if( m_pBaseType != m_collection.getEnumDefaultBaseType() )
		{
			baseString = ": " + m_pBaseType->getCodeExportName();
		}

		targetData.code += formatDynamicString(
			s_pBaseFormat,
			getCodeExportName().cStr(),
			baseString.cStr(),
			valuesCode.cStr(),
			getCodeExportName().cStr(),
			getCodeExportName().cStr(),
			invalidValue.cStr()
		);

		return true;
	}

	uintreg GenericDataTypeEnum::getAlignment() const
	{
		if( m_pBaseType == nullptr )
		{
			return 4u;
		}

		return m_pBaseType->getAlignment();
	}

	uintreg GenericDataTypeEnum::getSize() const
	{
		if( m_pBaseType == nullptr )
		{
			return 4u;
		}

		return m_pBaseType->getSize();
	}

	DynamicString GenericDataTypeEnum::getCodeExportName() const
	{
		return getName();
	}

	crc32 GenericDataTypeEnum::getTypeCrc() const
	{
		DynamicString typeString;
		for (uintreg i = 0u; i < m_values.getCount(); ++i)
		{
			typeString += m_values[ i ].name;
		}

		return crcString( typeString );
	}

	const GenericDataType* GenericDataTypeEnum::getBaseType() const
	{
		return m_pBaseType;
	}

	void GenericDataTypeEnum::addValue( const DynamicString& name, sint64 value, GenericDataTypeMode mode /*= GenericDataTypeMode_ToolAndRuntime */ )
	{
		GenericDataEnumValue& field = m_values.add();
		field.name		= name;
		field.hasValue	= true;
		field.pValue	= TIKI_NEW( GenericDataValue )( m_pBaseType );
		field.mode		= mode;
	}

	void GenericDataTypeEnum::removeValue( const DynamicString& name )
	{
		for (uintreg i = 0u; i < m_values.getCount(); ++i)
		{
			if ( m_values[ i ].name == name )
			{
				m_values.removeSortedAtIndex( i );
				return;
			}
		}
	}

	GenericDataValue* GenericDataTypeEnum::getValueByName( const DynamicString& name )
	{
		for( uintreg i = 0u; i < m_values.getCount(); ++i )
		{
			if( m_values[ i ].name == name )
			{
				return m_values[ i ].pValue;
			}
		}

		return nullptr;
	}

	const GenericDataValue* GenericDataTypeEnum::getValueByName( const DynamicString& name ) const
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
