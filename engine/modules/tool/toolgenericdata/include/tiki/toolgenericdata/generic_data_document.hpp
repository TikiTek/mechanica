#pragma once
#ifndef TIKI_GENERIC_DATA_DOCUMENT_HPP_INCLUDED
#define TIKI_GENERIC_DATA_DOCUMENT_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/map.hpp"
#include "tiki/toolgenericdata/genericdatatype.hpp"

#include <tinyxml2.h>

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

		bool							writeToResource( ReferenceKey& dataKey, ResourceWriter& writer ) const;

	private:

		GenericDataTypeCollection&		m_collection;

		tinyxml2::XMLDocument			m_document;

		const GenericDataTypeResource*	m_pType;
		GenericDataObject*				m_pObject;

		bool							importFromXml( XmlReader& reader );
		bool							exportToXml();

		const GenericDataType*			findTypeForNode( XmlReader& reader, const _XmlElement* pElement, GenericDataTypeType type ) const;
	};
}

#endif // TIKI_GENERIC_DATA_DOCUMENT_HPP_INCLUDED
