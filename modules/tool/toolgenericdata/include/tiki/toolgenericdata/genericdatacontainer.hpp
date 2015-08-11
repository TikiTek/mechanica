#pragma once
#ifndef TIKI_GENERICDATACONTAINER_HPP_INCLUDED__
#define TIKI_GENERICDATACONTAINER_HPP_INCLUDED__

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/list.hpp"

struct _XmlElement;

namespace tiki
{
	class GenericDataType;
	class GenericDataTypeCollection;
	class GenericDataValue;
	class ResourceWriter;
	class XmlReader;

	class GenericDataContainer
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataContainer );

	public:

		bool							importFromXml( XmlReader& reader, const _XmlElement* pObjectRootNode );

	protected:

		GenericDataTypeCollection&		m_collection;

										GenericDataContainer( GenericDataTypeCollection& collection );
										~GenericDataContainer();

		virtual const char*				getElementName() const TIKI_PURE;
		virtual const GenericDataType*	getParentType() const TIKI_PURE;

		virtual bool					applyElementValue( XmlReader& reader, const _XmlElement* pElement, const GenericDataValue& value ) TIKI_PURE;

		bool							writeValueToResource( ResourceWriter& writer, const GenericDataValue& value ) const;

	private:

		bool							loadValueFromXml( GenericDataValue& outValue, XmlReader& reader, const _XmlElement* pElement, const GenericDataType* pElementType, const GenericDataType* pParentType );

	};
}

#endif // TIKI_GENERICDATACONTAINER_HPP_INCLUDED__
