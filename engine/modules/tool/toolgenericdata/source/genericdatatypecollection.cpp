
#include "tiki/toolgenericdata/genericdatatypecollection.hpp"

#include "tiki/base/fourcc.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/stringconvert.hpp"
#include "tiki/base/stringparse.hpp"
#include "tiki/container/list.hpp"
#include "tiki/container/map.hpp"
#include "tiki/io/directory.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/path.hpp"
#include "tiki/io/xmlreader.hpp"
#include "tiki/toolbase/directory_tool.hpp"
#include "tiki/toolgenericdata/genericdatatypearray.hpp"
#include "tiki/toolgenericdata/genericdatatypeenum.hpp"
#include "tiki/toolgenericdata/genericdatatypepointer.hpp"
#include "tiki/toolgenericdata/genericdatatypereference.hpp"
#include "tiki/toolgenericdata/genericdatatyperesource.hpp"
#include "tiki/toolgenericdata/genericdatatypestruct.hpp"
#include "tiki/toolgenericdata/genericdatatypevaluetype.hpp"
#include "tiki/toolgenericdata/genericdatavalue.hpp"

namespace tiki
{
	struct GenericDataModuleData
	{
		string			name;
		List< string >	dependencies;
	};

	struct GenericDataModuleLoadingData
	{
		string					fileName;
		XmlReader				reader;
		const XmlElement*		pRootNode;
		bool					isLoaded;
		GenericDataModuleData	data;
	};

	GenericDataTypeCollection::GenericDataTypeCollection()
	{
		m_pModeEnum = nullptr;
	}

	GenericDataTypeCollection::~GenericDataTypeCollection()
	{
	}

	bool GenericDataTypeCollection::create( const string& contentFolder, bool recursive )
	{
		if ( !registerDefaultValueTypes() )
		{
			dispose();
			return false;
		}

		if ( !registerDefaultResourceTypes() )
		{
			dispose();
			return false;
		}

		List< string > files;
		findFiles( contentFolder, files, ".tikigenerictypes" );

		Array< GenericDataModuleLoadingData > modules;
		if ( !modules.create( files.getCount() ) )
		{
			dispose();
			return false;
		}

		bool ok = true;
		for (uint i = 0u; i < files.getCount(); ++i)
		{
			GenericDataModuleLoadingData& data = modules[ i ];
			data.fileName		= files[ i ];
			data.pRootNode		= nullptr;
			data.isLoaded		= false;
			data.data.name		= path::getFilenameWithoutExtension( data.fileName );

			if ( data.reader.create( data.fileName.cStr() ) )
			{
				data.pRootNode = data.reader.findNodeByName( "tikigenerictypes" );
				if ( data.pRootNode == nullptr )
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] '%s' has no root node.\n", data.fileName.cStr() );
					ok = false;
					continue;
				}

				const XmlAttribute* pRootBaseAtt = data.reader.findAttributeByName( "base", data.pRootNode );
				if ( pRootBaseAtt != nullptr )
				{
					data.data.dependencies.add( pRootBaseAtt->content );
				}
			}
			else
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] '%s' can't be parsed.\n", data.fileName.cStr() );
				ok = false;
			}
		}

		uint loadedModules = 0u;
		while ( loadedModules < modules.getCount() )
		{
			for (uint moduleIndex = 0u; moduleIndex < modules.getCount(); ++moduleIndex )
			{
				GenericDataModuleLoadingData& data = modules[ moduleIndex ];

				if ( data.pRootNode == nullptr )
				{
					continue;
				}

				bool hasAllDependencies = true;
				for (uint depIndex = 0u; depIndex < data.data.dependencies.getCount(); ++depIndex )
				{
					for (uint depModIndex = 0u; depModIndex < modules.getCount(); ++depModIndex )
					{
						GenericDataModuleLoadingData& depModData = modules[ depModIndex ];
						if ( depModData.data.name == data.data.dependencies[ depIndex ] )
						{
							hasAllDependencies &= depModData.isLoaded;
						}
					}
				}

				if ( !hasAllDependencies )
				{
					continue;
				}
					
				if ( !parseFile( data.reader, data.pRootNode, data.fileName, data.data.name ) )
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] '%s' can't be loaded.\n", data.fileName.cStr() );
					ok = false;
				}

				m_modules[ data.data.name ] = data.data;
				data.pRootNode = nullptr;
				data.isLoaded = true;
				data.reader.dispose();
				
				loadedModules++;
			}
		}
		
		modules.dispose();

		if ( !ok )
		{
			dispose();
		}

		return ok;
	}

	void GenericDataTypeCollection::dispose()
	{
		m_pModeEnum = nullptr;

		while( !m_types.isEmpty() )
		{
			GenericDataType& type = m_types.getFirst();

			m_types.removeSortedByValue( type );
			TIKI_MEMORY_DELETE_OBJECT( &type );
		}
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

	bool GenericDataTypeCollection::removeType( GenericDataType& type )
	{
		if ( findTypeByName( type.getName() ) != nullptr )
		{
			m_types.removeSortedByValue( type );

			return true;
		}
		else
		{
			TIKI_TRACE_ERROR( "[GenericDataTypeCollection::removeType] can't find type with name: '%s'\n", type.getName().cStr() );
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

	void GenericDataTypeCollection::findTypesByType( List< const GenericDataType* >& types, GenericDataTypeType typeType ) const
	{
		for (const GenericDataType& type : m_types)
		{
			if ( type.getType() == typeType )
			{
				types.add( &type );
			}
		}
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

	const GenericDataTypeArray* GenericDataTypeCollection::makeArrayType( const GenericDataType* pBaseType )
	{
		GenericDataTypeArray* pArrayType = nullptr;
		if ( !m_arrays.findValue( const_cast< const GenericDataTypeArray** >( &pArrayType ), pBaseType ) )
		{
			const string name = "ResArray<" + pBaseType->getName() + ">";
			pArrayType = TIKI_MEMORY_NEW_OBJECT( GenericDataTypeArray )( *this, name, pBaseType, GenericDataTypeMode_ToolAndRuntime );
			m_arrays.set( pBaseType, pArrayType );

			TIKI_VERIFY( addType( *pArrayType ) );
		}

		return pArrayType;
	}

	const GenericDataTypeReference* GenericDataTypeCollection::makeReferenceType( const GenericDataTypeResource* pBaseType )
	{
		GenericDataTypeReference* pReferenceType = nullptr;
		if ( !m_references.findValue( const_cast< const GenericDataTypeReference** >( &pReferenceType ), pBaseType ) )
		{
			const string name = "ResRef<" + pBaseType->getName() + ">";
			pReferenceType = TIKI_MEMORY_NEW_OBJECT( GenericDataTypeReference )( *this, name, GenericDataTypeMode_ToolAndRuntime, pBaseType );
			m_references.set( pBaseType, pReferenceType );

			TIKI_VERIFY( addType( *pReferenceType ) );
		}

		return pReferenceType;
	}

	const GenericDataTypePointer* GenericDataTypeCollection::makePointerType( const GenericDataTypeStruct* pBaseType )
	{
		GenericDataTypePointer* pPointerType = nullptr;
		if( !m_pointers.findValue( const_cast<const GenericDataTypePointer**>(&pPointerType), pBaseType ) )
		{
			const string name = pBaseType->getName() + "*";
			pPointerType = TIKI_MEMORY_NEW_OBJECT( GenericDataTypePointer )(*this, name, GenericDataTypeMode_ToolAndRuntime, pBaseType);
			m_pointers.set( pBaseType, pPointerType );

			TIKI_VERIFY( addType( *pPointerType ) );
		}

		return pPointerType;
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
			else if ( modifier.modifier == "reference" )
			{
				const GenericDataTypeResource* pTypedType = nullptr;
				if ( pType == nullptr )
				{
					pType = findTypeByName( modifier.content );
					if ( pType == nullptr )
					{
						TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseType] Unable to find Type with name '%s'.\n", modifier.content.cStr() );
						return nullptr;
					}

					if ( pType->getType() != GenericDataTypeType_Resource )
					{
						TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseType] Reference needs a Resource type as base. '%s' is not a resource.\n", modifier.content.cStr() );
						return nullptr;
					}

					pTypedType = (const GenericDataTypeResource*)pType;
				}

				pType = makeReferenceType( pTypedType );
			}
			else if( modifier.modifier == "pointer" )
			{
				const GenericDataTypeStruct* pTypedType = nullptr;
				if( pType == nullptr )
				{
					pType = findTypeByName( modifier.content );
					if( pType == nullptr )
					{
						TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseType] Unable to find Type with name '%s'.\n", modifier.content.cStr() );
						return nullptr;
					}

					if( pType->getType() != GenericDataTypeType_Struct )
					{
						TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseType] Pointer needs a Struct type as base. '%s' is not a resource.\n", modifier.content.cStr() );
						return nullptr;
					}

					pTypedType = (const GenericDataTypeStruct*)pType;
				}

				pType = makePointerType( pTypedType );
			}
			else
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseType] Modifier(%s) not supported.\n", modifier.modifier.cStr() );
				return nullptr;
			}
		}

		return pType;
	}

	bool GenericDataTypeCollection::parseValue( GenericDataValue& outValue, const string& valueString, const GenericDataType* pType, const GenericDataType* pParentType )
	{
		if ( pType == nullptr )
		{
			return false;
		}

		List< ModifierDescription > modifiers;

		ModifierDescription desc;
		string content = valueString;
		while ( parseToken( desc, content ) )
		{
			modifiers.add( desc );

			content = desc.content;
		}
		
		string enumValueName;
		if ( modifiers.isEmpty() )
		{
			content = valueString;			
		}
		else
		{
			for (uint i = modifiers.getCount() - 1u; i < modifiers.getCount(); --i)
			{
				const ModifierDescription& modifier = modifiers[ i ];

				if ( i == 0 )
				{
					content = modifier.content;
				}

				if ( modifier.modifier == "enum" )
				{
					const int dotIndex = content.indexOf( '.' );
					if ( dotIndex == -1 )
					{
						TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseValue] Please {enum TypeName.ValueName} for enum modfiers.\n" );
						return false;
					}

					const string enumTypeName = content.subString( 0u, dotIndex );
					const GenericDataType* pEnumType = findTypeByName( enumTypeName );
					if ( pEnumType == nullptr || pEnumType->getType() != GenericDataTypeType_Enum )
					{
						TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseValue] '%s' not found or not an enum.\n", enumTypeName.cStr() );
						return false;
					}

					enumValueName = content.subString( dotIndex + 1 );

					const GenericDataTypeEnum* pTypedEnumType = (const GenericDataTypeEnum*)pEnumType;
					const sint64* pValue = pTypedEnumType->getValueByName( enumValueName );
					if ( pValue == nullptr )
					{
						TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseValue] enum value with name '%s' not found.\n", enumValueName.cStr() );
						return false;
					}

					content = StringConvert::ToString( *pValue );
				}
				else if ( modifier.modifier == "reference" )
				{

				}
				else if ( modifier.modifier == "bit" )
				{
					const sint64 shift = ParseString::parseInt64( content.cStr() );
					const sint64 value = 1ll << shift;

					content = StringConvert::ToString( value );
				}
				else if ( modifier.modifier == "offset" )
				{
					if ( pParentType != nullptr && pParentType->getType() == GenericDataTypeType_Struct )
					{
						const GenericDataTypeStruct* pTypedParent = (const GenericDataTypeStruct*)pParentType;
						const List< GenericDataStructField >& fields = pTypedParent->getFields();

						bool found = false;
						sint64 offset = 0;
						for (uint i = 0u; i < fields.getCount(); ++i)
						{
							const GenericDataStructField& field = fields[ i ];

							offset = alignValue( offset, (sint64)field.pType->getAlignment() );

							if ( field.name == content )
							{
								content = StringConvert::ToString( offset );
								found = true;
								break;
							}

							offset += field.pType->getSize();							
						}

						if ( !found )
						{
							TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseValue] field with name '%s' for modifier '%s' not found.\n", content.cStr(), modifier.modifier.cStr() );
							return false;
						}
					}
					else
					{
						TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseValue] modifier '%s' must have a struct type. '%s' is not a struct.\n", modifier.modifier.cStr(), pParentType->getName().cStr() );
						return false;
					}
				}
				else
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseValue] modifier '%s' not supported.\n", modifier.modifier.cStr() );
					return false;
				}
			}
		}

		if ( pType->getType() == GenericDataTypeType_ValueType )
		{
			const GenericDataTypeValueType* pTypedType = (const GenericDataTypeValueType*)pType;

			if ( pTypedType->isBoolean() )
			{
				const bool value = ParseString::parseInt64( content.cStr() ) != 0;
				return outValue.setBoolean( value, pType );
			}
			else if ( pTypedType->isSignedInteger() )
			{
				const sint64 value = ParseString::parseInt64( content.cStr() );
				return outValue.setSignedValue( value, pType );
			}
			else if ( pTypedType->isUnsignedInteger() )
			{
				const uint64 value = ParseString::parseUInt64( content.cStr() );
				return outValue.setUnsignedValue( value, pType );
			}
			else if ( pTypedType->isFloatingPoint() )
			{
				const double value = ParseString::parseDouble( content.cStr() );
				return outValue.setFloatingPoint( value, pType );
			}
			else if ( pTypedType->isString() )
			{
				return outValue.setString( content, pType );
			}
		}
		else if ( pType->getType() == GenericDataTypeType_Enum )
		{
			return outValue.setEnum( enumValueName, pType );
		}
		else if ( pType->getType() == GenericDataTypeType_Reference )
		{
			return outValue.setReference( content, pType );
		}
		else if( pType->getType() == GenericDataTypeType_Pointer )
		{
			TIKI_NOT_IMPLEMENTED;
			//return outValue.setReference( content, pType );
		}

		return false;
	}

	bool GenericDataTypeCollection::exportCode( GenericDataTypeMode mode, const string& targetDir )
	{
		if ( !directory::exists( targetDir.cStr() ) )
		{
			if ( !directory::create( targetDir.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::exportCode] unable to create target directory(%s).\n", targetDir.cStr() );
				return false;
			}
		}

		static const char* s_pFactoriesIncludeFormat = "#include \"%s\"\n";
		static const char* s_pFactoriesCreateFormat = "\t\t%sGenericDataResource::registerResourceType( resourceManager );\n";
		static const char* s_pFactoriesDisposeFormat = "\t\t%sGenericDataResource::unregisterResourceType( resourceManager );\n";

		string factoriesIncludeCode = "";
		string factoriesCreateCode = "";
		string factoriesDisposeCode = "";
		Map<string, GenericDataExportData> moduleCode;
		for (const GenericDataType& type : m_types)
		{
			const string& moduleName = type.getModule();
			if ( moduleName.isEmpty() )
			{
				continue;
			}
			
			GenericDataExportData& moduleData = moduleCode[ moduleName ];

			if ( isBitSet( type.getMode(), mode ) )
			{
				type.exportCode( moduleData, mode );
			}

			if ( type.getType() == GenericDataTypeType_Resource )
			{
				factoriesCreateCode += formatString( s_pFactoriesCreateFormat, type.getName().cStr() );
				factoriesDisposeCode += formatString( s_pFactoriesDisposeFormat, type.getName().cStr() );
			}
		}

		static const char* s_pBaseFormat =	"#pragma once\n"
											"#ifndef TIKI_%s_INCLUDED__\n"
											"#define TIKI_%s_INCLUDED__\n"
											"\n"
											"#include \"tiki/base/types.hpp\"\n"
											"%s"
											"%s"
											"%s"
											"%s"
											"\n"
											"%s"
											"namespace tiki\n"
											"{\n"
											"\t#pragma warning( push )\n"
											"\t#pragma warning( disable: 4309 4369 4340 )\n"
											"\t\n"
											"%s"
											"%s"
											"\t\n"
											"\t#pragma warning( pop )\n"
											"}\n"
											"\n"
											"#endif // TIKI_%s_INCLUDED__\n";

		static const char* s_pReference			= "\tclass %s;\n";
		static const char* s_pStringInclude		= "#include \"tiki/base/basicstring.hpp\"\n";
		static const char* s_pArrayInclude		= "#include \"tiki/container/staticarray.hpp\"\n";
		static const char* s_pResourceInclude	= "#include \"tiki/genericdata/genericdataresource.hpp\"\n";
		static const char* s_pReferenceInclude	= "#include \"tiki/resource/resourcefile.hpp\"\n";
		static const char* s_pDependencyInclude	= "#include \"%s.hpp\"\n";

		for (uint moduleIndex = 0u; moduleIndex < moduleCode.getCount(); ++moduleIndex)
		{
			const auto& kvp = moduleCode.getPairAt( moduleIndex );
			const auto& moduleData = m_modules[ kvp.key ];

			const string fileName		= kvp.key + ".hpp";
			const string fileNameDefine	= fileName.toUpper().replace('.', '_');
			const string fullPath		= path::combine( targetDir, fileName );

			if ( kvp.value.containsResource )
			{
				factoriesIncludeCode += formatString( s_pFactoriesIncludeFormat, fileName.cStr() );
			}

			string referencesCode = "";
			for (uint refIndex = 0u; refIndex < kvp.value.references.getCount(); ++refIndex)
			{
				referencesCode += formatString( s_pReference, kvp.value.references[ refIndex ]->getBaseType()->getName().cStr() );
			}

			string dependenciesIncludeCode = "";
			for (uint depIndex = 0u; depIndex < moduleData.dependencies.getCount(); ++depIndex)
			{
				dependenciesIncludeCode += formatString( s_pDependencyInclude, moduleData.dependencies[ depIndex ].cStr() );
			}
			if ( !moduleData.dependencies.isEmpty() )
			{
				dependenciesIncludeCode += "\n";
			}			

			string finalCode = formatString(
				s_pBaseFormat,
				fileNameDefine.cStr(),
				fileNameDefine.cStr(),
				(kvp.value.containsString ? s_pStringInclude : ""),
				(kvp.value.containsArray ? s_pArrayInclude : ""),
				(kvp.value.containsResource ? s_pResourceInclude : ""),
				(!kvp.value.references.isEmpty() ? s_pReferenceInclude : ""),
				dependenciesIncludeCode.cStr(),
				referencesCode.cStr(),
				kvp.value.code.cStr(),
				fileNameDefine.cStr()
			);

			writeToFileIfNotEquals( fullPath, finalCode );
		}

		static const char* s_pFactoriesHeaderFormat =	"#pragma once\n"
														"#ifndef TIKI_GENERICDATAFACTORIES_INCLUDED__\n"
														"#define TIKI_GENERICDATAFACTORIES_INCLUDED__\n"
														"\n"
														"#include \"tiki/base/types.hpp\"\n"
														"\n"
														"namespace tiki\n"
														"{\n"
														"\tclass ResourceManager;\n"
														"\t\n"
														"\tclass GenericDataFactories\n"
														"\t{\n"
														"\t\tTIKI_NONCOPYABLE_CLASS( GenericDataFactories );\n"
														"\t\t\n"
														"\tpublic:\n"
														"\t\t\n"
														"\t\tvoid\tcreate( ResourceManager& resourceManager );\n"
														"\t\tvoid\tdispose( ResourceManager& resourceManager );\n"
														"\t\t\n"
														"\t};\n"
														"}\n"
														"\n"
														"#endif // TIKI_GENERICDATAFACTORIES_INCLUDED__\n";

		static const char* s_pFactoriesSourceFormat =	"\n"
														"#include \"genericdatafactories.hpp\"\n"
														"\n"
														"%s"
														"\n"
														"namespace tiki\n"
														"{\n"
														"\tvoid GenericDataFactories::create( ResourceManager& resourceManager )\n"
														"\t{\n"
														"%s"
														"\t}\n"
														"\t\n"
														"\tvoid GenericDataFactories::dispose( ResourceManager& resourceManager )\n"
														"\t{\n"
														"%s"
														"\t}\n"
														"};\n"
														"\n";

		const string headerFileName		= "genericdatafactories.hpp";
		const string sourceFileName		= "genericdatafactories.cpp";

		const string headerFullPath		= path::combine( targetDir, headerFileName );
		const string sourceFullPath		= path::combine( targetDir, sourceFileName );

		const string headerFinalCode	= formatString( s_pFactoriesHeaderFormat );
		const string sourceFinalCode	= formatString( s_pFactoriesSourceFormat, factoriesIncludeCode.cStr(), factoriesCreateCode.cStr(), factoriesDisposeCode.cStr() );

		writeToFileIfNotEquals( headerFullPath, headerFinalCode );
		writeToFileIfNotEquals( sourceFullPath, sourceFinalCode );

		return true;
	}

	bool GenericDataTypeCollection::registerDefaultValueTypes()
	{
		bool ok = true;

		GenericDataTypeValueType* pShort	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "short",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger16 );
		GenericDataTypeValueType* pInt		= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "int",		GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger32 );
		GenericDataTypeValueType* pLong		= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "long",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger64 );

		GenericDataTypeValueType* pSInt8	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "sint8",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger8 );
		GenericDataTypeValueType* pSInt16	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "sint16",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger16 );
		GenericDataTypeValueType* pSInt32	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "sint32",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger32 );
		GenericDataTypeValueType* pSInt64	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "sint64",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger64 );

		GenericDataTypeValueType* pUInt8	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "uint8",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_UnsingedInteger8 );
		GenericDataTypeValueType* pUInt16	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "uint16",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_UnsingedInteger16 );
		GenericDataTypeValueType* pUInt32	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "uint32",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_UnsingedInteger32 );
		GenericDataTypeValueType* pUInt64	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "uint64",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_UnsingedInteger64 );

		GenericDataTypeValueType* pHalf		= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "half",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_FloatingPoint16 );		
		GenericDataTypeValueType* pFloat	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "float",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_FloatingPoint32 );
		GenericDataTypeValueType* pDouble	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "double",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_FloatingPoint64 );

		GenericDataTypeValueType* pFloat16	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "float16",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_FloatingPoint16 );		
		GenericDataTypeValueType* pFloat32	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "float32",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_FloatingPoint32 );
		GenericDataTypeValueType* pFloat64	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "float64",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_FloatingPoint64 );

		GenericDataTypeValueType* pCrc32	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "crc32",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_UnsingedInteger32 );
		GenericDataTypeValueType* pTimeMS	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "timems",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger64 );

		ok &= addType( *pShort );
		ok &= addType( *pInt );
		ok &= addType( *pLong );

		ok &= addType( *pSInt8 );
		ok &= addType( *pSInt16 );
		ok &= addType( *pSInt32 );
		ok &= addType( *pSInt64 );
		ok &= addType( *pUInt8 );
		ok &= addType( *pUInt16 );
		ok &= addType( *pUInt32 );
		ok &= addType( *pUInt64 );

		ok &= addType( *pHalf );
		ok &= addType( *pFloat );
		ok &= addType( *pDouble );
		ok &= addType( *pFloat16 );
		ok &= addType( *pFloat32 );
		ok &= addType( *pFloat64 );

		ok &= addType( *pCrc32 );
		ok &= addType( *pTimeMS );

		GenericDataTypeValueType* pBoolean	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "bool", GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_Boolean );
		GenericDataTypeValueType* pString	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeValueType )( *this, "string", GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_String );

		ok &= addType( *pBoolean );
		ok &= addType( *pString );

		GenericDataTypeEnum* pGenericDataTypeMode = TIKI_MEMORY_NEW_OBJECT( GenericDataTypeEnum )( *this, "GenericDataTypeMode", GenericDataTypeMode_ToolOnly, pUInt8 );
		pGenericDataTypeMode->addValue( "Invalid",			0, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "RuntimeOnly",		1, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "ToolOnly",			2, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "ToolAndRuntime",	3, GenericDataTypeMode_ToolOnly );
		m_pModeEnum = pGenericDataTypeMode;

		ok &= addType( *pGenericDataTypeMode );

		return ok;
	}

	bool GenericDataTypeCollection::registerDefaultResourceTypes()
	{
		bool ok = true;

		GenericDataTypeResource* pAnimation	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeResource )( *this, "Animation",	GenericDataTypeMode_ToolAndRuntime, "animation",	TIKI_FOURCC( 'A', 'N', 'I', 'M' ) );
		GenericDataTypeResource* pFont		= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeResource )( *this, "Font",			GenericDataTypeMode_ToolAndRuntime, "font",			TIKI_FOURCC( 'F', 'O', 'N', 'T' ) );
		GenericDataTypeResource* pModel		= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeResource )( *this, "Model",		GenericDataTypeMode_ToolAndRuntime, "model",		TIKI_FOURCC( 'M', 'O', 'D', 'L' ) );
		GenericDataTypeResource* pShader	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeResource )( *this, "ShaderSet",	GenericDataTypeMode_ToolAndRuntime, "shader",		TIKI_FOURCC( 'T', 'G', 'S', 'S' ) );
		GenericDataTypeResource* pTexture	= TIKI_MEMORY_NEW_OBJECT( GenericDataTypeResource )( *this, "Texture",		GenericDataTypeMode_ToolAndRuntime, "texture",		TIKI_FOURCC( 'T', 'E', 'X', 'R' ) );

		ok &= addType( *pAnimation );
		ok &= addType( *pFont );
		ok &= addType( *pModel );
		ok &= addType( *pShader );
		ok &= addType( *pTexture );

		return ok;
	}

	void GenericDataTypeCollection::findFiles( const string& path, List< string >& files, const string& ext ) const
	{
		List< string > dirFiles;
		if ( !directory::getFiles( path, dirFiles ) )
		{
			TIKI_TRACE_ERROR( "[genericdata] Unable to find files in '%s'", path.cStr() );
			return;
		}
		
		for (size_t i = 0u; i < dirFiles.getCount(); ++i)
		{
			if ( path::getExtension( dirFiles[ i ] ) != ext )
			{
				continue;
			}

			files.add( path::combine( path, dirFiles[ i ] ) );
		}		

		List< string > dirDirectories;
		if( !directory::getDirectories( path, dirDirectories ) )
		{
			TIKI_TRACE_ERROR( "[genericdata] Unable to find directories in '%s'", path.cStr() );
			return;
		}
		
		for (size_t i = 0u; i < dirDirectories.getCount(); ++i)
		{
			findFiles( path::combine( path, dirDirectories[ i ] ), files, ext );
		}		
	}

	bool GenericDataTypeCollection::parseFile( XmlReader& reader, const _XmlElement* pRootNode, const string& fileName, const string& moduleName )
	{
		bool ok = true;
		const XmlElement* pChildNode = pRootNode->elements;
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
				else if ( isStringEquals( pChildNode->name, "resource" ) )
				{
					type = GenericDataTypeType_Resource;
				}

				GenericDataType* pType = nullptr;
				switch ( type )
				{
				case GenericDataTypeType_Enum:
					{
						if ( pBaseType == nullptr || pBaseType->getType() == GenericDataTypeType_ValueType )
						{
							const GenericDataTypeValueType* pTypesBase = (const GenericDataTypeValueType*)pBaseType;
							pType = TIKI_MEMORY_NEW_OBJECT( GenericDataTypeEnum )( *this, pNameAtt->content, mode, pTypesBase );
						}
						else
						{
							TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] Enum types requires a value type as base type. Typename: %s\n", pNameAtt->content );
							ok = false;
						}
					}
					break;

				case GenericDataTypeType_Struct:
					{
						if ( pBaseType == nullptr || pBaseType->getType() == GenericDataTypeType_Struct )
						{
							const GenericDataTypeStruct* pTypesBase = (const GenericDataTypeStruct*)pBaseType;
							pType = TIKI_MEMORY_NEW_OBJECT( GenericDataTypeStruct )( *this, pNameAtt->content, mode, pTypesBase );
						}
						else
						{
							TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] Struct types requires a struct as base type. Typename: %s\n", pNameAtt->content );
							ok = false;
						}
					}
					break;

				case GenericDataTypeType_Resource:
					{
						if ( pBaseType != nullptr && pBaseType->getType() == GenericDataTypeType_Struct )
						{
							const GenericDataTypeStruct* pTypesBase = (const GenericDataTypeStruct*)pBaseType;
							pType = TIKI_MEMORY_NEW_OBJECT( GenericDataTypeResource )( *this, pNameAtt->content, mode, pTypesBase );
						}
						else
						{
							TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] Resource types requires a struct as base type. Typename: %s\n", pNameAtt->content );
							ok = false;
						}
					}
					break;

				default:
					break;
				}

				if ( pType != nullptr )
				{
					pType->setModule( moduleName );
					if ( addType( *pType ) )
					{
						if ( !pType->loadFromXml( reader, pChildNode ) )
						{
							TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] unable to load type with name '%s' from xml.\n", pType->getName().cStr() );

							removeType( *pType );
							TIKI_MEMORY_DELETE_OBJECT( pType );
							pType = nullptr;

							ok = false;
						}
					}
					else
					{
						TIKI_MEMORY_DELETE_OBJECT( pType );
						pType = nullptr;

						ok = false;
					}
				}
				else
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] unable to resove type: '%s'.\n", pChildNode->name );
					ok = false;
				}
			}
			else if ( pChildNode->name != nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] node has no name attribute and will be ignored.\n" );
				ok = false;
			}

			pChildNode = pChildNode->next;
		}

		return ok;
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

	void GenericDataTypeCollection::writeToFileIfNotEquals( const string& fileName, const string& content )
	{
		Array< char > currentContent;
		file::readAllText( fileName.cStr(), currentContent );
		const uint currentContentLength = currentContent.getCount() - 1;

		bool isEquals = (content.getLength() == currentContentLength);
		if (isEquals)
		{
			for (uint i = 0u; i < content.getLength() && isEquals; ++i)
			{
				isEquals &= content[ i ] == currentContent[ i ];
			}
		}

		if (!isEquals)
		{
			file::writeAllBytes( fileName.cStr(), (const uint8*)content.cStr(), content.getLength() );
		}

		currentContent.dispose();
	}
}
