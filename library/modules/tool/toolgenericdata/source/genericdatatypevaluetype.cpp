
#include "tiki/toolgenericdata/genericdatatypevaluetype.hpp"

namespace tiki
{
	GenericDataTypeValueType::GenericDataTypeValueType( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, GenericDataValueTypeType type )
		: GenericDataType( collection, name, mode )
	{
		m_type = type;
	}

	GenericDataTypeValueType::~GenericDataTypeValueType()
	{
	}

	bool GenericDataTypeValueType::loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot )
	{
		return false;
	}

	bool GenericDataTypeValueType::exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const
	{
		return false;
	}

	GenericDataTypeType GenericDataTypeValueType::getType() const
	{
		return GenericDataTypeType_ValueType;
	}

	uint GenericDataTypeValueType::getAlignment() const
	{
		return getSize();
	}

	uint GenericDataTypeValueType::getSize() const
	{
		static const uint s_aSizes[] =
		{
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
			1,	// GenericDataValueTypeType_Boolean
			8,	// GenericDataValueTypeType_String
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aSizes ) == GenericDataTypeValueTypeType_Count );

		return s_aSizes[ m_type ];
	}

	string GenericDataTypeValueType::getExportName() const
	{
		return getName();
	}
}
