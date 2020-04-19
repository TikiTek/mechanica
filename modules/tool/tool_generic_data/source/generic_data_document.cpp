#include "tiki/tool_generic_data/generic_data_document.hpp"

#include "tiki/tool_generic_data/generic_data_document_collection.hpp"
#include "tiki/tool_generic_data/generic_data_object.hpp"
#include "tiki/tool_generic_data/generic_data_type_collection.hpp"
#include "tiki/tool_generic_data/generic_data_types.hpp"
#include "tiki/tool_xml/xml_attribute.hpp"
#include "tiki/tool_xml/xml_node.hpp"

namespace tiki
{
	GenericDataDocument::GenericDataDocument( GenericDataDocumentCollection& collection )
		: m_collection( collection )
	{
		m_pResourceType	= nullptr;
		m_pObject		= nullptr;
	}

	GenericDataDocument::~GenericDataDocument()
	{
		dispose();
	}

	bool GenericDataDocument::create( const GenericDataTypeResource* pResourceType, const GenericDataTypeStruct* pObjectType, const GenericDataObject* pParentObject )
	{
		if( pObjectType == nullptr )
		{
			return false;
		}

		m_pResourceType = pResourceType;
		m_pObject		= TIKI_NEW( GenericDataObject )( m_collection.getTypeCollection() );

		return m_pObject->create( pObjectType, pParentObject );
	}

	void GenericDataDocument::dispose()
	{
		m_pResourceType = nullptr;

		if ( m_pObject != nullptr )
		{
			m_pObject->dispose();
			TIKI_DELETE( m_pObject );
			m_pObject = nullptr;
		}
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

#if TIKI_ENABLED( TIKI_GENERIC_DATA_CONVERTER )
	bool GenericDataDocument::writeToResource( ReferenceKey& dataKey, ResourceSectionWriter& sectionWriter ) const
	{
		return m_pObject->writeToResource( &dataKey, sectionWriter );
	}
#endif

	bool GenericDataDocument::importFromXml()
	{
		XmlElement* pRootNode = m_document.getRoot();
		if( pRootNode == nullptr || !pRootNode->isName( "generic_data" ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::importFromXml] Unable to find root node.\n" );
			return false;
		}

		XmlElement* pObjectParentNode = nullptr;;
		const GenericDataType* pResourceType = nullptr;
		{
			XmlElement* pResourceNode = pRootNode->findFirstChild( "resource" );
			if( pResourceNode != nullptr )
			{
				pObjectParentNode = pResourceNode;
				pResourceType = findTypeForNode( pResourceNode, GenericDataTypeType_Resource );
				if( pResourceType == nullptr )
				{
					return false;
				}
			}
			else
			{
				pObjectParentNode = pRootNode;
			}
		}

		XmlElement* pObjectNode = pObjectParentNode->findFirstChild( "object" );
		const GenericDataType* pObjectType = findTypeForNode( pObjectNode, GenericDataTypeType_Struct );
		if( pObjectType == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::importFromXml] Unable to find 'object' node.\n" );
			return false;
		}

		GenericDataObject* pParentObject = nullptr;
		const XmlAttribute* pParentAttribute = pObjectNode->findAttribute( "parent" );
		if( pParentAttribute != nullptr )
		{
			GenericDataDocument* pParentDocument = m_collection.loadDocument( pParentAttribute->getValue() );
			if( pParentDocument == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataDocument::importFromXml] Failed to load parent document '%s'.\n", pParentAttribute->getValue() );
				return false;
			}

			pParentObject = pParentDocument->getObject();
		}

		const GenericDataTypeResource* pTypedResourceType	= (const GenericDataTypeResource*)pResourceType;
		const GenericDataTypeStruct* pTypedObjectType		= (const GenericDataTypeStruct*)pObjectType;

		if( !create( pTypedResourceType, pTypedObjectType, pParentObject ) )
		{
			return false;
		}

		GenericDataLoadContext context;
		context.pTypeCollection		= &m_collection.getTypeCollection();
		context.pDocumentCollection	= &m_collection;
		context.isType				= false;

		return m_pObject->importFromXml( pObjectNode, context );
	}

	bool GenericDataDocument::exportToXml()
	{
		return m_pObject->exportToXml( m_document.getRoot() );
	}

	const GenericDataType* GenericDataDocument::findTypeForNode( const XmlElement* pNode, GenericDataTypeType type ) const
	{
		if ( pNode == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::findTypeForNode] Unable to find type node.\n" );
			return nullptr;
		}

		const XmlAttribute* pTypeAtt = pNode->findAttribute( "type" );
		if ( pTypeAtt == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::findTypeForNode] Node needs an 'type' attribute.\n" );
			return nullptr;
		}

		const GenericDataType* pType = m_collection.getTypeCollection().findTypeByName( pTypeAtt->getValue() );
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
