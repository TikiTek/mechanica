
#include "tiki/genericdata/genericdatatype.hpp"

namespace tiki
{
	GenericDataType::GenericDataType( const GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode )
		: m_collection( collection )
		, m_name( name )
		, m_mode( mode )
	{
	}

	GenericDataType::~GenericDataType()
	{
	}

	const string& GenericDataType::getName() const
	{
		return m_name;
	}

	GenericDataTypeMode GenericDataType::getMode() const
	{
		return m_mode;
	}
}