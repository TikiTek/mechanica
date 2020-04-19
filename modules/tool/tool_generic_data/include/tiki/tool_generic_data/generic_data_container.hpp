#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"

namespace tiki
{
	class GenericDataType;
	class GenericDataTypeCollection;
	class GenericDataValue;
	class ResourceSectionWriter;
	class XmlElement;
	struct GenericDataLoadContext;

	class GenericDataContainer
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataContainer );

	public:

		virtual const GenericDataType*	getParentType() const TIKI_PURE;

		bool							importFromXml( XmlElement* pObjectNode, const GenericDataLoadContext& context );
		bool							exportToXml( XmlElement* pParentNode );

		virtual bool					initializeXmlElementForValue( XmlElement* pElement, const GenericDataValue* pValue ) const TIKI_PURE;
		virtual const char*				getElementName() const TIKI_PURE;

	protected:

		GenericDataTypeCollection&		m_collection;

		XmlElement*						m_pObjectNode;

										GenericDataContainer( GenericDataTypeCollection& collection );
										~GenericDataContainer();

		virtual const char*				getNodeName() const TIKI_PURE;

		virtual GenericDataValue*		addElementValue( const XmlElement* pNode ) TIKI_PURE;
		virtual GenericDataValue*		getElementValue( uint index ) TIKI_PURE;
		virtual uint					getElementCount() TIKI_PURE;

#if TIKI_ENABLED( TIKI_GENERIC_DATA_CONVERTER )
		bool							writeValueToResource( ResourceSectionWriter& sectionWriter, const GenericDataType* pTargetType, const GenericDataValue& value ) const;
#endif
	};
}
