
#include "tiki/genericdata/genericdatatypecollection.hpp"

#include "tiki/base/directory.hpp"
#include "tiki/base/file.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/path.hpp"
#include "tiki/genericdata/genericdatatypearray.hpp"
#include "tiki/genericdata/genericdatatypeenum.hpp"
#include "tiki/genericdata/genericdatatyperesource.hpp"
#include "tiki/genericdata/genericdatatypestruct.hpp"
#include "tiki/genericdata/genericdatatypeunion.hpp"
#include "tiki/genericdata/genericdatatypevaluetype.hpp"
#include "tiki/io/xmlreader.hpp"
#include "tiki/toolbase/directory_tool.hpp"
#include "tiki/toolbase/list.hpp"
#include "tiki/toolbase/map.hpp"

namespace tiki
{
	GenericDataTypeCollection::GenericDataTypeCollection()
	{
		m_pModeEnum = nullptr;
	}

	GenericDataTypeCollection::~GenericDataTypeCollection()
	{
	}

	void GenericDataTypeCollection::create( const string& contentFolder, bool recursive )
	{
		registerDefaultTypes();

		List< string > files;
		findFiles( contentFolder, files, ".tikigeneric" );

		for (uint i = 0u; i < files.getCount(); ++i)
		{
			const string& fileName = files[ i ];
			const string moduleName = path::getFilenameWithoutExtension( fileName );

			XmlReader reader;
			if ( !reader.create( fileName.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] '%s' can't be parsed.\n", files[ i ].cStr() );
				continue;
			}

			const XmlElement* pRoot = reader.findNodeByName( "tikigenerictypes" );
			if ( pRoot == nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] '%s' has no root node.\n", files[ i ].cStr() );
				continue;
			}

			const XmlElement* pChildNode = pRoot->elements;
			while ( pChildNode != nullptr )
			{
				const XmlAttribute* pNameAtt = reader.findAttributeByName( "name", pChildNode );
				const XmlAttribute* pBaseAtt = reader.findAttributeByName( "base", pChildNode );
				const XmlAttribute* pModeAtt = reader.findAttributeByName( "mode", pChildNode );

				if ( pNameAtt != nullptr )
				{
					const GenericDataType* pBaseType = nullptr;
					if ( pBaseAtt != nullptr )
					{
						pBaseType = parseType( pBaseAtt->content );
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

							pType = TIKI_MEMORY_NEW_OBJECT( GenericDataTypeEnum )( *this, pNameAtt->content, mode, *pBaseType );
						}
						break;

					case GenericDataTypeType_Struct:
						{
							pType = TIKI_MEMORY_NEW_OBJECT( GenericDataTypeStruct )( *this, pNameAtt->content, mode, pBaseType );
						}
						break;

					case GenericDataTypeType_Union:
						{
							pType = TIKI_MEMORY_NEW_OBJECT( GenericDataTypeUnion )( *this, pNameAtt->content, mode );
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
							pType->setModule( moduleName );
							addType( *pType );
						}
						else
						{
							TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] unable to load type with name '%s' from xml.\n", pType->getName().cStr() );
							TIKI_MEMORY_DELETE_OBJECT( pType );
						}
					}
				}
				else if ( pChildNode->name != nullptr )
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] node has no name attribute and will be ignored.\n" );
				}
				
				pChildNode = pChildNode->next;
			}

			reader.dispose();
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

	const GenericDataTypeArray* GenericDataTypeCollection::makeArrayType( const GenericDataType* pType )
	{
		GenericDataTypeArray* pArrayType = nullptr;
		if ( !m_arrays.findValue( const_cast< const GenericDataTypeArray** >( &pArrayType ), pType ) )
		{
			string name = pType->getName();
			name += "[]";

			pArrayType = TIKI_MEMORY_NEW_OBJECT( GenericDataTypeArray )( *this, name, pType, GenericDataTypeMode_ToolAndRuntime );
			m_arrays.set( pType, pArrayType );

			addType( *pArrayType );
		}

		return pArrayType;
	}

	const GenericDataType* GenericDataTypeCollection::parseType( const string& typeString )
	{
		List< ModifierDescription > modifiers;

		ModifierDescription desc;
		string content = typeString;
		while ( parseToken( desc, content ) )
		{
			modifiers.add( desc );

			content = desc.content;
		}

		if ( modifiers.isEmpty() )
		{
			return findTypeByName( typeString );
		}

		const GenericDataType* pType = nullptr;
		for (uint i = modifiers.getCount() - 1u; i < modifiers.getCount(); --i)
		{
			const ModifierDescription& modifier = modifiers[ i ];

			if ( modifier.modifier == "array" )
			{
				if ( pType == nullptr )
				{
					pType = findTypeByName( modifier.content );

					if ( pType == nullptr )
					{
						TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseType] Unable to find Type with name '%s'.\n", modifier.content.cStr() );
						return nullptr;
					}
				}

				pType = makeArrayType( pType );
			}
			else
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseType] Modifier(%s) not supported.\n", modifier.modifier.cStr() );
				return nullptr;
			}
		}

		return pType;
	}

	bool GenericDataTypeCollection::parseValue( GenericDataValue& outValue, const string& valueString )
	{
		return false;
	}

	bool GenericDataTypeCollection::exportCode( GenericDataTypeMode mode, const string& targetDir )
	{
		if ( !directory::exists( targetDir ) )
		{
			if ( !directory::create( targetDir ) )
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::exportCode] unable to create target directory(%s).\n", targetDir.cStr() );
				return false;
			}
		}

		Map<string, string> moduleCode;
		for (const GenericDataType& type : m_types)
		{
			const string& moduleName = type.getModule();
			if ( moduleName.isEmpty() )
			{
				continue;
			}
			
			string& code = moduleCode[ moduleName ];

			if ( type.getMode() & mode )
			{
				type.exportCode( code, mode );
			}
		}

		static const char* s_pBaseFormat =	"#pragma once\n"
											"#ifndef TIKI_%s_INCLUDED__\n"
											"#define TIKI_%s_INCLUDED__\n"
											"\n"
											"namespace tiki\n"
											"{\n"
											"%s\n"
											"}\n"
											"\n"
											"#endif // TIKI_%s_INCLUDED__\n";

		for (uint i = 0u; i < moduleCode.getCount(); ++i)
		{
			const auto& kvp = moduleCode.getPairAt( i );

			const string fileName		= kvp.key + ".hpp";
			const string fileNameDefine	= fileName.toUpper().replace('.', '_');
			const string fullPath		= path::combine( targetDir, fileName );

			string finalCode = formatString(
				s_pBaseFormat,
				fileNameDefine.cStr(),
				fileNameDefine.cStr(),
				kvp.value.cStr(),
				fileNameDefine.cStr()
			);

			file::writeAllBytes( fullPath.cStr(), (const uint8*)finalCode.cStr(), finalCode.getLength() );
		}

		return true;
	}

	void GenericDataTypeCollection::registerDefaultTypes()
	{
		GenericDataTypeValueType* pShort	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "short",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger16 );
		GenericDataTypeValueType* pInt		= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "int",		GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger32 );
		GenericDataTypeValueType* pLong		= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "long",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger64 );

		GenericDataTypeValueType* pSInt8	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "sint8",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger8 );
		GenericDataTypeValueType* pSInt16	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "sint16",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger16 );
		GenericDataTypeValueType* pSInt32	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "sint32",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger32 );
		GenericDataTypeValueType* pSInt64	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "sint64",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger64 );
		GenericDataTypeValueType* pUInt8	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "uint8",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger8 );
		GenericDataTypeValueType* pUInt16	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "uint16",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger16 );
		GenericDataTypeValueType* pUInt32	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "uint32",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger32 );
		GenericDataTypeValueType* pUInt64	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "uint64",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger64 );

		GenericDataTypeValueType* pHalf		= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "half",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_FloatingPoint16 );		
		GenericDataTypeValueType* pFloat	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "float",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_FloatingPoint32 );
		GenericDataTypeValueType* pDouble	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "double",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_FloatingPoint64 );
		GenericDataTypeValueType* pFloat16	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "float16",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_FloatingPoint16 );		
		GenericDataTypeValueType* pFloat32	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "float32",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_FloatingPoint32 );
		GenericDataTypeValueType* pFloat64	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "float64",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_FloatingPoint64 );

		GenericDataTypeValueType* pCrc32	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "crc32",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_UnsingedInteger32 );
		GenericDataTypeValueType* pTimeMS	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "timems",	GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_SingedInteger64 );

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

		addType( *pHalf );
		addType( *pFloat );
		addType( *pDouble );
		addType( *pFloat16 );
		addType( *pFloat32 );
		addType( *pFloat64 );

		addType( *pCrc32 );
		addType( *pTimeMS );

		GenericDataTypeValueType* pBoolean	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "bool", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_Boolean );
		GenericDataTypeValueType* pString	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "string", GenericDataTypeMode_ToolAndRuntime, GenericDataValueTypeType_String );

		addType( *pBoolean );
		addType( *pString );

		GenericDataTypeEnum* pGenericDataTypeMode = TIKI_MEMORY_NEW_OBJECT( GenericDataTypeEnum )( *this, "GenericDataTypeMode", GenericDataTypeMode_ToolOnly, *pUInt8 );
		pGenericDataTypeMode->addValue( "Invalid",			0, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "RuntimeOnly",		1, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "ToolOnly",			2, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "ToolAndRuntime",	3, GenericDataTypeMode_ToolOnly );
		m_pModeEnum = pGenericDataTypeMode;

		addType( *pGenericDataTypeMode );
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

	bool GenericDataTypeCollection::parseToken( ModifierDescription& outModifier, const string& text )
	{
		if ( text.isEmpty() || text[ 0u ] != '{' || text[ text.getLength() - 1 ] != '}' )
		{
			return false;
		}

		const int contentBeginIndex = text.indexOf( ' ' );
		if ( contentBeginIndex < 1 )
		{
			return false;
		}

		outModifier.modifier	= text.subString( 1u, contentBeginIndex - 1u );
		outModifier.content		= text.subString( contentBeginIndex + 1, text.getLength() - contentBeginIndex - 2 );

		return true;
	}
}
