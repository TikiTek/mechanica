#pragma once

#include "tiki/tool_generic_data/generic_data_type.hpp"

#include "tiki/base/dynamic_string.hpp"
#include "tiki/container/fixed_array.hpp"

namespace tiki
{
	class GenericDataTypeResource : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeResource );

	public:

										GenericDataTypeResource( GenericDataTypeCollection& collection, const DynamicString& name, const DynamicString& filename, GenericDataTypeMode mode, const GenericDataType* pBaseType, bool isGenericDataType );
										GenericDataTypeResource( GenericDataTypeCollection& collection, const DynamicString& name, const DynamicString& filename, GenericDataTypeMode mode, const DynamicString& postFix, fourcc fourCC );
		virtual							~GenericDataTypeResource();

		virtual bool					loadFromXml( XmlElement* pTypeNode ) TIKI_OVERRIDE_FINAL;
		virtual bool					exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE_FINAL;

		virtual uint					getAlignment() const TIKI_OVERRIDE_FINAL;
		virtual uint					getSize() const TIKI_OVERRIDE_FINAL;
		virtual DynamicString			getCodeExportName() const TIKI_OVERRIDE_FINAL;
		virtual crc32					getTypeCrc() const TIKI_OVERRIDE_FINAL;

		const GenericDataType*			getBaseType() const;
		const DynamicString&			getPostFix() const;
		fourcc							getFourCC() const;

	private:

		const GenericDataType*			m_pBaseType;

		bool							m_isGenericData;
		DynamicString					m_postFix;
		FixedArray< char, 4 >			m_fourCC;
	};
}
