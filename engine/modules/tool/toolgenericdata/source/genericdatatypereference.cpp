
#include "tiki/toolgenericdata/genericdatatypereference.hpp"

#include "tiki/toolgenericdata/genericdatatyperesource.hpp"

namespace tiki
{
	GenericDataTypeReference::GenericDataTypeReference( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataTypeResource* pBaseType )
		: GenericDataType( collection, name, mode )
		, m_pBaseType( pBaseType )
	{
	}

	GenericDataTypeReference::~GenericDataTypeReference()
	{
	}

	bool GenericDataTypeReference::loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot )
	{
		return false;
	}

	bool GenericDataTypeReference::exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const
	{
		return false;
	}

	GenericDataTypeType GenericDataTypeReference::getType() const
	{
		return GenericDataTypeType_Reference;
	}

	uint GenericDataTypeReference::getAlignment() const
	{
		return 8u;
	}

	uint GenericDataTypeReference::getSize() const
	{
		return 8u;
	}

	string GenericDataTypeReference::getExportName() const
	{
		return m_pBaseType->getExportName();
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