#include "tiki/tool_generic_data/generic_data_type.hpp"

namespace tiki
{
	GenericDataType::GenericDataType( GenericDataTypeCollection& collection, const DynamicString& name, const DynamicString& filename, GenericDataTypeType type, GenericDataTypeMode mode )
		: m_collection( collection )
		, m_name( name )
		, m_filename( filename )
		, m_mode( mode )
		, m_type( type )
	{
		m_pTypeTag		= nullptr;

		// value type creates it own default value to avoid access to uninitialized memory
		if( m_type != GenericDataTypeType_ValueType )
		{
			m_pDefaultValue = TIKI_NEW( GenericDataValue )(this);
		}
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
			pElement->setAttribute( "type", m_pTypeTag->toString().cStr() );
		}
		else
		{
			pElement->setAttribute( "type", m_name.cStr() );
		}

		return pElement;
	}
}