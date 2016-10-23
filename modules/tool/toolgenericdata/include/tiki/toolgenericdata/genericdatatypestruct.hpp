#pragma once
#ifndef TIKI_GENERICDATASTRUCT_HPP_INCLUDED__
#define TIKI_GENERICDATASTRUCT_HPP_INCLUDED__

#include "tiki/toolgenericdata/genericdatatype.hpp"

#include "tiki/container/list.hpp"
#include "tiki/toolgenericdata/genericdatavalue.hpp"

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

												GenericDataTypeStruct( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataTypeStruct* pBaseType );
		virtual									~GenericDataTypeStruct();

		virtual bool							loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot ) TIKI_OVERRIDE;
		virtual bool							exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE;

		virtual GenericDataTypeType				getType() const TIKI_OVERRIDE;
		virtual uint							getAlignment() const TIKI_OVERRIDE;
		virtual uint							getSize() const TIKI_OVERRIDE;
		virtual string							getExportName() const TIKI_OVERRIDE;
		virtual crc32							getTypeCrc() const TIKI_OVERRIDE;

		const GenericDataTypeStruct*			getBaseType() const;

		void									addField( const string& name, const GenericDataType* pType, GenericDataTypeMode mode = GenericDataTypeMode_ToolAndRuntime );
		void									removeField( const string& name );

		const List< GenericDataStructField >&	getFields() const;
		const GenericDataType*					getFieldTypeByName( const string& name ) const;

		const GenericDataObject*				getDefaultObject() const { return m_pDefaultObject; }

	private:

		const GenericDataTypeStruct*			m_pBaseType;

		GenericDataObject*						m_pDefaultObject;

		uint									m_alignment;
		uint									m_size;

		List< GenericDataStructField >			m_fields;
		
	};
}

#endif // TIKI_GENERICDATASTRUCT_HPP_INCLUDED__
