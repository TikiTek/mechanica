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

		DynamicString							code;
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

									GenericDataType( GenericDataTypeCollection& collection, const DynamicString& name, const DynamicString& filename, GenericDataTypeType type, GenericDataTypeMode mode );
		virtual						~GenericDataType();

		virtual bool				loadFromXml( XmlElement* pTypeNode ) = 0;
		XmlElement*					createXmlElement( XmlDocument& document, const char* pName ) const;
		virtual bool				exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const = 0;

		const DynamicString&		getName() const { return m_name; }
		const DynamicString&		getModule() const { return m_module; }
		void						setModule( const DynamicString& value ) { m_module = value; }
		const DynamicString&		getFilename() const { return m_filename; }
		GenericDataTypeMode			getMode() const { return m_mode; }
		GenericDataTypeType			getType() const { return m_type; }

		virtual uint				getAlignment() const = 0;
		virtual uint				getSize() const = 0;
		virtual DynamicString		getCodeExportName() const = 0;
		virtual crc32				getTypeCrc() const = 0;

		virtual bool				isTypeCompatible( const GenericDataType* pType ) const { return pType == this; }

		const GenericDataValue*		getDefaultValue() const { return m_pDefaultValue; }

	protected:

		GenericDataTypeCollection&	m_collection;

		GenericDataTag*				m_pTypeTag;
		GenericDataValue*			m_pDefaultValue;

	private:

		DynamicString						m_name;
		DynamicString						m_module;
		DynamicString						m_filename;

		GenericDataTypeType			m_type;
		GenericDataTypeMode			m_mode;
	};
}
