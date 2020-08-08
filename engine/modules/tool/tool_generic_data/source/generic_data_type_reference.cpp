#include "tiki/tool_generic_data/generic_data_type_reference.hpp"

#include "tiki/tool_generic_data/generic_data_type_resource.hpp"

namespace tiki
{
	GenericDataTypeReference::GenericDataTypeReference( GenericDataTypeCollection& collection, const DynamicString& name, const DynamicString& filename, GenericDataTypeMode mode, const GenericDataTypeResource* pBaseType )
		: GenericDataType( collection, name, filename, GenericDataTypeType_Reference, mode )
		, m_pBaseType( pBaseType )
	{
		m_pTypeTag = TIKI_NEW( GenericDataTag );
		m_pTypeTag->setTag( "reference" );
		m_pTypeTag->setContent( pBaseType->getName() );
	}

	GenericDataTypeReference::~GenericDataTypeReference()
	{
	}

	bool GenericDataTypeReference::loadFromXml( XmlElement* pTypeNode )
	{
		return false;
	}

	bool GenericDataTypeReference::exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const
	{
		return false;
	}

	uint GenericDataTypeReference::getAlignment() const
	{
		return 8u;
	}

	uint GenericDataTypeReference::getSize() const
	{
		return 8u;
	}

	DynamicString GenericDataTypeReference::getCodeExportName() const
	{
		return m_pBaseType->getCodeExportName();
	}

	crc32 GenericDataTypeReference::getTypeCrc() const
	{
		return m_pBaseType->getTypeCrc();
	}

	const GenericDataTypeResource* GenericDataTypeReference::getBaseType() const
	{
		return m_pBaseType;
	}
}