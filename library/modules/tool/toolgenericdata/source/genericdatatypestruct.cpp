
#include "tiki/genericdata/genericdatatypestruct.hpp"

#include "tiki/base/string.hpp"
#include "tiki/genericdata/genericdatatypecollection.hpp"
#include "tiki/io/xmlreader.hpp"

namespace tiki
{
	GenericDataTypeStruct::GenericDataTypeStruct( const GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataType* pBaseType )
		: GenericDataType( collection, name, mode )
		, m_pBaseType( pBaseType )
	{
	}

	GenericDataTypeStruct::~GenericDataTypeStruct()
	{
	}

	bool GenericDataTypeStruct::loadFromXml( const XmlReader& reader, const XmlElement* pTypeRoot )
	{
		if ( pTypeRoot == nullptr )
		{
			return false;
		}

		if ( !isStringEquals( pTypeRoot->name, getNodeName() ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] node has a wrong tag('%s' != '%s') \n", getName().cStr(), pTypeRoot->name, getNodeName() );
			return false;
		}

		const XmlElement* pChildElement = pTypeRoot->elements;
		while ( pChildElement != nullptr )
		{
			const bool isField	= isStringEquals( pChildElement->name, "field" );
			const bool isArray	= isStringEquals( pChildElement->name, "array" );
			if ( isField || isArray )
			{
				const XmlAttribute* pNameAtt = reader.findAttributeByName( "name", pChildElement );
				const XmlAttribute* pTypeAtt = reader.findAttributeByName( "type", pChildElement );
				const XmlAttribute* pModeAtt = reader.findAttributeByName( "mode", pChildElement );
				const XmlAttribute* pValueAtt = reader.findAttributeByName( "value", pChildElement );
				if ( pNameAtt && pTypeAtt )
				{
					const GenericDataType* pType = m_collection.findTypeByName( pTypeAtt->content );
					if ( pType == nullptr )
					{
						TIKI_TRACE_WARNING( "[GenericDataStruct(%s)::readFromXml] the type(%s) of field or array with name '%s' can't be found.\n", getName().cStr(), pTypeAtt->content, pNameAtt->content );
						return false;
					}

					GenericDataStructField& field = m_fields.add();
					field.name		= pNameAtt->content;
					field.pType		= pType;
					field.isArray	= isArray;
					field.mode		= GenericDataTypeMode_ToolAndRuntime;

					if ( pModeAtt != nullptr )
					{
						GenericDataTypeMode mode = m_collection.findModeByName( pModeAtt->content );
						if ( mode == GenericDataTypeMode_Invalid )
						{
							TIKI_TRACE_WARNING( "[GenericDataStruct(%s)::readFromXml] field or array with name '%s' has a invalid mode attribute. '%s' is not a valid mode.\n", getName().cStr(), pNameAtt->content, pModeAtt->content );
						}
						else
						{
							field.mode = mode;
						}
					}

					if ( pValueAtt != nullptr )
					{
						TIKI_TRACE_INFO( "[GenericDataStruct(%s)::readFromXml] field or array with name '%s' has a value attribute. thats currently not supported.\n", getName().cStr(), pNameAtt->content );
					}
				}
				else
				{
					TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] field or array has not all required attributes. name and type are required.\n", getName().cStr() );
					return false;
				}
			}

			pChildElement = pChildElement->next;
		}

		return true;
	}

	bool GenericDataTypeStruct::exportCode( string& targetData, GenericDataTypeMode mode ) const
	{
		static const char* s_pBaseFormat		= "\n"
												  "\tstruct %s%s\n"
												  "\t{\n"
												  "%s"
												  "\t};\n";

		static const char* s_pBaseTypeFormat	= " : public %s";
		static const char* s_pFieldFormat		= "\t\t%s %s;\n";

		string fieldsCode;
		for (uint i = 0u; i < m_fields.getCount(); ++i)
		{
			const GenericDataStructField& field = m_fields[ i ];

			fieldsCode += formatString( s_pFieldFormat, field.pType->getName().cStr(), field.name.cStr() );
		}

		string baseTypeCode;
		if ( m_pBaseType != nullptr )
		{
			baseTypeCode = formatString( s_pBaseTypeFormat, m_pBaseType->getName().cStr() );
		}

		targetData += formatString( s_pBaseFormat, baseTypeCode.cStr(), getName().cStr(), fieldsCode.cStr() );

		return true;
	}

	GenericDataTypeType GenericDataTypeStruct::getType() const
	{
		return GenericDataTypeType_Struct;
	}

	uint GenericDataTypeStruct::getAlignment() const
	{
		uint alignment = m_alignment;
		if ( m_pBaseType != nullptr )
		{
			alignment = TIKI_MAX( alignment, m_pBaseType->getAlignment() );
		}

		return alignment;
	}

	uint GenericDataTypeStruct::getSize() const
	{
		uint size = m_size;
		if ( m_pBaseType != nullptr )
		{
			size += m_pBaseType->getSize();
		}

		return size;
	}

	void GenericDataTypeStruct::addField( const string& name, const GenericDataType* pType, bool isArray /* = false */, GenericDataTypeMode mode /* = GenericDataTypeMode_ToolAndRuntime */ )
	{
		TIKI_ASSERT( pType != nullptr );

		GenericDataStructField& field = m_fields.add();
		field.name		= name;
		field.pType		= pType;
		field.isArray	= isArray;
		field.mode		= mode;
	}

	void GenericDataTypeStruct::removeField( const string& name )
	{
		for (uint i = 0u; i < m_fields.getCount(); ++i)
		{
			if ( m_fields[ i ].name == name )
			{
				m_fields.removeSortedAtIndex( i );
				return;
			}
		}
	}

	const List< GenericDataStructField >& GenericDataTypeStruct::getFields() const
	{
		return m_fields;
	}

	const GenericDataType* GenericDataTypeStruct::getFieldTypeByName( const string& name ) const
	{
		for (uint i = 0u; i < m_fields.getCount(); ++i)
		{			
			if ( m_fields[ i ].name == name )
			{				
				return m_fields[ i ].pType;
			}
		}

		return nullptr;
	}

	cstring GenericDataTypeStruct::getNodeName() const
	{
		return "struct";
	}
}
