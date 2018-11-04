#pragma once
#ifndef TIKI_GENERICDATASTRUCT_HPP_INCLUDED__
#define TIKI_GENERICDATASTRUCT_HPP_INCLUDED__

#include "tiki/toolgenericdata/generic_data_type.hpp"

#include "tiki/container/list.hpp"
#include "tiki/toolgenericdata/generic_data_value.hpp"

namespace tiki
{
	struct GenericDataStructField
	{
		string					name;
		const GenericDataType*	pType;
		GenericDataTypeMode		mode;
		bool					isInherited;
	};

	class GenericDataTypeStruct : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeStruct );

	public:

		typedef ArrayView< GenericDataStructField > FieldArrayView;

												GenericDataTypeStruct( GenericDataTypeCollection& collection, const string& name, const string& filename, GenericDataTypeMode mode, const GenericDataTypeStruct* pBaseType );
		virtual									~GenericDataTypeStruct();

		virtual bool							loadFromXml( XmlElement* pTypeNode ) TIKI_OVERRIDE;
		virtual bool							exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE;

		virtual uint							getAlignment() const TIKI_OVERRIDE;
		virtual uint							getSize() const TIKI_OVERRIDE;
		virtual string							getCodeExportName() const TIKI_OVERRIDE;
		virtual crc32							getTypeCrc() const TIKI_OVERRIDE;

		const GenericDataTypeStruct*			getBaseType() const;

		//void									addField( const string& name, const GenericDataType* pType, GenericDataTypeMode mode = GenericDataTypeMode_ToolAndRuntime );
		//void									removeField( const string& name );

		const List< GenericDataStructField >&	getFields() const;
		uint									getFieldCount() const { return m_fields.getCount(); }
		const GenericDataStructField*			getFieldByName( const string& name ) const;
		uint									getFieldIndexByName( const string& name ) const;

		FieldArrayView							getLocalFields() const;
		uint									getLocalFieldOffset() const { return m_localFieldOffset; }

		const GenericDataObject*				getDefaultObject() const { return m_pDefaultObject; }

	private:

		const GenericDataTypeStruct*			m_pBaseType;

		GenericDataObject*						m_pDefaultObject;

		uint									m_alignment;
		uint									m_size;

		List< GenericDataStructField >			m_fields;
		uint									m_localFieldOffset;
	};
}

#endif // TIKI_GENERICDATASTRUCT_HPP_INCLUDED__
