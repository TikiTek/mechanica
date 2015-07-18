#pragma once
#ifndef TIKI_GENERICDATAOBJECT_HPP_INCLUDED__
#define TIKI_GENERICDATAOBJECT_HPP_INCLUDED__

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/map.hpp"

namespace tiki
{
	class GenericDataTypeStruct;
	class GenericDataValue;
	class ResourceWriter;
	class XmlReader;

	class GenericDataObject
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataObject );

	public:

									GenericDataObject();
									~GenericDataObject();

		bool						createFromType( const GenericDataTypeStruct* pType );

		const GenericDataTypeStruct*	getType() const;

		bool						hasField( const string& name ) const;

		GenericDataValue&			getFieldValue( const string& name );
		const GenericDataValue&		getFieldValue( const string& name ) const;
		bool						setFieldValue( const string& name, const GenericDataValue& value );

		bool						importFromXml( XmlReader& reader );
		bool						exportToResource( ResourceWriter& writer ) const;

	private:

		const GenericDataTypeStruct*		m_pType;

		Map< string, GenericDataValue >	m_fields;

	};
}

#endif // TIKI_GENERICDATAOBJECT_HPP_INCLUDED__
