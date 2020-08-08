#pragma once

#include "tiki/tool_generic_data/generic_data_type.hpp"

namespace tiki
{
	class GenericDataTypeArray : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeArray );

	public:

									GenericDataTypeArray( GenericDataTypeCollection& collection, const DynamicString& name, const DynamicString& filename, GenericDataTypeMode mode, const GenericDataType* pBaseType );
		virtual						~GenericDataTypeArray();

		virtual bool				loadFromXml( XmlElement* pTypeNode ) TIKI_OVERRIDE_FINAL;
		virtual bool				exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE_FINAL;

		virtual uint				getAlignment() const TIKI_OVERRIDE_FINAL;
		virtual uint				getSize() const TIKI_OVERRIDE_FINAL;
		virtual DynamicString		getCodeExportName() const TIKI_OVERRIDE_FINAL;
		virtual crc32				getTypeCrc() const TIKI_OVERRIDE_FINAL;

		const GenericDataType*		getBaseType() const;

	private:

		const GenericDataType*		m_pBaseType;
	};
}
