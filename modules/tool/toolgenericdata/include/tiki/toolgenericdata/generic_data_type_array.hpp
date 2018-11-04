#pragma once
#ifndef TIKI_GENERICDATATYPEARRAY_HPP_INCLUDED__
#define TIKI_GENERICDATATYPEARRAY_HPP_INCLUDED__

#include "tiki/toolgenericdata/generic_data_type.hpp"

namespace tiki
{
	class GenericDataTypeArray : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeArray );

	public:

									GenericDataTypeArray( GenericDataTypeCollection& collection, const string& name, const string& filename, GenericDataTypeMode mode, const GenericDataType* pBaseType );
		virtual						~GenericDataTypeArray();

		virtual bool				loadFromXml( XmlElement* pTypeNode ) TIKI_OVERRIDE_FINAL;
		virtual bool				exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE_FINAL;

		virtual uint				getAlignment() const TIKI_OVERRIDE_FINAL;
		virtual uint				getSize() const TIKI_OVERRIDE_FINAL;
		virtual string				getCodeExportName() const TIKI_OVERRIDE_FINAL;
		virtual crc32				getTypeCrc() const TIKI_OVERRIDE_FINAL;

		const GenericDataType*		getBaseType() const;

	private:

		const GenericDataType*		m_pBaseType;
	};
}

#endif // TIKI_GENERICDATATYPEARRAY_HPP_INCLUDED__
