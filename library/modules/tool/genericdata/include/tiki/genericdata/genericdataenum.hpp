#pragma once
#ifndef TIKI_GENERICDATAENUM_HPP_INCLUDED__
#define TIKI_GENERICDATAENUM_HPP_INCLUDED__

#include "tiki/genericdata/genericdatatype.hpp"

#include "tiki/toolbase/list.hpp"

namespace tiki
{
	struct GenericDataEnumValue
	{
		string				name;
		sint64				value;
		GenericDataTypeMode	mode;
	};

	class GenericDataEnum : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataEnum );

	public:

											GenericDataEnum( const GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataType& baseType );
		virtual								~GenericDataEnum();

		virtual bool						loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot ) TIKI_OVERRIDE;

		virtual GenericDataTypeType			getType() const TIKI_OVERRIDE;
		virtual uint						getAlignment() const TIKI_OVERRIDE;
		virtual uint						getSize() const TIKI_OVERRIDE;
		const GenericDataType*				getBaseType() const;

		void								addValue( const string& name, sint64 value, GenericDataTypeMode mode = GenericDataTypeMode_ToolAndRuntime );
		void								removeValue( const string& name );

		const sint64*						getValueByName( const string& name ) const;
		const List< GenericDataEnumValue >&	getValues() const;

	private:

		const GenericDataType*			m_pBaseType;

		List< GenericDataEnumValue >	m_values;

	};
}

#endif // TIKI_GENERICDATAENUM_HPP_INCLUDED__
