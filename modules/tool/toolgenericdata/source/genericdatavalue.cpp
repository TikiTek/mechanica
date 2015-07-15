
#include "tiki/genericdata/genericdatavalue.hpp"

namespace tiki
{
	GenericDataValue::GenericDataValue()
	{
	}

	GenericDataValue::GenericDataValue( const GenericDataType* pType )
	{
	}

	GenericDataValue::~GenericDataValue()
	{
	}

	const GenericDataType* GenericDataValue::getType() const
	{
		return m_pType;
	}

	void GenericDataValue::setUint8( uint8 value )
	{

	}

	void GenericDataValue::setString( const string& value )
	{

	}

	void GenericDataValue::setObject( const GenericDataObject* pValue )
	{

	}
}
