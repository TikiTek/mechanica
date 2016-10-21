#pragma once
#ifndef TIKI_GENERICDATAOBJECT_HPP_INCLUDED__
#define TIKI_GENERICDATAOBJECT_HPP_INCLUDED__

#include "tiki/toolgenericdata/genericdatacontainer.hpp"

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/map.hpp"

struct _XmlElement;

namespace tiki
{
	class GenericDataTypeCollection;
	class GenericDataTypeStruct;
	class GenericDataValue;
	class ResourceWriter;
	class XmlReader;
	struct ReferenceKey;

	class GenericDataObject : public GenericDataContainer
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataObject );

	public:

										GenericDataObject( GenericDataTypeCollection& collection );
										~GenericDataObject();

		bool							create( const GenericDataTypeStruct* pType );
		void							dispose();

		const GenericDataTypeStruct*	getType() const;

		bool							hasField( const string& name ) const;

		GenericDataValue&				getFieldValue( const string& name );
		const GenericDataValue&			getFieldValue( const string& name ) const;
		bool							setFieldValue( const string& name, const GenericDataValue& value );

		bool							writeToResource( ReferenceKey* pDataKey, ResourceWriter& writer ) const;

	protected:

		virtual const char*				getElementName() const TIKI_OVERRIDE;
		virtual const GenericDataType*	getParentType() const TIKI_OVERRIDE;

		virtual bool					applyElementValue( const XmlReader& reader, const _XmlElement* pElement, const GenericDataValue& value ) TIKI_OVERRIDE;

	private:

		const GenericDataTypeStruct*	m_pType;

		Map< string, GenericDataValue >	m_fields;

	};
}

#endif // TIKI_GENERICDATAOBJECT_HPP_INCLUDED__
