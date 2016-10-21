#pragma once
#ifndef TIKI_GENERICDATADOCUMENT_HPP_INCLUDED__
#define TIKI_GENERICDATADOCUMENT_HPP_INCLUDED__

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/map.hpp"
#include "tiki/toolgenericdata/genericdatatype.hpp"

struct _XmlElement;

namespace tiki
{
	class GenericDataObject;
	class GenericDataTypeCollection;
	class GenericDataTypeResource;
	class GenericDataTypeStruct;
	class ResourceWriter;
	class XmlReader;
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

		bool							importFromXml( XmlReader& reader );
		// TODO: bool					exportToXml( XmlWriter& writer ) const;

		bool							writeToResource( ReferenceKey& dataKey, ResourceWriter& writer ) const;

	private:

		GenericDataTypeCollection&		m_collection;

		const GenericDataTypeResource*	m_pType;

		GenericDataObject*				m_pObject;

		const GenericDataType*			findTypeForNode( XmlReader& reader, const _XmlElement* pElement, GenericDataTypeType type ) const;
	};
}

#endif // TIKI_GENERICDATADOCUMENT_HPP_INCLUDED__
