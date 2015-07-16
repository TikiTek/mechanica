#pragma once
#ifndef TIKI_GENERICDATAUNION_HPP_INCLUDED__
#define TIKI_GENERICDATAUNION_HPP_INCLUDED__

#include "tiki/genericdata/genericdatatypestruct.hpp"

namespace tiki
{
	class GenericDataTypeUnion : public GenericDataTypeStruct
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataTypeUnion );

	public:

									GenericDataTypeUnion( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode );
		virtual						~GenericDataTypeUnion();

		virtual bool				loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot ) TIKI_OVERRIDE;

		virtual GenericDataTypeType	getType() const TIKI_OVERRIDE;

	protected:

		virtual cstring				getNodeName() const TIKI_OVERRIDE;

	};
}

#endif // TIKI_GENERICDATAUNION_HPP_INCLUDED__
