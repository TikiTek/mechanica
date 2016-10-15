#pragma once
#ifndef TIKI_GENERICDATATYPEPOINTER_HPP_INCLUDED
#define TIKI_GENERICDATATYPEPOINTER_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/toolgenericdata/genericdatatype.hpp"

namespace tiki
{
	class GenericDataTypeStruct;

	class GenericDataTypePointer : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypePointer );

	public:

										GenericDataTypePointer( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataTypeStruct* pBaseType );
		virtual							~GenericDataTypePointer();

		virtual bool					loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot ) TIKI_OVERRIDE_FINAL;
		virtual bool					exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE_FINAL;

		virtual GenericDataTypeType		getType() const TIKI_OVERRIDE_FINAL;
		virtual uint					getAlignment() const TIKI_OVERRIDE_FINAL;
		virtual uint					getSize() const TIKI_OVERRIDE_FINAL;
		virtual string					getExportName() const TIKI_OVERRIDE_FINAL;
		virtual crc32					getTypeCrc() const TIKI_OVERRIDE_FINAL;

		virtual bool					isTypeCompatible( const GenericDataType* pType ) const TIKI_OVERRIDE_FINAL;

		const GenericDataTypeStruct*	getBaseType() const;

	private:

		const GenericDataTypeStruct*	m_pBaseType;
	};
}

#endif // TIKI_GENERICDATATYPEPOINTER_HPP_INCLUDED
