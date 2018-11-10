#include "tiki/toolgenericdata/generic_data_object.hpp"

#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/generic_data_type_collection.hpp"
#include "tiki/toolgenericdata/generic_data_type_struct.hpp"
#include "tiki/toolgenericdata/generic_data_value.hpp"

#if TIKI_ENABLED( TIKI_GENERICDATA_CONVERTER )
#	include "tiki/converterbase/resource_writer.hpp"
#endif

namespace tiki
{
	GenericDataObject::GenericDataObject( GenericDataTypeCollection& collection )
		: GenericDataContainer( collection )
	{
		m_pType			= nullptr;
		m_pParentObject	= nullptr;
	}

	GenericDataObject::~GenericDataObject()
	{
		dispose();
	}

	bool GenericDataObject::create( const GenericDataTypeStruct* pType, const GenericDataObject* pParentObject )
	{
		if ( pType == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::create] Null type not allowed.\n" );
			return false;
		}

		if ( m_pType != nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::create] Dublicate create is not allowed.\n" );
			return false;
		}

		m_pType			= pType;
		m_pParentObject = pParentObject;
		if( m_pParentObject == nullptr &&
			this != pType->getDefaultObject() )
		{
			m_pParentObject = pType->getDefaultObject();
		}

		if( !m_fields.create( pType->getFieldCount() ) )
		{
			return false;
		}

		const List< GenericDataStructField >& structFields = pType->getFields();
		for( uint i = 0u; i < m_fields.getCount(); ++i )
		{
			ObjectField& field = m_fields[ i ];
			const GenericDataStructField& structField = structFields[ i ];

			field.name		= structField.name;
			field.pType		= structField.pType;
			field.pValue	= nullptr;
		}

		return true;
	}

	bool GenericDataObject::createCopyFrom( const GenericDataObject* pCopyFrom )
	{
		if( pCopyFrom == nullptr ||
			pCopyFrom->getType() == nullptr )
		{
			return false;
		}

		m_pType			= pCopyFrom->getType();
		m_pParentObject = pCopyFrom->getParentObject();

		if( !m_fields.create( m_pType->getFieldCount() ) )
		{
			return false;
		}

		const List< GenericDataStructField >& structFields = m_pType->getFields();
		for( uint i = 0u; i < m_fields.getCount(); ++i )
		{
			ObjectField& field = m_fields[ i ];
			const ObjectField& sourceField = pCopyFrom->m_fields[ i ];

			field.name		= sourceField.name;
			field.pType		= sourceField.pType;
			field.pValue	= nullptr;

			if( sourceField.pValue != nullptr )
			{
				field.pValue = TIKI_NEW( GenericDataValue )( sourceField.pType );
				if( !field.pValue->setCopyFromValue( m_collection, sourceField.pValue ) )
				{
					dispose();
					return false;
				}
			}
		}

		return true;
	}

	void GenericDataObject::dispose()
	{
		for (uint i = 0u; i < m_fields.getCount(); ++i)
		{
			ObjectField& field = m_fields[ i ];
			if( field.pValue != nullptr )
			{
				TIKI_DELETE( field.pValue );
			}
		}
		m_fields.dispose();
	}

	const GenericDataTypeStruct* GenericDataObject::getType() const
	{
		return m_pType;
	}

	//bool GenericDataObject::hasField( const string& name ) const
	//{
	//	return m_fields.hasKey( name );
	//}

	const string& GenericDataObject::getFieldName( uint index ) const
	{
		return m_fields[ index ].name;
	}

	const GenericDataType* GenericDataObject::getFieldType( uint index ) const
	{
		return m_fields[ index ].pType;
	}

	GenericDataValue* GenericDataObject::getFieldValue( uint index, bool createMissing )
	{
		if( m_fields[ index ].pValue != nullptr )
		{
			return m_fields[ index ].pValue;
		}
		else if( createMissing )
		{
			ObjectField& field = m_fields[ index ];
			field.pValue = TIKI_NEW( GenericDataValue )(field.pType);

			return field.pValue;
		}

		return nullptr;
	}

	GenericDataValue* GenericDataObject::getFieldValue( const string& name, bool createMissing )
	{
		const uint index = m_pType->getFieldIndexByName( name );
		if( index == (uint)-1 )
		{
			return nullptr;
		}

		return getFieldValue( index, createMissing );
	}

	bool GenericDataObject::setFieldValue( uint index, GenericDataValue* pValue )
	{
		ObjectField& field = m_fields[ index ];
		if( pValue != nullptr && field.pType != pValue->getType() )
		{
			return false;
		}

		if( field.pValue != nullptr && field.pValue != pValue )
		{
			TIKI_DELETE( field.pValue );
		}

		field.pValue = pValue;
		return true;
	}

	const GenericDataValue* GenericDataObject::getFieldOrDefaultValue( uint index ) const
	{
		if( m_fields[ index ].pValue == nullptr && m_pParentObject != nullptr )
		{
			return m_pParentObject->getFieldOrDefaultValue( index );
		}

		return m_fields[ index ].pValue;
	}

	void GenericDataObject::removeField( uint index )
	{
		ObjectField& field = m_fields[ index ];
		if( field.pValue != nullptr )
		{
			TIKI_DELETE( field.pValue );
			field.pValue = nullptr;
		}
	}

	bool GenericDataObject::removeField( const string& name )
	{
		const uint index = m_pType->getFieldIndexByName( name );
		if( index == (uint)-1 )
		{
			return false;
		}

		removeField( index );
		return true;
	}

#if TIKI_ENABLED( TIKI_GENERICDATA_CONVERTER )
	bool GenericDataObject::writeToResource( ReferenceKey* pDataKey, ResourceSectionWriter& sectionWriter ) const
	{
		ResourceSectionWriter internalSectionWriter;
		ResourceSectionWriter* pSectionWriter = &sectionWriter;
		if ( pDataKey != nullptr )
		{
			sectionWriter.getResourceWriter().openDataSection( internalSectionWriter, SectionType_Main, getType()->getAlignment() );
			pSectionWriter = &internalSectionWriter;

			*pDataKey = pSectionWriter->addDataPoint();
		}

		bool ok = true;
		const List< GenericDataStructField >& fields = m_pType->getFields();
		for (uint i = 0u; i < fields.getCount(); ++i)
		{
			const GenericDataStructField& structField = fields[ i ];

			const ObjectField& field = m_fields[ i ];
			const GenericDataValue* pValue = nullptr;
			if ( field.pValue == nullptr && m_pParentObject != nullptr )
			{
				pValue = m_pParentObject->getFieldOrDefaultValue( i );
			}
			else
			{
				TIKI_ASSERT( field.pValue != nullptr );
				pValue = field.pValue;
			}

			if ( pValue != nullptr )
			{
				TIKI_ASSERT( pValue->getType() == structField.pType );
				if ( !writeValueToResource( *pSectionWriter, *pValue ) )
				{
					TIKI_TRACE_ERROR( "[GenericDataObject::writeToResource] Unable to write value for Field '%s'\n", structField.name.cStr() );
					ok = false;
				}
			}
			else
			{
				ok = false;
			}
		}

		if ( pDataKey != nullptr )
		{
			sectionWriter.getResourceWriter().closeDataSection( internalSectionWriter );
		}

		return ok;
	}
#endif

	bool GenericDataObject::initializeXmlElementForValue( XmlElement* pElement, const GenericDataValue* pValue ) const
	{
		for( uint i = 0u; i < m_fields.getCount(); ++i )
		{
			const ObjectField& field = m_fields[ i ];
			if( field.pValue != pValue )
			{
				continue;
			}

			pElement->setAttribute( "name", field.name.cStr() );
			return true;
		}

		TIKI_TRACE_ERROR( "[GenericDataObject::initializeXmlElementForValue] Field for value '%s' not found.\n", pValue->toString().cStr() );
		return false;
	}

	const char* GenericDataObject::getNodeName() const
	{
		return "object";
	}

	const char* GenericDataObject::getElementName() const
	{
		return "field";
	}

	const GenericDataType* GenericDataObject::getParentType() const
	{
		return m_pType;
	}

	GenericDataValue* GenericDataObject::addElementValue( const XmlElement* pNode )
	{
		const XmlAttribute* pNameAtt = pNode->findAttribute( "name" );
		if ( pNameAtt == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataObject::importFromXml] 'field' node needs a 'name' attribute.\n" );
			return false;
		}

		return getFieldValue( pNameAtt->getValue(), true );
	}

	GenericDataValue* GenericDataObject::getElementValue( uint index )
	{
		if( index >= m_fields.getCount() )
		{
			return nullptr;
		}

		return getFieldValue( index, false );
	}

	uint GenericDataObject::getElementCount()
	{
		return m_fields.getCount();
	}
}