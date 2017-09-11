#include "tiki/toolgenericdata/generic_data_type.hpp"

namespace tiki
{
	GenericDataType::GenericDataType( GenericDataTypeCollection& collection, const string& name, GenericDataTypeType type, GenericDataTypeMode mode )
		: m_collection( collection )
		, m_name( name )
		, m_mode( mode )
		, m_type( type )
	{
		m_pDefaultValue = TIKI_NEW( GenericDataValue )( this );
	}

	GenericDataType::~GenericDataType()
	{
		TIKI_DELETE( m_pDefaultValue );
	}

	const string& GenericDataType::getName() const
	{
		return m_name;
	}

	GenericDataTypeMode GenericDataType::getMode() const
	{
		return m_mode;
	}

	GenericDataTypeType GenericDataType::getType() const
	{
		return m_type;
	}

	const string& GenericDataType::getModule() const
	{
		return m_module;
	}

	void GenericDataType::setModule( const string& value )
	{
		m_module = value;
	}

	const GenericDataValue* GenericDataType::getDefaultValue() const
	{
		return m_pDefaultValue;
	}

	bool GenericDataType::isTypeCompatible( const GenericDataType* pType ) const
	{
		return this == pType;
	}
}