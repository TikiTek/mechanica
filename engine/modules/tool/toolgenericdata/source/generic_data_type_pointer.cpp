#include "tiki/toolgenericdata/generic_data_type_pointer.hpp"

#include "tiki/toolgenericdata/generic_data_type_struct.hpp"

namespace tiki
{
	GenericDataTypePointer::GenericDataTypePointer( GenericDataTypeCollection& collection, const string& name, const string& filename, GenericDataTypeMode mode, const GenericDataTypeStruct* pBaseType )
		: GenericDataType( collection, name, filename, GenericDataTypeType_Pointer, mode )
		, m_pBaseType( pBaseType )
	{
		m_pTypeTag = TIKI_NEW( GenericDataTag );
		m_pTypeTag->setTag( "pointer" );
		m_pTypeTag->setContent( pBaseType->getName() );
	}

	GenericDataTypePointer::~GenericDataTypePointer()
	{
	}

	bool GenericDataTypePointer::loadFromXml( XmlElement* pTypeNode )
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

	string GenericDataTypePointer::getCodeExportName() const
	{
		return formatDynamicString( "ResRef< %s >", m_pBaseType->getCodeExportName().cStr() );
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