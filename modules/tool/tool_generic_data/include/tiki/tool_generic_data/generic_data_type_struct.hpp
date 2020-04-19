#pragma once

#include "tiki/tool_generic_data/generic_data_type.hpp"

#include "tiki/container/list.hpp"
#include "tiki/tool_generic_data/generic_data_value.hpp"

namespace tiki
{
	struct GenericDataStructField
	{
		DynamicString			name;
		const GenericDataType*	pType;
		GenericDataTypeMode		mode;
		bool					isInherited;
	};

	class GenericDataTypeStruct : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeStruct );

	public:

		typedef ArrayView< GenericDataStructField > FieldArrayView;

												GenericDataTypeStruct( GenericDataTypeCollection& collection, const DynamicString& name, const DynamicString& filename, GenericDataTypeMode mode, const GenericDataTypeStruct* pBaseType );
		virtual									~GenericDataTypeStruct();

		virtual bool							loadFromXml( XmlElement* pTypeNode ) TIKI_OVERRIDE;
		virtual bool							exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE;

		virtual uintreg							getAlignment() const TIKI_OVERRIDE;
		virtual uintreg							getSize() const TIKI_OVERRIDE;
		virtual DynamicString					getCodeExportName() const TIKI_OVERRIDE;
		virtual crc32							getTypeCrc() const TIKI_OVERRIDE;

		const GenericDataTypeStruct*			getBaseType() const;
		bool									isDerivedType( const GenericDataTypeStruct* pType ) const;

		//void									addField( const DynamicString& name, const GenericDataType* pType, GenericDataTypeMode mode = GenericDataTypeMode_ToolAndRuntime );
		//void									removeField( const DynamicString& name );

		const List< GenericDataStructField >&	getFields() const;
		uintreg									getFieldCount() const { return m_fields.getCount(); }
		const GenericDataStructField*			getFieldByName( const DynamicString& name ) const;
		uintreg									getFieldIndexByName( const DynamicString& name ) const;

		FieldArrayView							getLocalFields() const;
		uintreg									getLocalFieldOffset() const { return m_localFieldOffset; }

		const GenericDataObject*				getDefaultObject() const { return m_pDefaultObject; }

	private:

		const GenericDataTypeStruct*			m_pBaseType;

		GenericDataObject*						m_pDefaultObject;

		uintreg									m_alignment;
		uintreg									m_size;

		List< GenericDataStructField >			m_fields;
		uintreg									m_localFieldOffset;
	};
}
