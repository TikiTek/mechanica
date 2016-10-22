
#include "tiki/toolgenericdata/genericdatadocument.hpp"

#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/genericdataobject.hpp"
#include "tiki/toolgenericdata/genericdatatypecollection.hpp"

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
		
		return m_pObject->create( pObjectType );
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

	bool GenericDataDocument::importFromXml( XmlReader& reader )
	{
		const XmlElement* pRootNode = reader.findNodeByName( "tikigenericobjects" );
		if ( !pRootNode )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::importFromXml] Unable to find root node.\n" );
			return false;
		}

		const XmlElement* pResourceNode = reader.findFirstChild( "resource", pRootNode );
		const GenericDataType* pResourceType = findTypeForNode( reader, pResourceNode, GenericDataTypeType_Resource );
		if ( pResourceType == nullptr )
		{
			return false;
		}

		const XmlElement* pObjectNode = reader.findFirstChild( "object", pResourceNode );
		const GenericDataType* pObjectType = findTypeForNode( reader, pObjectNode, GenericDataTypeType_Struct );
		if ( pObjectType == nullptr )
		{
			return false;
		}

		const GenericDataTypeResource* pTypedResourceType	= (const GenericDataTypeResource*)pResourceType;
		const GenericDataTypeStruct* pTypedObjectType		= (const GenericDataTypeStruct*)pObjectType;
		
		if ( !create( pTypedResourceType, pTypedObjectType ) )
		{
			return false;
		}

		return m_pObject->importFromXml( reader, pObjectNode );
	}

	bool GenericDataDocument::writeToResource( ReferenceKey& dataKey, ResourceWriter& writer ) const
	{
		return m_pObject->writeToResource( &dataKey, writer );
	}

	const GenericDataType* GenericDataDocument::findTypeForNode( XmlReader& reader, const _XmlElement* pElement, GenericDataTypeType type ) const
	{
		if ( pElement == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::findTypeForNode] Unable to find resource node.\n" );
			return nullptr;
		}

		const XmlAttribute* pTypeAtt = reader.findAttributeByName( "type", pElement );
		if ( pTypeAtt == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::findTypeForNode] Node needs an 'type' attribute.\n" );
			return nullptr;
		}

		const GenericDataType* pType = m_collection.findTypeByName( pTypeAtt->content );
		if ( pType == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataDocument::findTypeForNode] Type with name '%s' can't found.\n", pTypeAtt->content );
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
