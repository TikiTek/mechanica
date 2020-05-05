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

		//bool							hasField( const DynamicString& name ) const;
		uintreg							getFieldCount() const { return m_fields.getCount(); }

		const DynamicString&			getFieldName( uintreg index ) const;
		const GenericDataType*			getFieldType( uintreg index ) const;
		GenericDataValue*				getFieldValue( uintreg index, bool createMissing );
		const GenericDataValue*			getFieldValue( uintreg index ) const;
		bool							setFieldValue( uintreg index, GenericDataValue* pValue );
		GenericDataValue*				getFieldValue( const DynamicString& name, bool createMissing );
		const GenericDataValue*			getFieldValue( const DynamicString& name ) const;
		GenericDataLevelValue			getFieldLevelValue( uintreg index );
		GenericDataLevelValue			getFieldLevelValue( const DynamicString& name );

		void							removeField( uintreg index );
		bool							removeField( const DynamicString& name );

#if TIKI_ENABLED( TIKI_GENERIC_DATA_CONVERTER )
		bool							writeToResource( ReferenceKey* pDataKey, ResourceSectionWriter& sectionWriter ) const;
#endif

		virtual bool					initializeXmlElementForValue( XmlElement* pElement, const GenericDataValue* pValue ) const TIKI_OVERRIDE_FINAL;
		virtual const char*				getElementName() const TIKI_OVERRIDE_FINAL;

	protected:

		virtual const char*				getNodeName() const TIKI_OVERRIDE_FINAL;

		virtual GenericDataValue*		addElementValue( const XmlElement* pNode ) TIKI_OVERRIDE_FINAL;
		virtual GenericDataValue*		getElementValue( uintreg index ) TIKI_OVERRIDE_FINAL;
		virtual uintreg					getElementCount() TIKI_OVERRIDE_FINAL;

	private:

		struct ObjectField
		{
			DynamicString				name;
			const GenericDataType*		pType;
			GenericDataValue*			pValue;
		};

		typedef Array< ObjectField > FieldArray;

		const GenericDataTypeStruct*	m_pType;
		const GenericDataObject*		m_pParentObject;

		FieldArray						m_fields;

		GenericDataLevelValue			getFieldLevelValueInternal( uintreg index, bool firstLevel );
		GenericDataLevelValue			getFieldLevelValueInternal( uintreg index, bool firstLevel ) const;
	};
}
