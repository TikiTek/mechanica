#pragma once
#ifndef TIKI_GENERICDATATYPE_HPP_INCLUDED__
#define TIKI_GENERICDATATYPE_HPP_INCLUDED__

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/container/list.hpp"
#include "tiki/toolgenericdata/genericdatatypetype.hpp"

struct _XmlElement;

namespace tiki
{
	class GenericDataTag;
	class GenericDataTypeCollection;
	class GenericDataTypeReference;
	class GenericDataValue;
	class XmlDocument;
	class XmlElement;

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

									GenericDataType( GenericDataTypeCollection& collection, const string& name, GenericDataTypeType type, GenericDataTypeMode mode );
		virtual						~GenericDataType();

		virtual bool				loadFromXml( XmlElement* pTypeNode ) TIKI_PURE;
		XmlElement*					createXmlElement( XmlDocument& document, const char* pName ) const;
		virtual bool				exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_PURE;

		const string&				getName() const;
		GenericDataTypeMode			getMode() const;
		GenericDataTypeType			getType() const;
		const string&				getModule() const;
		void						setModule( const string& value );

		virtual uint				getAlignment() const TIKI_PURE;
		virtual uint				getSize() const TIKI_PURE;
		virtual string				getCodeExportName() const TIKI_PURE;
		virtual crc32				getTypeCrc() const TIKI_PURE;

		virtual bool				isTypeCompatible( const GenericDataType* pType ) const;

		const GenericDataValue*		getDefaultValue() const;

	protected:

		GenericDataTypeCollection&	m_collection;

		GenericDataTag*				m_pTypeTag;
		GenericDataValue*			m_pDefaultValue;

	private:

		string						m_name;
		string						m_module;
		GenericDataTypeType			m_type;
		GenericDataTypeMode			m_mode;
	};
}

#endif // TIKI_GENERICDATATYPE_HPP_INCLUDED__

