
#include "tiki/genericdata/genericdatavaluetype.hpp"

namespace tiki
{
	GenericDataValueType::GenericDataValueType( const GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, GenericDataValueTypeType type )
		: GenericDataType( collection, name, mode )
	{
		m_type = type;
	}

	GenericDataValueType::~GenericDataValueType()
	{
	}

	bool GenericDataValueType::loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot )
	{
		return false;
	}

	bool GenericDataValueType::exportCode( string& targetData, GenericDataTypeMode mode, const string& targetDir )
	{
		return true;
	}

	GenericDataTypeType GenericDataValueType::getType() const
	{
		return GenericDataTypeType_ValueType;
	}

	uint GenericDataValueType::getAlignment() const
	{
		return getSize();
	}

	uint GenericDataValueType::getSize() const
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
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aSizes ) == GenericDataValueTypeType_Count );

		return s_aSizes[ m_type ];
	}
}
