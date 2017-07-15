#pragma once
#ifndef TIKI_GENERIC_DATA_CONTAINER_HPP_INCLUDED
#define TIKI_GENERIC_DATA_CONTAINER_HPP_INCLUDED

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"

struct _XmlElement;

namespace tiki
{
	class GenericDataType;
	class GenericDataTypeCollection;
	class GenericDataValue;
	class ResourceWriter;
	class XmlNode;

	class GenericDataContainer
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataContainer );

	public:

		bool							importFromXml( XmlNode* pObjectNode );
		bool							exportToXml();

	protected:

		GenericDataTypeCollection&		m_collection;

		XmlNode*						m_pObjectNode;

										GenericDataContainer( GenericDataTypeCollection& collection );
										~GenericDataContainer();

		virtual const char*				getElementName() const TIKI_PURE;
		virtual const GenericDataType*	getParentType() const TIKI_PURE;

		virtual GenericDataValue*		addElementValue( XmlNode* pNode ) TIKI_PURE;

		bool							writeValueToResource( ResourceWriter& writer, const GenericDataValue& value ) const;
	};
}

#endif // TIKI_GENERICDATACONTAINER_HPP_INCLUDED
