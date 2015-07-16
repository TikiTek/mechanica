
#include "tiki/genericdata/genericdatatypeunion.hpp"

namespace tiki
{
	GenericDataTypeUnion::GenericDataTypeUnion( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode )
		: GenericDataTypeStruct( collection, name, mode, nullptr )
	{
	}

	GenericDataTypeUnion::~GenericDataTypeUnion()
	{
	}

	bool GenericDataTypeUnion::loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot )
	{
		if ( !GenericDataTypeStruct::loadFromXml( reader, pTypeRoot ) )
		{
			return false;
		}

		m_size = 0;
		for (uint i = 0u; i < getFields().getCount(); ++i)
		{
			const GenericDataStructField& field = getFields()[ i ];

			if ( field.pType->getType() == GenericDataTypeType_Array )
			{
				TIKI_TRACE_ERROR( "[GenericDataUnion::loadFromXml] Union can't contains an array.\n" );
				return false;
			}

			m_size = TIKI_MAX( m_size, field.pType->getSize() );
		}

		return true;
	}

	GenericDataTypeType GenericDataTypeUnion::getType() const
	{
		return GenericDataTypeType_Union;
	}

	cstring GenericDataTypeUnion::getNodeName() const
	{
		return "union";
	}
}
