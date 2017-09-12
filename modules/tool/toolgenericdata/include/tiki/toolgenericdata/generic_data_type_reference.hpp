#pragma once
#ifndef TIKI_GENERICDATATYPEREFERENCE_HPP_INCLUDED__
#define TIKI_GENERICDATATYPEREFERENCE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/toolgenericdata/generic_data_type.hpp"

namespace tiki
{
	class GenericDataTypeResource;

	class GenericDataTypeReference : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeReference );

	public:

										GenericDataTypeReference( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataTypeResource* pBaseType );
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

#endif // TIKI_GENERICDATATYPEREFERENCE_HPP_INCLUDED__
