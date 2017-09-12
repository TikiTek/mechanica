#include "tiki/toolgenericdata/generic_data_type.hpp"

namespace tiki
{
	GenericDataType::GenericDataType( GenericDataTypeCollection& collection, const string& name, GenericDataTypeType type, GenericDataTypeMode mode )
		: m_collection( collection )
		, m_name( name )
		, m_mode( mode )
		, m_type( type )
	{
		m_pTypeTag		= nullptr;
		m_pDefaultValue = TIKI_NEW( GenericDataValue )( this );
	}

	GenericDataType::~GenericDataType()
	{
		if( m_pTypeTag != nullptr )
		{
			TIKI_DELETE( m_pTypeTag );
		}

		TIKI_DELETE( m_pDefaultValue );
	}

	XmlElement* GenericDataType::createXmlElement( XmlDocument& document, const char* pName ) const
	{
		XmlElement* pElement = document.createElement( pName );

		if( m_pTypeTag != nullptr )
		{
			pElement->setAttribute( "type", m_pTypeTag->writeTagString().cStr() );
		}
		else
		{
			pElement->setAttribute( "type", m_name.cStr() );
		}

		return pElement;
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