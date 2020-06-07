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

		virtual const GenericDataType*	getParentType() const = 0;

		bool							importFromXml( XmlElement* pObjectNode, const GenericDataLoadContext& context );
		bool							exportToXml( XmlElement* pParentNode );

		virtual bool					initializeXmlElementForValue( XmlElement* pElement, const GenericDataValue* pValue ) const = 0;
		virtual const char*				getElementName() const = 0;

	protected:

		GenericDataTypeCollection&		m_collection;

		XmlElement*						m_pObjectNode;

										GenericDataContainer( GenericDataTypeCollection& collection );
										~GenericDataContainer();

		virtual const char*				getNodeName() const = 0;

		virtual GenericDataValue*		addElementValue( const XmlElement* pNode ) = 0;
		virtual GenericDataValue*		getElementValue( uint index ) = 0;
		virtual uint					getElementCount() = 0;

#if TIKI_ENABLED( TIKI_GENERIC_DATA_CONVERTER )
		bool							writeValueToResource( ResourceSectionWriter& sectionWriter, const GenericDataType* pTargetType, const GenericDataValue& value ) const;
#endif
	};
}
