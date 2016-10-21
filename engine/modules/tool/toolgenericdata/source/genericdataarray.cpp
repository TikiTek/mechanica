
#include "tiki/toolgenericdata/genericdataarray.hpp"

#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/genericdataobject.hpp"
#include "tiki/toolgenericdata/genericdatatypearray.hpp"
#include "tiki/toolgenericdata/genericdatatypecollection.hpp"
#include "tiki/toolgenericdata/genericdatatypestruct.hpp"
#include "tiki/toolgenericdata/genericdatavalue.hpp"

#if TIKI_ENABLED( TIKI_GENERICDATA_CONVERTER )
#	include "tiki/converterbase/resourcewriter.hpp"
#endif

namespace tiki
{
	GenericDataArray::GenericDataArray( GenericDataTypeCollection& collection )
		: GenericDataContainer( collection )
	{
		m_pType = nullptr;
	}

	GenericDataArray::~GenericDataArray()
	{
	}

	bool GenericDataArray::create( const GenericDataTypeArray* pType )
	{
		if ( pType == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataArray::createFromType] Null type not allowed.\n" );
			return false;
		}

		if ( m_pType != nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataArray::create] Dublicate create is not allowed.\n" );
			return false;
		}
	

		m_pType	= pType;

		return true;
	}

	void GenericDataArray::dispose()
	{
		for (uint i = 0u; i < m_array.getCount(); ++i)
		{
			GenericDataValue& element = m_array[ i ];
			element.dispose();
		}
	}

	const GenericDataTypeArray* GenericDataArray::getType() const
	{
		return m_pType;
	}
	
	uint GenericDataArray::getCount() const
	{
		return m_array.getCount();
	}

	GenericDataValue& GenericDataArray::getElement( uint index )
	{
		return m_array[ index ];
	}

	const GenericDataValue& GenericDataArray::getElement( uint index ) const
	{
		return m_array[ index ];
	}

	bool GenericDataArray::addElement( const GenericDataValue& value )
	{
		m_array.add();

		return setElement( m_array.getCount() - 1u, value );
	}

	bool GenericDataArray::setElement( uint index, const GenericDataValue& value )
	{
		if ( index >= m_array.getCount() )
		{
			TIKI_TRACE_ERROR( "[GenericDataArray::setElement] Index is greater as the size(%i > %i).\n", index, m_array.getCount() );
			return false;
		}

		if ( value.getType() != m_pType->getBaseType() )
		{
			TIKI_TRACE_ERROR( "[GenericDataArray::setElement] Can't assign value with different type('%s' != '%s')!\n", value.getType()->getName().cStr(), m_pType->getBaseType()->getName().cStr() );
			return false;
		}

		m_array[ index ] = value;

		return true;
	}

	bool GenericDataArray::removeElement( uint index )
	{
		if ( index >= m_array.getCount() )
		{
			TIKI_TRACE_ERROR( "[GenericDataArray::removeElement] Index is greater as the size(%i > %i).\n", index, m_array.getCount() );
			return false;
		}

		m_array[ index ].dispose();
		m_array.removeSortedAtIndex( index );

		return true;
	}

	bool GenericDataArray::writeToResource( ReferenceKey& dataKey, ResourceWriter& writer ) const
	{
#if TIKI_ENABLED( TIKI_GENERICDATA_CONVERTER )
		writer.openDataSection( 0u, AllocatorType_MainMemory );
		dataKey = writer.addDataPoint();

		bool ok = true;
		for (uint i = 0u; i < m_array.getCount(); ++i)
		{
			const GenericDataValue& value = m_array[ i ];
			ok &= writeValueToResource( writer, value );
		}

		writer.closeDataSection();

		return ok;
#else
		return false;
#endif
	}

	const char* GenericDataArray::getElementName() const
	{
		return "element";
	}

	const GenericDataType* GenericDataArray::getParentType() const
	{
		return m_pType;
	}

	bool GenericDataArray::applyElementValue( const XmlReader& reader, const _XmlElement* pElement, const GenericDataValue& value )
	{
		return addElement( value );
	}
}