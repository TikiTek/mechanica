#include "tiki/toolgenericdata/genericdatatypepointer.hpp"

#include "tiki/toolgenericdata/genericdatatypestruct.hpp"

namespace tiki
{
	GenericDataTypePointer::GenericDataTypePointer( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataTypeStruct* pBaseType )
		: GenericDataType( collection, name, mode )
		, m_pBaseType( pBaseType )
	{
	}

	GenericDataTypePointer::~GenericDataTypePointer()
	{
	}

	bool GenericDataTypePointer::loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot )
	{
		return false;
	}

	bool GenericDataTypePointer::exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const
	{
		return false;
	}

	GenericDataTypeType GenericDataTypePointer::getType() const
	{
		return GenericDataTypeType_Pointer;
	}

	uint GenericDataTypePointer::getAlignment() const
	{
		return 8u;
	}

	uint GenericDataTypePointer::getSize() const
	{
		return 8u;
	}

	string GenericDataTypePointer::getExportName() const
	{
		return formatString( "ResRef< %s >", m_pBaseType->getExportName().cStr() );
	}

	crc32 GenericDataTypePointer::getTypeCrc() const
	{
		return m_pBaseType->getTypeCrc();
	}

	const GenericDataTypeStruct* GenericDataTypePointer::getBaseType() const
	{
		return m_pBaseType;
	}
}