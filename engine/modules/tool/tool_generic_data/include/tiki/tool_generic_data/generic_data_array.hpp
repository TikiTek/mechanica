#pragma once

#include "tiki/tool_generic_data/generic_data_container.hpp"

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

		explicit						GenericDataArray( GenericDataTypeCollection& collection );
										~GenericDataArray();

		bool							create( const GenericDataTypeArray* pType );
		bool							createCopyFrom( const GenericDataArray* pCopyFrom );
		void							dispose();

		const GenericDataTypeArray*		getType() const;
		virtual const GenericDataType*	getParentType() const TIKI_OVERRIDE_FINAL;

		uint							getCount() const;

		GenericDataValue*				addElement();
		GenericDataValue*				getElement( uint index );
		const GenericDataValue*			getElement( uint index ) const;
		bool							removeElement( uint index );

#if TIKI_ENABLED( TIKI_GENERIC_DATA_CONVERTER )
		bool							writeToResource( ReferenceKey& dataKey, ResourceWriter& writer ) const;
#endif

		virtual bool					initializeXmlElementForValue( XmlElement* pElement, const GenericDataValue* pValue ) const TIKI_OVERRIDE_FINAL;
		virtual const char*				getElementName() const TIKI_OVERRIDE_FINAL;

	protected:

		virtual const char*				getNodeName() const TIKI_OVERRIDE_FINAL;

		virtual GenericDataValue*		addElementValue( const XmlElement* pNode ) TIKI_OVERRIDE_FINAL;
		virtual GenericDataValue*		getElementValue( uint index ) TIKI_OVERRIDE_FINAL;
		virtual uint					getElementCount() TIKI_OVERRIDE_FINAL;

	private:

		const GenericDataTypeArray*		m_pType;

		List< GenericDataValue* >		m_array;
	};
}
