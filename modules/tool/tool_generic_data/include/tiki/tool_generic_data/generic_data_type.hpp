#pragma once

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/linked_list.hpp"
#include "tiki/container/list.hpp"
#include "tiki/tool_generic_data/generic_data_type_type.hpp"

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

									GenericDataType( GenericDataTypeCollection& collection, const string& name, const string& filename, GenericDataTypeType type, GenericDataTypeMode mode );
		virtual						~GenericDataType();

		virtual bool				loadFromXml( XmlElement* pTypeNode ) TIKI_PURE;
		XmlElement*					createXmlElement( XmlDocument& document, const char* pName ) const;
		virtual bool				exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const TIKI_PURE;

		const string&				getName() const { return m_name; }
		const string&				getModule() const { return m_module; }
		void						setModule( const string& value ) { m_module = value; }
		const string&				getFilename() const { return m_filename; }
		GenericDataTypeMode			getMode() const { return m_mode; }
		GenericDataTypeType			getType() const { return m_type; }

		virtual uint				getAlignment() const TIKI_PURE;
		virtual uint				getSize() const TIKI_PURE;
		virtual string				getCodeExportName() const TIKI_PURE;
		virtual crc32				getTypeCrc() const TIKI_PURE;

		virtual bool				isTypeCompatible( const GenericDataType* pType ) const { return pType == this; }

		const GenericDataValue*		getDefaultValue() const { return m_pDefaultValue; }

	protected:

		GenericDataTypeCollection&	m_collection;

		GenericDataTag*				m_pTypeTag;
		GenericDataValue*			m_pDefaultValue;

	private:

		string						m_name;
		string						m_module;
		string						m_filename;

		GenericDataTypeType			m_type;
		GenericDataTypeMode			m_mode;
	};
}
