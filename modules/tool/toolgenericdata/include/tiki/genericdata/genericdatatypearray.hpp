#pragma once
#ifndef TIKI_GENERICDATATYPEARRAY_HPP_INCLUDED__
#define TIKI_GENERICDATATYPEARRAY_HPP_INCLUDED__

#include "tiki/genericdata/genericdatatype.hpp"

namespace tiki
{
	class GenericDataTypeArray : public GenericDataType
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeArray );

	public:

									GenericDataTypeArray( const GenericDataTypeCollection& collection, const string& name, const GenericDataType* pBaseType, GenericDataTypeMode mode );
		virtual						~GenericDataTypeArray();

		virtual bool				loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot ) TIKI_OVERRIDE;
		virtual bool				exportCode( string& targetData, GenericDataTypeMode mode ) const TIKI_OVERRIDE;

		virtual GenericDataTypeType	getType() const TIKI_OVERRIDE;
		virtual uint				getAlignment() const TIKI_OVERRIDE;
		virtual uint				getSize() const TIKI_OVERRIDE;

		const GenericDataType*		getBaseType() const;

	private:

		const GenericDataType*		m_pBaseType;

	};
}

#endif // TIKI_GENERICDATATYPEARRAY_HPP_INCLUDED__
