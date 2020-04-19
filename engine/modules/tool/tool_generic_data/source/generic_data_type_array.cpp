#include "tiki/tool_generic_data/generic_data_type_array.hpp"

#include "tiki/tool_generic_data/generic_data_tag.hpp"

namespace tiki
{
	GenericDataTypeArray::GenericDataTypeArray( GenericDataTypeCollection& collection, const DynamicString& name, const DynamicString& filename, GenericDataTypeMode mode, const GenericDataType* pBaseType )
		: GenericDataType( collection, name, filename, GenericDataTypeType_Array, mode )
		, m_pBaseType( pBaseType )
	{
		m_pTypeTag = TIKI_NEW( GenericDataTag );
		m_pTypeTag->setTag( "array" );
		m_pTypeTag->setContent( pBaseType->getName() );
	}

	GenericDataTypeArray::~GenericDataTypeArray()
	{
	}

	bool GenericDataTypeArray::loadFromXml( XmlElement* pTypeNode )
	{
		return false;
	}

	bool GenericDataTypeArray::exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const
	{
		return false;
	}

	uint GenericDataTypeArray::getAlignment() const
	{
		return 8u;
	}

	uint GenericDataTypeArray::getSize() const
	{
		return 16u;
	}

	DynamicString GenericDataTypeArray::getCodeExportName() const
	{
		return formatDynamicString( "ResArray< %s >", m_pBaseType->getCodeExportName().cStr() );
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
