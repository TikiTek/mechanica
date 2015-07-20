#pragma once
#ifndef TIKI_GENERICDATAOBJECT_HPP_INCLUDED__
#define TIKI_GENERICDATAOBJECT_HPP_INCLUDED__

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/map.hpp"

struct _XmlElement;

namespace tiki
{
	class GenericDataTypeCollection;
	class GenericDataTypeStruct;
	class GenericDataValue;
	class ResourceWriter;
	class XmlReader;

	class GenericDataObject
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

		bool							importFromXml( XmlReader& reader, const _XmlElement* pObjectRootNode );

		// TODO: bool					exportToXml( XmlWriter& writer ) const;
		bool							exportToResource( ResourceWriter& writer ) const;

	private:

		GenericDataTypeCollection&		m_collection;

		const GenericDataTypeStruct*	m_pType;

		Map< string, GenericDataValue >	m_fields;

	};
}

#endif // TIKI_GENERICDATAOBJECT_HPP_INCLUDED__
