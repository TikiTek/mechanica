#pragma once
#ifndef TIKI_GENERICDATAARRAY_HPP_INCLUDED__
#define TIKI_GENERICDATAARRAY_HPP_INCLUDED__

#include "tiki/toolgenericdata/genericdatacontainer.hpp"

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"

struct _XmlElement;

namespace tiki
{
	class GenericDataTypeArray;
	class GenericDataTypeCollection;
	class GenericDataValue;
	class ResourceWriter;
	class XmlReader;
	struct ReferenceKey;

	class GenericDataArray : public GenericDataContainer
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataArray );

	public:

										GenericDataArray( GenericDataTypeCollection& collection );
										~GenericDataArray();

		bool							create( const GenericDataTypeArray* pType );
		void							dispose();

		const GenericDataTypeArray*		getType() const;

		uint							getCount() const;

		GenericDataValue*				addElement();
		GenericDataValue*				getElement( uint index );
		const GenericDataValue*			getElement( uint index ) const;
		bool							removeElement( uint index );

		// TODO: bool					exportToXml( XmlWriter& writer ) const;
		bool							writeToResource( ReferenceKey& dataKey, ResourceWriter& writer ) const;

	protected:

		virtual const char*				getElementName() const TIKI_OVERRIDE_FINAL;
		virtual const GenericDataType*	getParentType() const TIKI_OVERRIDE_FINAL;

		virtual GenericDataValue*		addElementValue( const XmlReader& reader, const _XmlElement* pElement ) TIKI_OVERRIDE_FINAL;

	private:

		const GenericDataTypeArray*		m_pType;

		List< GenericDataValue* >		m_array;

	};
}

#endif // TIKI_GENERICDATAARRAY_HPP_INCLUDED__
