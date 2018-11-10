#include "tiki/toolgenericdata/generic_data_type_struct.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/string.hpp"
#include "tiki/toolgenericdata/generic_data_object.hpp"
#include "tiki/toolgenericdata/generic_data_type_collection.hpp"
#include "tiki/toolxml/xml_attribute.hpp"
#include "tiki/toolxml/xml_element.hpp"

namespace tiki
{
	GenericDataTypeStruct::GenericDataTypeStruct( GenericDataTypeCollection& collection, const string& name, const string& filename, GenericDataTypeMode mode, const GenericDataTypeStruct* pBaseType )
		: GenericDataType( collection, name, filename, GenericDataTypeType_Struct, mode )
		, m_pBaseType( pBaseType )
	{
		m_pDefaultObject	= nullptr;
		m_alignment			= 1u;
		m_size				= 0u;

		if ( m_pBaseType != nullptr )
		{
			const List< GenericDataStructField >& fields = m_pBaseType->getFields();
			for (uint i = 0u; i < fields.getCount(); ++i)
			{
				GenericDataStructField& field = m_fields.add();
				field				= fields[ i ];
				field.isInherited	= true;

				m_alignment	= TIKI_MAX( m_alignment, field.pType->getAlignment() );
				m_size		= alignValue( m_size, field.pType->getAlignment() );
				m_size		= m_size + field.pType->getSize();
			}

			m_localFieldOffset = m_fields.getCount();
		}
	}

	GenericDataTypeStruct::~GenericDataTypeStruct()
	{
	}

	bool GenericDataTypeStruct::loadFromXml( XmlElement* pTypeNode )
	{
		if ( pTypeNode == nullptr )
		{
			return false;
		}

		if ( !pTypeNode->isName( "struct" ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] node has a wrong tag('%s' != 'struct') \n", getName().cStr(), pTypeNode->getName() );
			return false;
		}

		for( XmlElement* pChildNode = pTypeNode->getFirstChild(); pChildNode != nullptr; pChildNode = pChildNode->getNextSibling() )
		{
			if( !pChildNode->isName( "field" ) )
			{
				continue;
			}

			const XmlAttribute* pNameAtt = pChildNode->findAttribute( "name" );
			const XmlAttribute* pTypeAtt = pChildNode->findAttribute( "type" );
			if( pNameAtt == nullptr || pTypeAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataStruct(%s)::readFromXml] field or array has not all required attributes. name and type are required.\n", getName().cStr() );
				return false;
			}

			const GenericDataType* pType = m_collection.parseType( pTypeAtt->getValue() );
			if( pType == nullptr )
			{
				TIKI_TRACE_WARNING( "[GenericDataStruct(%s)::readFromXml] Type(%s) for field with name '%s' can't be found.\n", getName().cStr(), pTypeAtt->getValue(), pNameAtt->getValue() );
				return false;
			}

			GenericDataStructField& field = m_fields.add();
			field.name			= pNameAtt->getValue();
			field.pType			= pType;
			field.mode			= GenericDataTypeMode_ToolAndRuntime;
			field.isInherited	= false;

			m_alignment			= TIKI_MAX( m_alignment, field.pType->getAlignment() );
			m_size				= alignValue( m_size, field.pType->getAlignment() );
			m_size				= m_size + field.pType->getSize();

			const XmlAttribute* pModeAtt = pChildNode->findAttribute( "mode" );
			if( pModeAtt != nullptr )
			{
				GenericDataTypeMode mode = m_collection.findModeByName( pModeAtt->getValue() );
				if( mode == GenericDataTypeMode_Invalid )
				{
					TIKI_TRACE_WARNING( "%s(%u): field with name '%s' has a invalid mode attribute. '%s' is not a valid mode.\n", getFilename().cStr(), pChildNode->getLineNumber(), pNameAtt->getValue(), pModeAtt->getValue() );
				}
				else
				{
					field.mode = mode;
				}
			}
		}

		m_pDefaultObject = TIKI_NEW( GenericDataObject )( m_collection );
		m_pDefaultObject->create( this, nullptr );
		TIKI_VERIFY( m_pDefaultValue->setObject( m_pDefaultObject ) );

		for( XmlElement* pChildNode = pTypeNode->getFirstChild(); pChildNode != nullptr; pChildNode = pChildNode->getNextSibling() )
		{
			const bool isField	= pChildNode->isName( "field" );
			const bool isValue	= pChildNode->isName( "value" );
			if( !isField && !isValue )
			{
				continue;
			}

			const XmlAttribute* pNameAtt = pChildNode->findAttribute( "name" );
			const XmlAttribute* pTypeAtt = pChildNode->findAttribute( "type" );
			if( pNameAtt == nullptr || pTypeAtt == nullptr )
			{
				TIKI_TRACE_ERROR( "%s(%u): %s of '%s' has not all required attributes. name and type are required.\n", getFilename().cStr(), pChildNode->getLineNumber(), pChildNode->getName(), getName().cStr() );
				return false;
			}

			const GenericDataType* pType = m_collection.parseType( pTypeAtt->getValue() );
			if ( pType == nullptr )
			{
				TIKI_TRACE_WARNING( "%s(%u): Type '%s' for field with name '%s' can't be found.\n", getFilename().cStr(), pChildNode->getLineNumber(), pTypeAtt->getValue(), pNameAtt->getValue() );
				return false;
			}

			GenericDataStructField* pValueField = nullptr;
			const string fieldName = pNameAtt->getValue();
			const uint fieldIndex = getFieldIndexByName( fieldName );
			if( fieldIndex == (uint)-1 )
			{
				TIKI_TRACE_WARNING( "%s(%u): Could not find field aith name '%s'.\n", getFilename().cStr(), pChildNode->getLineNumber(), pNameAtt->getValue() );
				return false;
			}

			GenericDataStructField& field = m_fields[ fieldIndex ];
			if ( isValue && field.pType != pType )
			{
				TIKI_TRACE_ERROR( "%s(%u): field with name '%s' must have the same type like in base class.\n", getFilename().cStr(), pChildNode->getLineNumber(), pNameAtt->getValue() );
				return false;
			}

			const XmlAttribute* pModeAtt = pChildNode->findAttribute( "mode" );
			if ( pModeAtt != nullptr && isValue )
			{
				TIKI_TRACE_WARNING( "%s(%u): can't override mode in value(%s).\n", getFilename().cStr(), pChildNode->getLineNumber(), pNameAtt->getValue() );
			}

			GenericDataValue* pDefaultValue = m_pDefaultObject->getFieldValue( fieldIndex, true );

			bool ok = true;
			const XmlAttribute* pValueAtt = pChildNode->findAttribute( "value" );
			if ( pValueAtt != nullptr )
			{
				if ( !m_collection.parseValue( pDefaultValue, pValueAtt->getValue(), pType, this ) )
				{
					TIKI_TRACE_ERROR( "%s(%u): default value of '%s' can't be parsed.\n", getFilename().cStr(), pChildNode->getLineNumber(), pNameAtt->getValue() );
					ok = false;
				}
			}
			else
			{
				XmlElement* pValueNode = pChildNode->findFirstChild( "value" );
				if( pValueNode != nullptr )
				{
					if( !pDefaultValue->importFromXml( pValueNode, pType, m_pDefaultObject, m_collection, true ) )
					{
						TIKI_TRACE_ERROR( "%s(%u): default value node can't be parsed.\n", getFilename().cStr(), pChildNode->getLineNumber() );
						ok = false;
					}
				}
				else
				{
					if( !pDefaultValue->setCopyFromValue( m_collection, pType->getDefaultValue() ) )
					{
						TIKI_TRACE_ERROR( "%s(%u): could not copy default value of type '%s'.\n", getFilename().cStr(), pChildNode->getLineNumber(), pType->getName().cStr() );
						ok = false;
					}
				}
			}

			if( !ok )
			{
				m_pDefaultObject->removeField( fieldIndex );
			}
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

		string fieldsCode = "";
		for (uint i = 0u; i < m_fields.getCount(); ++i)
		{
			const GenericDataStructField& field = m_fields[ i ];
			if ( !isBitSet( field.mode, mode ) || field.isInherited )
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

			fieldsCode += formatDynamicString( s_pFieldFormat, field.pType->getCodeExportName().cStr(), field.name.cStr() );
		}

		string baseTypeCode = "";
		if ( m_pBaseType != nullptr )
		{
			baseTypeCode = formatDynamicString( s_pBaseTypeFormat, m_pBaseType->getCodeExportName().cStr() );
		}

		targetData.code += formatDynamicString( s_pBaseFormat, getCodeExportName().cStr(), baseTypeCode.cStr(), fieldsCode.cStr() );

		return true;
	}

	uint GenericDataTypeStruct::getAlignment() const
	{
		return m_alignment;
	}

	uint GenericDataTypeStruct::getSize() const
	{
		return m_size;
	}

	string GenericDataTypeStruct::getCodeExportName() const
	{
		return getName();
	}

	crc32 GenericDataTypeStruct::getTypeCrc() const
	{
		string typeString;
		for (size_t i = 0u; i < m_fields.getCount(); ++i)
		{
			const GenericDataStructField& field = m_fields[ i ];
			typeString += formatDynamicString( "%s%08x", field.name.cStr(), field.pType->getTypeCrc() );
		}

		return crcString( typeString );
	}

	const GenericDataTypeStruct* GenericDataTypeStruct::getBaseType() const
	{
		return m_pBaseType;
	}

	//void GenericDataTypeStruct::addField( const string& name, const GenericDataType* pType, GenericDataTypeMode mode /* = GenericDataTypeMode_ToolAndRuntime */ )
	//{
	//	TIKI_ASSERT( pType != nullptr );
	//
	//	GenericDataStructField& field = m_fields.add();
	//	field.name			= name;
	//	field.pType			= pType;
	//	field.mode			= mode;
	//	field.isInherited	= false;
	//}
	//
	//void GenericDataTypeStruct::removeField( const string& name )
	//{
	//	for (uint i = 0u; i < m_fields.getCount(); ++i)
	//	{
	//		if ( m_fields[ i ].name == name )
	//		{
	//			m_fields.removeSortedAtIndex( i );
	//			return;
	//		}
	//	}
	//}

	const List< GenericDataStructField >& GenericDataTypeStruct::getFields() const
	{
		return m_fields;
	}

	const GenericDataStructField* GenericDataTypeStruct::getFieldByName( const string& name ) const
	{
		for (uint i = 0u; i < m_fields.getCount(); ++i)
		{
			if ( m_fields[ i ].name == name )
			{
				return &m_fields[ i ];
			}
		}

		TIKI_TRACE_ERROR( "[GenericDataTypeStruct::getFieldTypeByName] Try to get Type for not existing field. Name: %s.\n", name.cStr() );
		return nullptr;
	}

	uint GenericDataTypeStruct::getFieldIndexByName( const string& name ) const
	{
		for( uint i = 0; i < m_fields.getCount(); i++ )
		{
			if( m_fields[ i ].name == name )
			{
				return i;
			}
		}

		return (uint)-1;
	}

	GenericDataTypeStruct::FieldArrayView GenericDataTypeStruct::getLocalFields() const
	{
		return FieldArrayView( m_fields.getBegin() + m_localFieldOffset, m_fields.getCount() - m_localFieldOffset );
	}
}
