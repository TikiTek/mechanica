#pragma once
#ifndef TIKI_GENERICDATAOBJECT_HPP_INCLUDED__
#define TIKI_GENERICDATAOBJECT_HPP_INCLUDED__

#include "tiki/toolgenericdata/genericdatacontainer.hpp"

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/map.hpp"
#include "tiki/toolgenericdata/genericdatavalue.hpp"

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
		void							dispose();

		const GenericDataTypeStruct*	getType() const;

		bool							hasField( const string& name ) const;
		uint							getFieldCount() const { return m_fields.getCount(); }

		const string&					getFieldName( uint index ) const;
		const GenericDataType*			getFieldType( uint index ) const;
		GenericDataValue*				getFieldValue( const string& name, bool createMissing );
		const GenericDataValue*			getFieldValue( const string& name ) const;
		GenericDataValue*				getFieldValue( uint index ) const;

		bool							removeField( const string& name );

		bool							writeToResource( ReferenceKey* pDataKey, ResourceWriter& writer ) const;

	protected:

		virtual const char*				getElementName() const TIKI_OVERRIDE_FINAL;
		virtual const GenericDataType*	getParentType() const TIKI_OVERRIDE_FINAL;

		virtual GenericDataValue*		addElementValue( const XmlReader& reader, const _XmlElement* pElement ) TIKI_OVERRIDE_FINAL;

	private:

		struct ObjectField
		{
			const GenericDataType*	pType;
			GenericDataValue*		pValue;
		};

		const GenericDataTypeStruct*	m_pType;
		const GenericDataObject*		m_pParentObject;

		Map< string, ObjectField >		m_fields;

	};
}

#endif // TIKI_GENERICDATAOBJECT_HPP_INCLUDED__
