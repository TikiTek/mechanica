#pragma once
#ifndef TIKI_GENERICDATAVALUETYPE_HPP_INCLUDED__
#define TIKI_GENERICDATAVALUETYPE_HPP_INCLUDED__

#include "tiki/genericdata/genericdatatype.hpp"

namespace tiki
{
	enum GenericDataValueTypeType
	{
		GenericDataValueTypeType_Invalid = -1,

		GenericDataValueTypeType_SingedInteger8,
		GenericDataValueTypeType_SingedInteger16,
		GenericDataValueTypeType_SingedInteger32,
		GenericDataValueTypeType_SingedInteger64,
		GenericDataValueTypeType_UnsingedInteger8,
		GenericDataValueTypeType_UnsingedInteger16,
		GenericDataValueTypeType_UnsingedInteger32,
		GenericDataValueTypeType_UnsingedInteger64,
		GenericDataValueTypeType_FloatingPoint16,
		GenericDataValueTypeType_FloatingPoint32,
		GenericDataValueTypeType_FloatingPoint64,

		GenericDataValueTypeType_Count
	};

	class GenericDataValueType : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataValueType );

	public:

									GenericDataValueType( const GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, GenericDataValueTypeType type );
		virtual						~GenericDataValueType();

		virtual bool				loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot ) TIKI_OVERRIDE;

		virtual GenericDataTypeType	getType() const TIKI_OVERRIDE;
		virtual uint				getAlignment() const TIKI_OVERRIDE;
		virtual uint				getSize() const TIKI_OVERRIDE;

	private:

		GenericDataValueTypeType	m_type;

	};
}

#endif // TIKI_GENERICDATAVALUETYPE_HPP_INCLUDED__
