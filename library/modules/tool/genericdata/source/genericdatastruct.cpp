
#include "tiki/genericdata/genericdatastruct.hpp"

#include "tiki/base/string.hpp"
#include "tiki/genericdata/genericdatatypecollection.hpp"
#include "tiki/io/xmlreader.hpp"

namespace tiki
{
	GenericDataStruct::GenericDataStruct( const GenericDataTypeCollection& collection, const string& name, GenericDataTypeMode mode )
		: GenericDataType( collection, name, mode )
	{
	}

	GenericDataStruct::~GenericDataStruct()
	{
	}

	bool GenericDataStruct::loadFromXml( const XmlReader& reader, const XmlElement* pTypeRoot )
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
			const bool isArray	= isStringEquals( pChildElement->name, "array" );
			if ( isField || isArray )
			{
				const XmlAttribute* pNameAtt = reader.findAttributeByName( "name", pChildElement );
				const XmlAttribute* pTypeAtt = reader.findAttributeByName( "type", pChildElement );
				const XmlAttribute* pModeAtt = reader.findAttributeByName( "mode", pChildElement );
				const XmlAttribute* pValueAtt = reader.findAttributeByName( "value", pChildElement );
				if ( pNameAtt && pTypeAtt )
				{
					const GenericDataType* pType = m_collection.getTypeByName( pTypeAtt->content );
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
						GenericDataTypeMode mode = m_collection.getModeByName( pModeAtt->content );
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

	GenericDataTypeType GenericDataStruct::getType() const
	{
		return GenericDataTypeType_Struct;
	}

	uint GenericDataStruct::getAlignment() const
	{
		return m_alignment;
	}

	uint GenericDataStruct::getSize() const
	{
		return m_size;
	}

	void GenericDataStruct::addField( const string& name, const GenericDataType* pType, bool isArray /* = false */, GenericDataTypeMode mode /* = GenericDataTypeMode_ToolAndRuntime */ )
	{
		TIKI_ASSERT( pType != nullptr );

		GenericDataStructField& field = m_fields.add();
		field.name		= name;
		field.pType		= pType;
		field.isArray	= isArray;
		field.mode		= mode;
	}

	void GenericDataStruct::removeField( const string& name )
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

	const List< GenericDataStructField >& GenericDataStruct::getFields() const
	{
		return m_fields;
	}

	const GenericDataType* GenericDataStruct::getFieldTypeByName( const string& name ) const
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
