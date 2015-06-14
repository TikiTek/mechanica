#pragma once
#ifndef TIKI_GENERICDATASTRUCT_HPP_INCLUDED__
#define TIKI_GENERICDATASTRUCT_HPP_INCLUDED__

#include "tiki/genericdata/genericdatatype.hpp"

#include "tiki/toolbase/list.hpp"

namespace tiki
{
	struct GenericDataStructField
	{
		string					name;
		const GenericDataType*	pType;
		bool					isArray;
		GenericDataTypeMode		mode;
	};

	class GenericDataStruct : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataStruct );

	public:

												GenericDataStruct( const GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataType* pBaseType );
		virtual									~GenericDataStruct();

		virtual bool							loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot ) TIKI_OVERRIDE;

		virtual GenericDataTypeType				getType() const TIKI_OVERRIDE;
		virtual uint							getAlignment() const TIKI_OVERRIDE;
		virtual uint							getSize() const TIKI_OVERRIDE;

		void									addField( const string& name, const GenericDataType* pType, bool isArray = false, GenericDataTypeMode mode = GenericDataTypeMode_ToolAndRuntime );
		void									removeField( const string& name );

		const List< GenericDataStructField >&	getFields() const;
		const GenericDataType*					getFieldTypeByName( const string& name ) const;

	protected:

		uint							m_size;

	private:

		const GenericDataType*			m_pBaseType;

		uint							m_alignment;	

		List< GenericDataStructField >	m_fields;

	};
}

#endif // TIKI_GENERICDATASTRUCT_HPP_INCLUDED__
