
#include "tiki/toolgenericdata/genericdatatype.hpp"

namespace tiki
{
	GenericDataType::GenericDataType( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode )
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

	bool GenericDataType::isTypeCompatible( const GenericDataType* pType ) const
	{
		return this == pType;
	}

	const string& GenericDataType::getModule() const
	{
		return m_module;
	}

	void GenericDataType::setModule( const string& value )
	{
		m_module = value;
	}

	GenericDataTypeMode GenericDataType::getMode() const
	{
		return m_mode;
	}
}