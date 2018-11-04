#pragma once
#ifndef TIKI_GENERICDATATYPERESOURCE_HPP_INCLUDED__
#define TIKI_GENERICDATATYPERESOURCE_HPP_INCLUDED__

#include "tiki/toolgenericdata/generic_data_type.hpp"

#include "tiki/base/dynamic_string.hpp"
#include "tiki/container/fixedarray.hpp"

namespace tiki
{
	class GenericDataTypeResource : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeResource );

	public:

										GenericDataTypeResource( GenericDataTypeCollection& collection, const string& name, const string& filename, GenericDataTypeMode mode, const GenericDataType* pBaseType );
										GenericDataTypeResource( GenericDataTypeCollection& collection, const string& name, const string& filename, GenericDataTypeMode mode, const string& postFix, fourcc fourCC );
		virtual							~GenericDataTypeResource();

		virtual bool					loadFromXml( XmlElement* pTypeNode ) TIKI_OVERRIDE_FINAL;
		virtual bool					exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE_FINAL;

		virtual uint					getAlignment() const TIKI_OVERRIDE_FINAL;
		virtual uint					getSize() const TIKI_OVERRIDE_FINAL;
		virtual string					getCodeExportName() const TIKI_OVERRIDE_FINAL;
		virtual crc32					getTypeCrc() const TIKI_OVERRIDE_FINAL;

		const GenericDataType*			getBaseType() const;
		const string&					getPostFix() const;
		fourcc							getFourCC() const;

	private:

		const GenericDataType*			m_pBaseType;

		string							m_postFix;
		FixedArray< char, 4 >			m_fourCC;
	};
}

#endif // TIKI_GENERICDATATYPERESOURCE_HPP_INCLUDED__
