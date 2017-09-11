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

									GenericDataTypeArray( GenericDataTypeCollection& collection, const string& name, const GenericDataType* pBaseType, GenericDataTypeMode mode );
		virtual						~GenericDataTypeArray();

		virtual bool				loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot ) TIKI_OVERRIDE;
		virtual bool				exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE;

		virtual uint				getAlignment() const TIKI_OVERRIDE;
		virtual uint				getSize() const TIKI_OVERRIDE;
		virtual string				getExportName() const TIKI_OVERRIDE;
		virtual crc32				getTypeCrc() const TIKI_OVERRIDE;

		const GenericDataType*		getBaseType() const;

	private:

		const GenericDataType*		m_pBaseType;

	};
}

#endif // TIKI_GENERICDATATYPEARRAY_HPP_INCLUDED__
