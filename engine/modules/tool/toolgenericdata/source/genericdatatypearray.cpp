
#include "tiki/toolgenericdata/genericdatatypearray.hpp"

namespace tiki
{
	GenericDataTypeArray::GenericDataTypeArray( GenericDataTypeCollection& collection, const string& name, const GenericDataType* pBaseType, GenericDataTypeMode mode )
		: GenericDataType( collection, name, mode )
		, m_pBaseType( pBaseType )
	{
	}

	GenericDataTypeArray::~GenericDataTypeArray()
	{
	}

	bool GenericDataTypeArray::loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot )
	{
		return false;
	}

	bool GenericDataTypeArray::exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const
	{
		return false;
	}

	GenericDataTypeType GenericDataTypeArray::getType() const
	{
		return GenericDataTypeType_Array;
	}

	uint GenericDataTypeArray::getAlignment() const
	{
		return 8u;
	}

	uint GenericDataTypeArray::getSize() const
	{
		return 16u;
	}

	string GenericDataTypeArray::getExportName() const
	{
		return formatString( "ResArray< %s >", m_pBaseType->getExportName().cStr() );
	}

	crc32 GenericDataTypeArray::getTypeCrc() const
	{
		return m_pBaseType->getTypeCrc();
	}

	const GenericDataType* GenericDataTypeArray::getBaseType() const
	{
		return m_pBaseType;
	}
}
