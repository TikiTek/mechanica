#pragma once
#ifndef TIKI_GENERICDATATYPE_HPP_INCLUDED__
#define TIKI_GENERICDATATYPE_HPP_INCLUDED__

#include "tiki/base/basicstring.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/list.hpp"
#include "tiki/toolgenericdata/genericdatatypetype.hpp"

struct _XmlElement;

namespace tiki
{
	class GenericDataTypeCollection;
	class GenericDataTypeReference;
	class GenericDataValue;
	class XmlReader;

	enum GenericDataTypeMode
	{
		GenericDataTypeMode_Invalid			= 0,

		GenericDataTypeMode_RuntimeOnly		= 1 << 0,
		GenericDataTypeMode_ToolOnly		= 1 << 1,
		GenericDataTypeMode_ToolAndRuntime	= GenericDataTypeMode_RuntimeOnly | GenericDataTypeMode_ToolOnly
	};

	struct GenericDataExportData
	{
		GenericDataExportData()
		{
			containsResource	= false;
			containsArray		= false;
			containsString		= false;
		}

		string									code;
		List< const GenericDataTypeReference* >	references;
		bool									containsResource;
		bool									containsArray;
		bool									containsString;
	};

	class GenericDataType : public LinkedItem< GenericDataType >
	{
		TIKI_NONCOPYABLE_CLASS( GenericDataType );
		friend class GenericDataTypeCollection;

	public:
		
									GenericDataType( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode );
		virtual						~GenericDataType();

		virtual bool				loadFromXml( const XmlReader& reader, const _XmlElement* pTypeRoot ) TIKI_PURE;
		virtual bool				exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_PURE;

		const string&				getName() const;
		GenericDataTypeMode			getMode() const;

		virtual GenericDataTypeType	getType() const TIKI_PURE;
		virtual uint				getAlignment() const TIKI_PURE;
		virtual uint				getSize() const TIKI_PURE;
		virtual string				getExportName() const TIKI_PURE;
		virtual crc32				getTypeCrc() const TIKI_PURE;

		virtual bool				isTypeCompatible( const GenericDataType* pType ) const;

		const string&				getModule() const;
		void						setModule( const string& value );

		bool						loadValueFromXml( GenericDataValue& outValue, const XmlReader& reader, const _XmlElement* pElement, const GenericDataType* pParentType ) const;

	protected:

		GenericDataTypeCollection&	m_collection;

	private:

		string						m_name;
		string						m_module;
		GenericDataTypeMode			m_mode;

	};
}

#endif // TIKI_GENERICDATATYPE_HPP_INCLUDED__

