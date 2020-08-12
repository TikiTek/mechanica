#pragma once
#ifndef TIKI_GENERICDATATYPEREFERENCE_HPP_INCLUDED__
#define TIKI_GENERICDATATYPEREFERENCE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/toolgenericdata/genericdatatype.hpp"

namespace tiki
{
	class GenericDataTypeResource;

	class GenericDataTypeReference : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeReference );

	public:

										GenericDataTypeReference( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataTypeResource* pBaseType );
		virtual							~GenericDataTypeReference();

		virtual bool					loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot ) TIKI_OVERRIDE;
		virtual bool					exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE;

		virtual GenericDataTypeType		getType() const TIKI_OVERRIDE;
		virtual uint					getAlignment() const TIKI_OVERRIDE;
		virtual uint					getSize() const TIKI_OVERRIDE;
		virtual string					getExportName() const TIKI_OVERRIDE;
		virtual crc32					getTypeCrc() const TIKI_OVERRIDE;

		const GenericDataTypeResource*	getBaseType() const;

	private:

		const GenericDataTypeResource*	m_pBaseType;

	};

}

#endif // TIKI_GENERICDATATYPEREFERENCE_HPP_INCLUDED__
