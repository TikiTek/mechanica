#include "tiki/tool_generic_data/generic_data_array.hpp"

#include "tiki/tool_generic_data/generic_data_object.hpp"
#include "tiki/tool_generic_data/generic_data_type_array.hpp"
#include "tiki/tool_generic_data/generic_data_type_collection.hpp"
#include "tiki/tool_generic_data/generic_data_type_struct.hpp"
#include "tiki/tool_generic_data/generic_data_value.hpp"

#if TIKI_ENABLED( TIKI_GENERIC_DATA_CONVERTER )
#	include "tiki/converter_base/resource_writer.hpp"
#	include "tiki/converter_base/resource_section_writer.hpp"
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
		dispose();
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

	bool GenericDataArray::createCopyFrom( const GenericDataArray* pCopyFrom )
	{
		if( pCopyFrom == nullptr ||
			pCopyFrom->getType() == nullptr )
		{
			return false;
		}

		m_pType = pCopyFrom->getType();

		m_array.resize( pCopyFrom->getCount() );
		for( uint i = 0u; i < pCopyFrom->getCount(); ++i )
		{
			m_array[ i ] = TIKI_NEW( GenericDataValue )( m_pType->getBaseType() );
			if( !m_array[ i ]->setCopyFromValue( m_collection, pCopyFrom->getElement( i ) ) )
			{
				dispose();
				return false;
			}
		}

		return true;
	}

	void GenericDataArray::dispose()
	{
		for (uint i = 0u; i < m_array.getCount(); ++i)
		{
			TIKI_DELETE( m_array[ i ] );
		}
		m_array.dispose();
	}

	const GenericDataTypeArray* GenericDataArray::getType() const
	{
		return m_pType;
	}

	uint GenericDataArray::getCount() const
	{
		return m_array.getCount();
	}

	GenericDataValue* GenericDataArray::addElement( bool createContainer )
	{
		GenericDataValue* pValue = TIKI_NEW( GenericDataValue )( m_pType->getBaseType() );
		if( createContainer )
		{
			if( pValue->getValueType() == GenericDataValueType_Array )
			{
				GenericDataArray* pArray = TIKI_NEW( GenericDataArray )( m_collection );
				if( !pArray->create( static_cast< const GenericDataTypeArray* >( pValue->getType() ) ) )
				{
					TIKI_DELETE( pArray );
					TIKI_DELETE( pValue );
					return nullptr;
				}

				TIKI_VERIFY( pValue->setArray( pArray ) );
			}
			else if( pValue->getValueType() == GenericDataValueType_Object )
			{
				GenericDataObject* pObject = TIKI_NEW( GenericDataObject )( m_collection );
				if( !pObject->create( static_cast< const GenericDataTypeStruct* >( pValue->getType() ), nullptr ) )
				{
					TIKI_DELETE( pObject );
					TIKI_DELETE( pValue );
					return nullptr;
				}

				TIKI_VERIFY( pValue->setObject( pObject ) );
			}
		}

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

#if TIKI_ENABLED( TIKI_GENERIC_DATA_CONVERTER )
	bool GenericDataArray::writeToResource( ReferenceKey& dataKey, ResourceWriter& writer ) const
	{
		ResourceSectionWriter sectionWriter;
		writer.openDataSection( sectionWriter, SectionType_Main );

		dataKey = sectionWriter.addDataPoint();

		bool ok = true;
		for (uint i = 0u; i < m_array.getCount(); ++i)
		{
			const GenericDataValue* pValue = m_array[ i ];
			ok &= writeValueToResource( sectionWriter, m_pType->getBaseType(), *pValue );
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
		return addElement( false );
	}

	GenericDataValue* GenericDataArray::getElementValue( uint index )
	{
		if( index >= m_array.getCount() )
		{
			return nullptr;
		}

		return getElement( index );
	}

	uint GenericDataArray::getElementCount()
	{
		return m_array.getCount();
	}
}