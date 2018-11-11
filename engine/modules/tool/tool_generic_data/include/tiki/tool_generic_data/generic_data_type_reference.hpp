#pragma once

#include "tiki/base/types.hpp"
#include "tiki/tool_generic_data/generic_data_type.hpp"

namespace tiki
{
	class GenericDataTypeResource;

	class GenericDataTypeReference : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeReference );

	public:

										GenericDataTypeReference( GenericDataTypeCollection& collection, const string& name, const string& filename, GenericDataTypeMode mode, const GenericDataTypeResource* pBaseType );
		virtual							~GenericDataTypeReference();

		virtual bool					loadFromXml( XmlElement* pTypeNode ) TIKI_OVERRIDE_FINAL;
		virtual bool					exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE_FINAL;

		virtual uint					getAlignment() const TIKI_OVERRIDE_FINAL;
		virtual uint					getSize() const TIKI_OVERRIDE_FINAL;
		virtual string					getCodeExportName() const TIKI_OVERRIDE_FINAL;
		virtual crc32					getTypeCrc() const TIKI_OVERRIDE_FINAL;

		const GenericDataTypeResource*	getBaseType() const;

	private:

		const GenericDataTypeResource*	m_pBaseType;
	};
}
