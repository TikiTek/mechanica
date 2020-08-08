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

		m_elements.resize( pCopyFrom->getCount() );
		for( uintreg i = 0u; i < pCopyFrom->getCount(); ++i )
		{
			m_elements[ i ] = TIKI_NEW( GenericDataValue )( m_pType->getBaseType() );
			if( !m_elements[ i ]->setCopyFromValue( m_collection, pCopyFrom->getElement( i ) ) )
			{
				dispose();
				return false;
			}
		}

		return true;
	}

	void GenericDataArray::dispose()
	{
		for (uintreg i = 0u; i < m_elements.getCount(); ++i)
		{
			TIKI_DELETE( m_elements[ i ] );
		}
		m_elements.dispose();
	}

	const GenericDataTypeArray* GenericDataArray::getType() const
	{
		return m_pType;
	}

	GenericDataValue* GenericDataArray::addElement( bool createContainer )
	{
		GenericDataValue* pValue = TIKI_NEW( GenericDataValue )( m_pType->getBaseType() );
		if( pValue == nullptr )
		{
			return nullptr;
		}

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

		m_elements.add( pValue );
		return pValue;
	}

	GenericDataValue* GenericDataArray::getElement( uintreg index )
	{
		return m_elements[ index ];
	}

	const GenericDataValue* GenericDataArray::getElement( uintreg index ) const
	{
		return m_elements[ index ];
	}

	bool GenericDataArray::removeElement( uintreg index )
	{
		if ( index >= m_elements.getCount() )
		{
			TIKI_TRACE_ERROR( "[GenericDataArray::removeElement] Index is greater as the size(%i > %i).\n", index, m_elements.getCount() );
			return false;
		}

		GenericDataValue* pValue = m_elements[ index ];
		TIKI_DELETE( pValue );

		m_elements.removeSortedAtIndex( index );
		return true;
	}

#if TIKI_ENABLED( TIKI_GENERIC_DATA_CONVERTER )
	bool GenericDataArray::writeToResource( ReferenceKey& dataKey, ResourceWriter& writer ) const
	{
		ResourceSectionWriter sectionWriter;
		writer.openDataSection( sectionWriter, SectionType_Main );

		dataKey = sectionWriter.addDataPoint();

		bool ok = true;
		for (uintreg i = 0u; i < m_elements.getCount(); ++i)
		{
			const GenericDataValue* pValue = m_elements[ i ];
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

	const GenericDataType* GenericDataArray::getElementType() const
	{
		return m_pType->getBaseType();
	}

	GenericDataValue* GenericDataArray::addElementValue( const XmlElement* pNode )
	{
		return addElement( false );
	}

	GenericDataValue* GenericDataArray::getElementValue( uintreg index )
	{
		if( index >= m_elements.getCount() )
		{
			return nullptr;
		}

		return getElement( index );
	}

	uintreg GenericDataArray::getElementCount()
	{
		return m_elements.getCount();
	}
}