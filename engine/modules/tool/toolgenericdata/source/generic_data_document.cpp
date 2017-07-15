#include "tiki/toolgenericdata/generic_data_document.hpp"

#include "tiki/toolgenericdata/generic_data_object.hpp"
#include "tiki/toolgenericdata/genericdatatypecollection.hpp"
#include "tiki/toolxml/xml_attribute.hpp"
#include "tiki/toolxml/xml_node.hpp"

namespace tiki
{
	GenericDataDocument::GenericDataDocument( GenericDataTypeCollection& collection )
		: m_collection( collection )
	{
		m_pType		= nullptr;
		m_pObject	= nullptr;
	}

	GenericDataDocument::~GenericDataDocument()
	{
		dispose();
	}

	bool GenericDataDocument::create( const GenericDataTypeResource* pResourceType, const GenericDataTypeStruct* pObjectType )
	{
		if ( pResourceType == nullptr || pObjectType == nullptr )
		{
			return false;
		}

		m_pType		= pResourceType;
		m_pObject	= TIKI_NEW( GenericDataObject )( m_collection );

		return m_pObject->create( pObjectType, nullptr );
	}

	void GenericDataDocument::dispose()
	{
		m_pType = nullptr;

		if ( m_pObject != nullptr )
		{
			m_pObject->dispose();
			TIKI_DELETE( m_pObject );
			m_pObject = nullptr;
		}
	}

	const GenericDataTypeResource* GenericDataDocument::getType() const
	{
		return m_pType;
	}

	GenericDataObject* GenericDataDocument::getObject()
	{
		return m_pObject;
	}

	const GenericDataObject* GenericDataDocument::getObject() const
	{
		return m_pObject;
	}

	bool GenericDataDocument::importFromFile( const char* pFilename )
	{
		if( !m_document.loadFromFile( pFilename ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::importFromFile] Unable to parse '%s'.\n", pFilename );
		}

		if( !importFromXml() )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::importFromFile] Unable to import '%s'.\n", pFilename );
			return false;
		}

		return true;
	}

	bool GenericDataDocument::exportToFile( const char* pFilename )
	{
		if( !exportToXml() )
		{
			return false;
		}

		return m_document.saveToFile( pFilename );
	}

	bool GenericDataDocument::writeToResource( ReferenceKey& dataKey, ResourceWriter& writer ) const
	{
		return m_pObject->writeToResource( &dataKey, writer );
	}

	bool GenericDataDocument::importFromXml()
	{
		const XmlNode* pRootNode = m_document.getRoot();
		if( !pRootNode || !isStringEquals( pRootNode->getName(), "tikigenericobjects" ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::importFromXml] Unable to find root node.\n" );
			return false;
		}

		const XmlNode* pResourceNode = pRootNode->findFirstChild( "resource" );
		const GenericDataType* pResourceType = findTypeForNode( pResourceNode, GenericDataTypeType_Resource );
		if( pResourceType == nullptr )
		{
			return false;
		}

		const XmlNode* pObjectNode = pResourceNode->findFirstChild( "object" );
		const GenericDataType* pObjectType = findTypeForNode( pObjectNode, GenericDataTypeType_Struct );
		if( pObjectType == nullptr )
		{
			return false;
		}

		const GenericDataTypeResource* pTypedResourceType	= (const GenericDataTypeResource*)pResourceType;
		const GenericDataTypeStruct* pTypedObjectType		= (const GenericDataTypeStruct*)pObjectType;

		if( !create( pTypedResourceType, pTypedObjectType ) )
		{
			return false;
		}

		return m_pObject->importFromXml( pObjectNode );
	}

	bool GenericDataDocument::exportToXml()
	{
		return true;
	}

	const GenericDataType* GenericDataDocument::findTypeForNode( const XmlNode* pNode, GenericDataTypeType type ) const
	{
		if ( pNode == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::findTypeForNode] Unable to find resource node.\n" );
			return nullptr;
		}

		const XmlAttribute* pTypeAtt = pNode->findAttribute( "type" );
		if ( pTypeAtt == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::findTypeForNode] Node needs an 'type' attribute.\n" );
			return nullptr;
		}

		const GenericDataType* pType = m_collection.findTypeByName( pTypeAtt->getValue() );
		if ( pType == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::findTypeForNode] Type with name '%s' can't found.\n", pTypeAtt->getValue() );
			return nullptr;
		}

		if ( pType->getType() != type )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::findTypeForNode] Type has the wrong type.\n" );
			return nullptr;
		}

		return pType;
	}

}
