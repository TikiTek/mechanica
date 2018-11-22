#pragma once

#include "tiki/tool_generic_data/generic_data_container.hpp"

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/map.hpp"
#include "tiki/tool_generic_data/generic_data_value.hpp"

struct _XmlElement;

namespace tiki
{
	class GenericDataTypeCollection;
	class GenericDataTypeStruct;
	class ResourceWriter;
	class XmlReader;
	struct ReferenceKey;

	class GenericDataObject : public GenericDataContainer
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataObject );

	public:

										GenericDataObject( GenericDataTypeCollection& collection );
										~GenericDataObject();

		bool							create( const GenericDataTypeStruct* pType, const GenericDataObject* pParentObject );
		bool							createCopyFrom( const GenericDataObject* pCopyFrom );
		void							dispose();

		const GenericDataTypeStruct*	getType() const;
		virtual const GenericDataType*	getParentType() const TIKI_OVERRIDE_FINAL;

		const GenericDataObject*		getParentObject() const { return m_pParentObject; }

		//bool							hasField( const string& name ) const;
		uint							getFieldCount() const { return m_fields.getCount(); }

		const string&					getFieldName( uint index ) const;
		const GenericDataType*			getFieldType( uint index ) const;
		GenericDataValue*				getFieldValue( uint index, bool createMissing );
		const GenericDataValue*			getFieldValue( uint index ) const;
		bool							setFieldValue( uint index, GenericDataValue* pValue );
		GenericDataValue*				getFieldValue( const string& name, bool createMissing );
		const GenericDataValue*			getFieldValue( const string& name ) const;
		const GenericDataValue*			getFieldOrDefaultValue( uint index ) const;

		void							removeField( uint index );
		bool							removeField( const string& name );

#if TIKI_ENABLED( TIKI_GENERIC_DATA_CONVERTER )
		bool							writeToResource( ReferenceKey* pDataKey, ResourceSectionWriter& sectionWriter ) const;
#endif

		virtual bool					initializeXmlElementForValue( XmlElement* pElement, const GenericDataValue* pValue ) const TIKI_OVERRIDE_FINAL;
		virtual const char*				getElementName() const TIKI_OVERRIDE_FINAL;

	protected:

		virtual const char*				getNodeName() const TIKI_OVERRIDE_FINAL;

		virtual GenericDataValue*		addElementValue( const XmlElement* pNode ) TIKI_OVERRIDE_FINAL;
		virtual GenericDataValue*		getElementValue( uint index ) TIKI_OVERRIDE_FINAL;
		virtual uint					getElementCount() TIKI_OVERRIDE_FINAL;

	private:

		struct ObjectField
		{
			string					name;
			const GenericDataType*	pType;
			GenericDataValue*		pValue;
		};

		typedef Array< ObjectField > FieldArray;

		const GenericDataTypeStruct*	m_pType;
		const GenericDataObject*		m_pParentObject;

		FieldArray						m_fields;
	};
}
