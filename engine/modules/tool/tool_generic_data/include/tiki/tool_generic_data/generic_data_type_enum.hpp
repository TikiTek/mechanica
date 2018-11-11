#pragma once

#include "tiki/tool_generic_data/generic_data_type.hpp"

#include "tiki/container/list.hpp"

namespace tiki
{
	class GenericDataTypeValueType;

	struct GenericDataEnumValue
	{
		string				name;
		bool				hasValue;
		GenericDataValue*	pValue;
		GenericDataTypeMode	mode;
	};

	class GenericDataTypeEnum : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeEnum );

	public:

											GenericDataTypeEnum( GenericDataTypeCollection& collection, const string& name, const string& filename, GenericDataTypeMode mode, const GenericDataTypeValueType* pBaseType );
		virtual								~GenericDataTypeEnum();

		virtual bool						loadFromXml( XmlElement* pTypeNode ) TIKI_OVERRIDE_FINAL;
		virtual bool						exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE_FINAL;

		virtual uint						getAlignment() const TIKI_OVERRIDE_FINAL;
		virtual uint						getSize() const TIKI_OVERRIDE_FINAL;
		virtual string						getCodeExportName() const TIKI_OVERRIDE_FINAL;
		virtual crc32						getTypeCrc() const TIKI_OVERRIDE_FINAL;

		const GenericDataType*				getBaseType() const;

		void								addValue( const string& name, sint64 value, GenericDataTypeMode mode = GenericDataTypeMode_ToolAndRuntime );
		void								removeValue( const string& name );

		GenericDataValue*					getValueByName( const string& name );
		const GenericDataValue*				getValueByName( const string& name ) const;
		const List< GenericDataEnumValue >&	getValues() const;

		bool								isValidValue( const GenericDataEnumValue& enumValue ) const;

	private:

		const GenericDataTypeValueType*		m_pBaseType;

		List< GenericDataEnumValue >		m_values;
	};
}
