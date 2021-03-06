#include "tiki/tool_generic_data/generic_data_type_value_type.hpp"

#include "tiki/base/crc32.hpp"

namespace tiki
{
	GenericDataTypeValueType::GenericDataTypeValueType( GenericDataTypeCollection& collection, const DynamicString& name, const DynamicString& filename, GenericDataTypeMode mode, GenericDataValueTypeType type )
		: GenericDataType( collection, name, filename, GenericDataTypeType_ValueType, mode )
		, m_type( type )
	{
		m_pDefaultValue = TIKI_NEW( GenericDataValue )( this );
	}

	GenericDataTypeValueType::~GenericDataTypeValueType()
	{
	}

	bool GenericDataTypeValueType::loadFromXml( XmlElement* pTypeNode )
	{
		return false;
	}

	bool GenericDataTypeValueType::exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const
	{
		return false;
	}

	uint GenericDataTypeValueType::getAlignment() const
	{
		return getSize();
	}

	uint GenericDataTypeValueType::getSize() const
	{
		static const uint s_aSizes[] =
		{
			1,	// GenericDataValueTypeType_Boolean
			1,	// GenericDataValueTypeType_SingedInteger8
			2,	// GenericDataValueTypeType_SingedInteger16
			4,	// GenericDataValueTypeType_SingedInteger32
			8,	// GenericDataValueTypeType_SingedInteger64
			1,	// GenericDataValueTypeType_UnsingedInteger8
			2,	// GenericDataValueTypeType_UnsingedInteger16
			4,	// GenericDataValueTypeType_UnsingedInteger32
			8,	// GenericDataValueTypeType_UnsingedInteger64
			2,	// GenericDataValueTypeType_FloatingPoint16
			4,	// GenericDataValueTypeType_FloatingPoint32
			8,	// GenericDataValueTypeType_FloatingPoint64
			8,	// GenericDataValueTypeType_String
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aSizes ) == GenericDataTypeValueTypeType_Count );

		return s_aSizes[ m_type ];
	}

	DynamicString GenericDataTypeValueType::getCodeExportName() const
	{
		if( m_type == GenericDataTypeValueTypeType_String )
		{
			return "ResRef< char >";
		}

		return getName();
	}

	crc32 GenericDataTypeValueType::getTypeCrc() const
	{
		return crcString( getName() );
	}

	GenericDataValueTypeType GenericDataTypeValueType::getValueType() const
	{
		return m_type;
	}

	bool GenericDataTypeValueType::isBoolean() const
	{
		return m_type == GenericDataTypeValueTypeType_Boolean;
	}

	bool GenericDataTypeValueType::isSignedInteger() const
	{
		return m_type == GenericDataTypeValueTypeType_SingedInteger8 ||
			   m_type == GenericDataTypeValueTypeType_SingedInteger16 ||
			   m_type == GenericDataTypeValueTypeType_SingedInteger32 ||
			   m_type == GenericDataTypeValueTypeType_SingedInteger64;
	}

	bool GenericDataTypeValueType::isUnsignedInteger() const
	{
		return m_type == GenericDataTypeValueTypeType_UnsingedInteger8 ||
			   m_type == GenericDataTypeValueTypeType_UnsingedInteger16 ||
			   m_type == GenericDataTypeValueTypeType_UnsingedInteger32 ||
			   m_type == GenericDataTypeValueTypeType_UnsingedInteger64;
	}

	bool GenericDataTypeValueType::isFloatingPoint() const
	{
		return m_type == GenericDataTypeValueTypeType_FloatingPoint16 ||
			   m_type == GenericDataTypeValueTypeType_FloatingPoint32 ||
			   m_type == GenericDataTypeValueTypeType_FloatingPoint64;
	}

	bool GenericDataTypeValueType::isString() const
	{
		return m_type == GenericDataTypeValueTypeType_String;
	}
}
