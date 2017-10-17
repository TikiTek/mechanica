#include "tiki/toolgenericdata/generic_data_array.hpp"

#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/generic_data_object.hpp"
#include "tiki/toolgenericdata/generic_data_type_array.hpp"
#include "tiki/toolgenericdata/generic_data_type_collection.hpp"
#include "tiki/toolgenericdata/generic_data_type_struct.hpp"
#include "tiki/toolgenericdata/generic_data_value.hpp"

#if TIKI_ENABLED( TIKI_GENERICDATA_CONVERTER )
#	include "tiki/converterbase/resource_writer.hpp"
#	include "tiki/converterbase/resource_section_writer.hpp"
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
		TIKI_ASSERT( m_array.isEmpty() );
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
			TIKI_DELETE( m_array[ i ] );
		}
		m_array.clear();
	}

	const GenericDataTypeArray* GenericDataArray::getType() const
	{
		return m_pType;
	}

	uint GenericDataArray::getCount() const
	{
		return m_array.getCount();
	}

	GenericDataValue* GenericDataArray::addElement()
	{
		GenericDataValue* pValue = TIKI_NEW( GenericDataValue )( m_pType->getBaseType() );
		m_array.add( pValue );

		return pValue;
	}

	GenericDataValue* GenericDataArray::getElement( uint index )
	{
		return m_array[ index ];
	}

	const GenericDataValue* GenericDataArray::getElement( uint index ) const
	{
		return m_array[ index ];
	}

	bool GenericDataArray::removeElement( uint index )
	{
		if ( index >= m_array.getCount() )
		{
			TIKI_TRACE_ERROR( "[GenericDataArray::removeElement] Index is greater as the size(%i > %i).\n", index, m_array.getCount() );
			return false;
		}

		GenericDataValue* pValue = m_array[ index ];
		TIKI_DELETE( pValue );

		m_array.removeSortedAtIndex( index );
		return true;
	}

#if TIKI_ENABLED( TIKI_GENERICDATA_CONVERTER )
	bool GenericDataArray::writeToResource( ReferenceKey& dataKey, ResourceWriter& writer ) const
	{
		ResourceSectionWriter sectionWriter;
		writer.openDataSection( sectionWriter, SectionType_Main );

		dataKey = sectionWriter.addDataPoint();

		bool ok = true;
		for (uint i = 0u; i < m_array.getCount(); ++i)
		{
			const GenericDataValue* pValue = m_array[ i ];
			ok &= writeValueToResource( sectionWriter, *pValue );
		}

		writer.closeDataSection( sectionWriter );
		return ok;
	}
#endif

	bool GenericDataArray::initializeXmlElementForValue( XmlElement* pElement, const GenericDataValue* pValue ) const
	{
		return true;
	}

	const char* GenericDataArray::getNodeName() const
	{
		return "array";
	}

	const char* GenericDataArray::getElementName() const
	{
		return "element";
	}

	const GenericDataType* GenericDataArray::getParentType() const
	{
		return m_pType;
	}

	GenericDataValue* GenericDataArray::addElementValue( const XmlElement* pNode )
	{
		return addElement();
	}

	GenericDataValue* GenericDataArray::getElementValue( uint index )
	{
		if( index >= m_array.getCount() )
		{
			return nullptr;
		}

		return getElement( index );
	}
}