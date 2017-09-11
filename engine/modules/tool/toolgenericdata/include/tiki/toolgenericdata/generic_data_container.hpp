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

		const GenericDataTypeArray*		getType() const;
		virtual const GenericDataType*	getParentType() const TIKI_PURE;

		bool							importFromXml( XmlNode* pObjectNode );
		bool							exportToXml( XmlNode* pParentNode );

	protected:

		GenericDataTypeCollection&		m_collection;

		XmlNode*						m_pObjectNode;

		const GenericDataTypeArray*		m_pType;

										GenericDataContainer( GenericDataTypeCollection& collection );
										~GenericDataContainer();

		virtual const char*				getNodeName() const TIKI_PURE;
		virtual const char*				getElementName() const TIKI_PURE;

		virtual GenericDataValue*		addElementValue( const XmlNode* pNode ) TIKI_PURE;
		virtual GenericDataValue*		getElementValue( uint index ) TIKI_PURE;

		bool							writeValueToResource( ResourceWriter& writer, const GenericDataValue& value ) const;
	};
}

#endif // TIKI_GENERICDATACONTAINER_HPP_INCLUDED
