#pragma once
#ifndef TIKI_GENERICDATAVALUETYPE_HPP_INCLUDED__
#define TIKI_GENERICDATAVALUETYPE_HPP_INCLUDED__

#include "tiki/toolgenericdata/genericdatatype.hpp"

namespace tiki
{
	enum GenericDataValueTypeType
	{
		GenericDataTypeValueTypeType_Invalid = -1,

		GenericDataTypeValueTypeType_Boolean,

		GenericDataTypeValueTypeType_SingedInteger8,
		GenericDataTypeValueTypeType_SingedInteger16,
		GenericDataTypeValueTypeType_SingedInteger32,
		GenericDataTypeValueTypeType_SingedInteger64,

		GenericDataTypeValueTypeType_UnsingedInteger8,
		GenericDataTypeValueTypeType_UnsingedInteger16,
		GenericDataTypeValueTypeType_UnsingedInteger32,
		GenericDataTypeValueTypeType_UnsingedInteger64,

		GenericDataTypeValueTypeType_FloatingPoint16,
		GenericDataTypeValueTypeType_FloatingPoint32,
		GenericDataTypeValueTypeType_FloatingPoint64,

		GenericDataTypeValueTypeType_String,

		GenericDataTypeValueTypeType_Count
	};

	class GenericDataTypeValueType : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeValueType );

	public:

									GenericDataTypeValueType( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, GenericDataValueTypeType type );
		virtual						~GenericDataTypeValueType();

		virtual bool				loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot ) TIKI_OVERRIDE;
		virtual bool				exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE;

		virtual GenericDataTypeType	getType() const TIKI_OVERRIDE;
		virtual uint				getAlignment() const TIKI_OVERRIDE;
		virtual uint				getSize() const TIKI_OVERRIDE;
		virtual string				getExportName() const TIKI_OVERRIDE;

		bool						isBoolean() const;
		bool						isSignedInteger() const;
		bool						isUnsignedInteger() const;
		bool						isFloatingPoint() const;
		bool						isString() const;

	private:

		GenericDataValueTypeType	m_type;

	};
	
}

#endif // TIKI_GENERICDATAVALUETYPE_HPP_INCLUDED__
