#include "tiki/toolgenericdata/genericdatatypepointer.hpp"

#include "tiki/toolgenericdata/genericdatatypestruct.hpp"

namespace tiki
{
	GenericDataTypePointer::GenericDataTypePointer( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataTypeStruct* pBaseType )
		: GenericDataType( collection, name, GenericDataTypeType_Pointer, mode )
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
		return formatDynamicString( "ResRef< %s >", m_pBaseType->getExportName().cStr() );
	}

	crc32 GenericDataTypePointer::getTypeCrc() const
	{
		return m_pBaseType->getTypeCrc();
	}

	bool GenericDataTypePointer::isTypeCompatible( const GenericDataType* pType ) const
	{
		if( GenericDataType::isTypeCompatible( pType ) || m_pBaseType->isTypeCompatible( pType ) )
		{
			return true;
		}

		if( pType->getType() != GenericDataTypeType_Struct )
		{
			return false;
		}

		const GenericDataTypeStruct* pOtherType = (const GenericDataTypeStruct*)pType;
		return m_pBaseType->isTypeCompatible( pOtherType->getBaseType() );
	}

	const GenericDataTypeStruct* GenericDataTypePointer::getBaseType() const
	{
		return m_pBaseType;
	}
}