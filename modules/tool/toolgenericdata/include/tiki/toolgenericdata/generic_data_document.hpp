#pragma once
#ifndef TIKI_GENERIC_DATA_DOCUMENT_HPP_INCLUDED
#define TIKI_GENERIC_DATA_DOCUMENT_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/map.hpp"
#include "tiki/toolgenericdata/generic_data_type.hpp"
#include "tiki/toolxml/xml_document.hpp"

namespace tiki
{
	class GenericDataObject;
	class GenericDataTypeCollection;
	class GenericDataTypeResource;
	class GenericDataTypeStruct;
	class ResourceWriter;
	struct ReferenceKey;

	class GenericDataDocument
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataDocument );

	public:

		GenericDataDocument( GenericDataTypeCollection& collection );
		~GenericDataDocument();

		bool							create( const GenericDataTypeResource* pResourceType, const GenericDataTypeStruct* pObjectType );
		void							dispose();

		const GenericDataTypeResource*	getType() const;

		GenericDataObject*				getObject();
		const GenericDataObject*		getObject() const;

		bool							importFromFile( const char* pFilename );
		bool							exportToFile( const char* pFilename );

#if TIKI_ENABLED( TIKI_GENERICDATA_CONVERTER )
		bool							writeToResource( ReferenceKey& dataKey, ResourceSectionWriter& sectionWriter ) const;
#endif

	private:

		GenericDataTypeCollection&		m_collection;

		XmlDocument						m_document;

		const GenericDataTypeResource*	m_pType;
		GenericDataObject*				m_pObject;

		bool							importFromXml();
		bool							exportToXml();

		const GenericDataType*			findTypeForNode( const XmlElement* pNode, GenericDataTypeType type ) const;
	};
}

#endif // TIKI_GENERIC_DATA_DOCUMENT_HPP_INCLUDED
