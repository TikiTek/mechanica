#pragma once
#ifndef TIKI_GENERICDATAOBJECT_HPP_INCLUDED__
#define TIKI_GENERICDATAOBJECT_HPP_INCLUDED__

#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolbase/map.hpp"

namespace tiki
{
	class GenericDataType;
	class GenericDataValue;
	class ResourceWriter;

	class GenericDataObject
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataObject );

	public:

		GenericDataObject();
		~GenericDataObject();

		const GenericDataType*	getType() const;
		bool					setType( const GenericDataType* pType );

		bool					hasField( const string& name ) const;

		GenericDataValue&		getFieldValue( const string& name );
		const GenericDataValue&	getFieldValue( const string& name ) const;

		bool					setFieldValue( const string& name, const GenericDataValue& value );

		void					writeToResource( ResourceWriter& writer ) const;

	private:

		const GenericDataType*			m_pType;

		Map< string, GenericDataValue >	m_fields;

	};
}

#endif // TIKI_GENERICDATAOBJECT_HPP_INCLUDED__
