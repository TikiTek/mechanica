
#include "tiki/toolgenericdata/genericdatatypestruct.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/string.hpp"
#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/genericdatatypecollection.hpp"

namespace tiki
{
	GenericDataTypeStruct::GenericDataTypeStruct( GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode, const GenericDataTypeStruct* pBaseType )
		: GenericDataType( collection, name, mode )
		, m_pBaseType( pBaseType )
	{
		if ( m_pBaseType != nullptr )
		{
			const List< GenericDataStructField >& fields = m_pBaseType->getFields();
			for (uint i = 0u; i < fields.getCount(); ++i)
			{
				m_fields.add( fields[ i ] );
			}
		}
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

		if ( !isStringEquals( pTypeRoot->name, "struct" ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] node has a wrong tag('%s' != 'struct') \n", getName().cStr(), pTypeRoot->name );
			return false;
		}

		const XmlElement* pChildElement = pTypeRoot->elements;
		while ( pChildElement != nullptr )
		{
			const bool isField	= isStringEquals( pChildElement->name, "field" );
			const bool isValue	= isStringEquals( pChildElement->name, "value" );
			if ( isField || isValue )
			{
				const XmlAttribute* pNameAtt = reader.findAttributeByName( "name", pChildElement );
				const XmlAttribute* pTypeAtt = reader.findAttributeByName( "type", pChildElement );
				const XmlAttribute* pModeAtt = reader.findAttributeByName( "mode", pChildElement );
				const XmlAttribute* pValueAtt = reader.findAttributeByName( "value", pChildElement );
				if ( pNameAtt && pTypeAtt )
				{
					const GenericDataType* pType = m_collection.parseType( pTypeAtt->content );
					if ( pType == nullptr )
					{
						TIKI_TRACE_WARNING( "[GenericDataStruct(%s)::readFromXml] Type(%s) for field with name '%s' can't be found.\n", getName().cStr(), pTypeAtt->content, pNameAtt->content );
						return false;
					}

					GenericDataStructField* pValueField = nullptr;
					const string fieldName = pNameAtt->content;
					if ( isValue )
					{						
						for (uint i = 0u; i < m_fields.getCount(); ++i)
						{
							GenericDataStructField& field = m_fields[ i ];
							if ( field.name == fieldName )
							{
								pValueField = &field;
								break;
							}
						}

						if ( pValueField == nullptr )
						{
							TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] field with name '%s' can't found in base class.\n", getName().cStr(), pNameAtt->content );
							return false;
						}
					}

					GenericDataStructField& field = (isValue ? *pValueField : m_fields.add());
					if ( isField )
					{
						field.name			= fieldName;
						field.pType			= pType;
						field.defaultValue	= GenericDataValue( pType );
						field.mode			= GenericDataTypeMode_ToolAndRuntime;
					}
					
					if ( isValue && field.pType != pType )
					{
						TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] field with name '%s' must have the same type like in base class.\n", getName().cStr(), pNameAtt->content );
						return false;
					}

					if ( pModeAtt != nullptr )
					{
						if ( !isValue )
						{
							GenericDataTypeMode mode = m_collection.findModeByName( pModeAtt->content );
							if ( mode == GenericDataTypeMode_Invalid )
							{
								TIKI_TRACE_WARNING( "[GenericDataStruct(%s)::readFromXml] field with name '%s' has a invalid mode attribute. '%s' is not a valid mode.\n", getName().cStr(), pNameAtt->content, pModeAtt->content );
							}
							else
							{
								field.mode = mode;
							}
						}
						else
						{
							TIKI_TRACE_WARNING( "[GenericDataStruct(%s)::readFromXml] can't override mode in value(%s).\n", getName().cStr(), pNameAtt->content );
						}
					}

					if ( pValueAtt != nullptr )
					{
						if ( !m_collection.parseValue( field.defaultValue, pValueAtt->content, pType, this ) )
						{
							TIKI_TRACE_INFO( "[GenericDataStruct(%s)::readFromXml] default value of '%s' can't be parsed.\n", getName().cStr(), pNameAtt->content );
						}
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

	bool GenericDataTypeStruct::exportCode( GenericDataExportData& targetData, GenericDataTypeMode mode ) const
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
			if ( !isBitSet( field.mode, mode ) )
			{
				continue;
			}

			targetData.containsArray	|= (field.pType->getType() == GenericDataTypeType_Array);
			targetData.containsString	|= (field.pType->getName() == "string");
			
			if (field.pType->getType() == GenericDataTypeType_Reference)
			{
				const GenericDataTypeReference* pRefType = (const GenericDataTypeReference*)field.pType;
				if ( !targetData.references.contains( pRefType ) )
				{
					targetData.references.add( pRefType );
				}				
			}

			fieldsCode += formatString( s_pFieldFormat, field.pType->getExportName().cStr(), field.name.cStr() );
		}

		string baseTypeCode;
		if ( m_pBaseType != nullptr )
		{
			baseTypeCode = formatString( s_pBaseTypeFormat, m_pBaseType->getExportName().cStr() );
		}

		targetData.code += formatString( s_pBaseFormat, getExportName().cStr(), baseTypeCode.cStr(), fieldsCode.cStr() );

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

	string GenericDataTypeStruct::getExportName() const
	{
		return getName();
	}

	crc32 GenericDataTypeStruct::getTypeCrc() const
	{
		string typeString;
		for (size_t i = 0u; i < m_fields.getCount(); ++i)
		{
			const GenericDataStructField& field = m_fields[ i ];
			typeString += formatString( "%s%08x", field.name, field.pType->getTypeCrc() );
		}

		return crcString( typeString );
	}

	const GenericDataTypeStruct* GenericDataTypeStruct::getBaseType() const
	{
		return m_pBaseType;
	}

	void GenericDataTypeStruct::addField( const string& name, const GenericDataType* pType, GenericDataTypeMode mode /* = GenericDataTypeMode_ToolAndRuntime */ )
	{
		TIKI_ASSERT( pType != nullptr );

		GenericDataStructField& field = m_fields.add();
		field.name			= name;
		field.pType			= pType;
		field.defaultValue	= GenericDataValue( pType );
		field.mode			= mode;
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
}
