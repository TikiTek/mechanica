#pragma once
#ifndef TIKI_GENERICDATAENUM_HPP_INCLUDED__
#define TIKI_GENERICDATAENUM_HPP_INCLUDED__

#include "tiki/toolgenericdata/genericdatatype.hpp"

#include "tiki/container/list.hpp"

namespace tiki
{
	class GenericDataTypeValueType;
	
	struct GenericDataEnumValue
	{
		string				name;
		bool				hasValue;
		sint64				value;
		GenericDataTypeMode	mode;
	};

	class GenericDataTypeEnum : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeEnum );

	public:

											GenericDataTypeEnum( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataTypeValueType* pBaseType );
		virtual								~GenericDataTypeEnum();

		virtual bool						loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot ) TIKI_OVERRIDE;
		virtual bool						exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE;

		virtual GenericDataTypeType			getType() const TIKI_OVERRIDE;
		virtual uint						getAlignment() const TIKI_OVERRIDE;
		virtual uint						getSize() const TIKI_OVERRIDE;
		virtual string						getExportName() const TIKI_OVERRIDE;
		virtual crc32						getTypeCrc() const TIKI_OVERRIDE;

		const GenericDataType*				getBaseType() const;

		void								addValue( const string& name, sint64 value, GenericDataTypeMode mode = GenericDataTypeMode_ToolAndRuntime );
		void								removeValue( const string& name );

		const sint64*						getValueByName( const string& name ) const;
		const List< GenericDataEnumValue >&	getValues() const;

	private:

		const GenericDataTypeValueType*	m_pBaseType;

		List< GenericDataEnumValue >		m_values;

	};
}

#endif // TIKI_GENERICDATAENUM_HPP_INCLUDED__
