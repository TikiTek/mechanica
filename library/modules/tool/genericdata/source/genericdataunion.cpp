
#include "tiki/genericdata/genericdataunion.hpp"

namespace tiki
{
	GenericDataUnion::GenericDataUnion( const GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode )
		: GenericDataStruct( collection, name, mode, nullptr )
	{
	}

	GenericDataUnion::~GenericDataUnion()
	{
	}

	bool GenericDataUnion::loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot )
	{
		if ( !GenericDataStruct::loadFromXml( reader, pTypeRoot ) )
		{
			return false;
		}

		m_size = 0;
		for (uint i = 0u; i < getFields().getCount(); ++i)
		{
			const GenericDataStructField& field = getFields()[ i ];

			m_size = TIKI_MAX( m_size, field.pType->getSize() );
		}

		return true;
	}

	GenericDataTypeType GenericDataUnion::getType() const
	{
		return GenericDataTypeType_Union;
	}
}
