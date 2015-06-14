
#include "tiki/genericdata/genericdatatypecollection.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/base/path.hpp"
#include "tiki/genericdata/genericdataenum.hpp"
#include "tiki/genericdata/genericdataresource.hpp"
#include "tiki/genericdata/genericdatastruct.hpp"
#include "tiki/genericdata/genericdataunion.hpp"
#include "tiki/genericdata/genericdatavaluetype.hpp"
#include "tiki/io/xmlreader.hpp"
#include "tiki/toolbase/directory_tool.hpp"
#include "tiki/toolbase/list.hpp"

namespace tiki
{
	GenericDataTypeCollection::GenericDataTypeCollection()
	{
		m_pModeEnum = nullptr;
	}

	GenericDataTypeCollection::~GenericDataTypeCollection()
	{
	}

	void GenericDataTypeCollection::create( const char* pFolderName, bool recursive )
	{
		registerDefaultTypes();

		List< string > files;
		findFiles( pFolderName, files, ".tikigeneric" );

		for (uint i = 0u; i < files.getCount(); ++i)
		{
			XmlReader reader;
			if ( !reader.create( files[ i ].cStr() ) )
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] '%s' can't be parsed.\n", files[ i ].cStr() );
				continue;
			}

			const XmlElement* pRoot = reader.getRoot();
			if ( pRoot == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] '%s' has no root node.\n", files[ i ].cStr() );
				continue;
			}

			const XmlElement* pChildNode = pRoot->elements;
			while ( pChildNode != nullptr )
			{
				const XmlAttribute* pNameAtt = reader.findAttributeByName( "name", pChildNode );
				const XmlAttribute* pBaseAtt = reader.findAttributeByName( "name", pChildNode );
				const XmlAttribute* pModeAtt = reader.findAttributeByName( "name", pChildNode );

				if ( pNameAtt != nullptr )
				{
					const GenericDataType* pBaseType = nullptr;
					if ( pBaseAtt != nullptr )
					{
						pBaseType = findTypeByName( pBaseAtt->content );
					}

					GenericDataTypeMode mode = GenericDataTypeMode_ToolAndRuntime;
					if ( pModeAtt != nullptr )
					{
						mode = findModeByName( pModeAtt->content );
						if ( mode == GenericDataTypeMode_Invalid )
						{
							TIKI_TRACE_WARNING( "[GenericDataTypeCollection::create] type named '%s' has a invalid mode(%s).\n", pNameAtt->content, pModeAtt->content );
							mode = GenericDataTypeMode_ToolAndRuntime;
						}
					}

					GenericDataTypeType type = GenericDataTypeType_Invalid;
					if ( isStringEquals( pChildNode->name, "enum" ) )
					{
						type = GenericDataTypeType_Enum;
					}
					else if ( isStringEquals( pChildNode->name, "struct" ) )
					{
						type = GenericDataTypeType_Struct;
					}
					else if ( isStringEquals( pChildNode->name, "union" ) )
					{
						type = GenericDataTypeType_Union;
					}
					else if ( isStringEquals( pChildNode->name, "resource" ) )
					{
						type = GenericDataTypeType_Resource;
					}

					GenericDataType* pType = nullptr;
					switch ( type )
					{
					case GenericDataTypeType_Enum:
						{
							if ( pBaseType == nullptr )
							{
								pBaseType = findTypeByName( "int" );
							}

							pType = TIKI_MEMORY_NEW_OBJECT( GenericDataEnum )( *this, pNameAtt->content, mode, *pBaseType );
						}
						break;

					case GenericDataTypeType_Struct:
						{
							pType = TIKI_MEMORY_NEW_OBJECT( GenericDataStruct )( *this, pNameAtt->content, mode, pBaseType );
						}
						break;

					case GenericDataTypeType_Union:
						{
							pType = TIKI_MEMORY_NEW_OBJECT( GenericDataUnion )( *this, pNameAtt->content, mode );
						}
						break;

					case GenericDataTypeType_Resource:
						{
							//pType = TIKI_MEMORY_NEW_OBJECT( GenericDataResource )( *this, pNameAtt->content, mode );
						}
						break;

					default:
						break;
					}

					if ( pType != nullptr )
					{
						if ( pType->loadFromXml( reader, pChildNode ) )
						{
							addType( *pType );
						}
						else
						{
							TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] unable to load type with name '%s' from xml.\n", pType->getName().cStr() );
							TIKI_MEMORY_DELETE_OBJECT( pType );
						}
					}
				}
				else
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] node has no name attribute and will be ignored.\n" );
				}
				
				pChildNode = pChildNode->next;
			}
		}
	}

	void GenericDataTypeCollection::dispose()
	{
		m_pModeEnum = nullptr;

		List< GenericDataType* > types;
		{
			types.reserve( m_types.getCount() );
			for ( GenericDataType& type : m_types )
			{
				types.add( &type );
			}
			m_types.clear();
		}

		for (uint i = 0u; i < types.getCount(); ++i)
		{
			TIKI_MEMORY_DELETE_OBJECT( types[ i ] );
		}
		types.dispose();
	}

	bool GenericDataTypeCollection::addType( GenericDataType& type )
	{
		if ( findTypeByName( type.getName() ) == nullptr )
		{
			m_types.push( type );

			return true;
		}
		else
		{
			TIKI_TRACE_ERROR( "[GenericDataTypeCollection::addType] dublicate type name: '%s'\n", type.getName().cStr() );
			return false;
		}		
	}

	const GenericDataType* GenericDataTypeCollection::findTypeByName( const string& name ) const
	{
		for (const GenericDataType& type : m_types)
		{
			if ( type.getName() == name )
			{
				return &type;
			}
		}

		return nullptr;

	}

	GenericDataTypeMode GenericDataTypeCollection::findModeByName( const string& name ) const
	{
		const sint64* pValue = m_pModeEnum->getValueByName( name );
		if ( pValue != nullptr )
		{
			return (GenericDataTypeMode)*pValue;
		}

		return GenericDataTypeMode_Invalid;
	}

	bool GenericDataTypeCollection::exportCode( GenericDataTypeMode mode )
	{
		return false;
	}

	void GenericDataTypeCollection::registerDefaultTypes()
	{
		GenericDataValueType* pShort	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "short", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger16 );
		GenericDataValueType* pInt		= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "int", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger32 );
		GenericDataValueType* pLong		= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "long", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger64 );

		GenericDataValueType* pSInt8	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "sint8",  GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger8 );
		GenericDataValueType* pSInt16	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "sint16", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger16 );
		GenericDataValueType* pSInt32	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "sint32", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger32 );
		GenericDataValueType* pSInt64	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "sint64", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger64 );
		GenericDataValueType* pUInt8	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "uint8",  GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger8 );
		GenericDataValueType* pUInt16	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "uint16", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger16 );
		GenericDataValueType* pUInt32	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "uint32", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger32 );
		GenericDataValueType* pUInt64	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "uint64", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger64 );
		GenericDataValueType* pFloat16	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "half",   GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_FloatingPoint16 );
		GenericDataValueType* pFloat32	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "float",  GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_FloatingPoint32 );
		GenericDataValueType* pFloat64	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "double", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_FloatingPoint64 );

		GenericDataValueType* pCrc32	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "crc32", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_UnsingedInteger32 );
		GenericDataValueType* pTimeMS	= TIKI_MEMORY_NEW_OBJECT( GenericDataValueType )( *this, "timems", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger64 );

		addType( *pShort );
		addType( *pInt );
		addType( *pLong );

		addType( *pSInt8 );
		addType( *pSInt16 );
		addType( *pSInt32 );
		addType( *pSInt64 );
		addType( *pUInt8 );
		addType( *pUInt16 );
		addType( *pUInt32 );
		addType( *pUInt64 );
		addType( *pFloat16 );
		addType( *pFloat32 );
		addType( *pFloat64 );

		addType( *pCrc32 );
		addType( *pTimeMS );

		GenericDataEnum* pGenericDataTypeMode = TIKI_MEMORY_NEW_OBJECT( GenericDataEnum )( *this, "GenericDataTypeMode", GenericDataTypeMode_ToolOnly, *pUInt8 );
		pGenericDataTypeMode->addValue( "Invalid",			0, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "RuntimeOnly",		1, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "ToolOnly",			2, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "ToolAndRuntime",	3, GenericDataTypeMode_ToolOnly );
		m_pModeEnum = pGenericDataTypeMode;
	}

	void GenericDataTypeCollection::findFiles( const string& path, List< string >& files, const string& ext ) const
	{
		List< string > dirFiles;
		directory::getFiles( path, dirFiles );
		for (size_t i = 0u; i < dirFiles.getCount(); ++i)
		{
			if ( path::getExtension( dirFiles[ i ] ) != ext )
			{
				continue;
			}

			files.add( path::combine( path, dirFiles[ i ] ) );
		}		

		List< string > dirDirectories;
		directory::getDirectories( path, dirDirectories );
		for (size_t i = 0u; i < dirDirectories.getCount(); ++i)
		{
			findFiles( path::combine( path, dirDirectories[ i ] ), files, ext );
		}		
	}
}
