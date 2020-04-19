#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/map.hpp"
#include "tiki/tool_generic_data/generic_data_type.hpp"
#include "tiki/tool_xml/xml_document.hpp"

namespace tiki
{
	class GenericDataObject;
	class GenericDataDocumentCollection;
	class GenericDataTypeResource;
	class GenericDataTypeStruct;
	class ResourceWriter;
	struct ReferenceKey;

	class GenericDataDocument
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataDocument );

	public:

		GenericDataDocument( GenericDataDocumentCollection& collection );
		~GenericDataDocument();

		bool							create( const GenericDataTypeResource* pResourceType, const GenericDataTypeStruct* pObjectType, const GenericDataObject* pParentObject );
		void							dispose();

		const GenericDataTypeResource*	getResourceType() const { return m_pResourceType; }

		GenericDataObject*				getObject() { return m_pObject; }
		const GenericDataObject*		getObject() const { return m_pObject; }

		bool							importFromFile( const char* pFilename );
		bool							exportToFile( const char* pFilename );

#if TIKI_ENABLED( TIKI_GENERIC_DATA_CONVERTER )
		bool							writeToResource( ReferenceKey& dataKey, ResourceSectionWriter& sectionWriter ) const;
#endif

	private:

		GenericDataDocumentCollection&	m_collection;

		XmlDocument						m_document;

		const GenericDataTypeResource*	m_pResourceType;
		GenericDataObject*				m_pObject;

		bool							importFromXml();
		bool							exportToXml();

		const GenericDataType*			findTypeForNode( const XmlElement* pNode, GenericDataTypeType type ) const;
	};
}
