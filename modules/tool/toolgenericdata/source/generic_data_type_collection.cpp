#include "tiki/toolgenericdata/generic_data_type_collection.hpp"

#include "tiki/base/fourcc.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/container/list.hpp"
#include "tiki/container/map.hpp"
#include "tiki/io/directory.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/path.hpp"
#include "tiki/toolbase/directory_tool.hpp"
#include "tiki/toolgenericdata/generic_data_tag.hpp"
#include "tiki/toolgenericdata/generic_data_type_array.hpp"
#include "tiki/toolgenericdata/generic_data_type_enum.hpp"
#include "tiki/toolgenericdata/generic_data_type_pointer.hpp"
#include "tiki/toolgenericdata/generic_data_type_reference.hpp"
#include "tiki/toolgenericdata/generic_data_type_resource.hpp"
#include "tiki/toolgenericdata/generic_data_type_struct.hpp"
#include "tiki/toolgenericdata/generic_data_type_value_type.hpp"
#include "tiki/toolgenericdata/generic_data_value.hpp"
#include "tiki/toolproject/project.hpp"
#include "tiki/toolproject/package.hpp"
#include "tiki/toolxml/xml_attribute.hpp"
#include "tiki/toolxml/xml_document.hpp"

namespace tiki
{
	struct GenericDataModuleData
	{
		string			name;
		List< string >	dependencies;
	};

	struct GenericDataModuleLoadingData
	{
		Path					filePath;
		XmlDocument				document;
		XmlElement*				pRootNode;
		bool					isLoaded;
		GenericDataModuleData	data;
	};

	GenericDataTypeCollection::GenericDataTypeCollection()
		: m_tagHandler( *this )
	{
		m_pModeEnum = nullptr;
	}

	GenericDataTypeCollection::~GenericDataTypeCollection()
	{
	}

	bool GenericDataTypeCollection::create()
	{
		if( !registerDefaultValueTypes() )
		{
			dispose();
			return false;
		}

		if( !registerDefaultResourceTypes() )
		{
			dispose();
			return false;
		}

		return true;
	}

	void GenericDataTypeCollection::dispose()
	{
		m_pModeEnum = nullptr;

		while( !m_types.isEmpty() )
		{
			GenericDataType& type = m_types.getFirst();

			m_types.removeSortedByValue( type );
			TIKI_DELETE( &type );
		}
	}

	bool GenericDataTypeCollection::addPackage( const Package& package )
	{
		if( m_packages.contains( &package ) )
		{
			return true;
		}
		m_packages.pushBack( &package );

		for( const Package* pParentPackage : package.getDependencies() )
		{
			if( !addPackage( *pParentPackage ) )
			{
				return false;
			}
		}

		List< Path > typeFiles;
		package.findGenericDataTypeFiles( typeFiles );

		return loadFiles( typeFiles );
	}

	GenericDataTagHandler& GenericDataTypeCollection::getTagHandler()
	{
		return m_tagHandler;
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

	const GenericDataTypeCollection::TypeList& GenericDataTypeCollection::getTypes() const
	{
		return m_types;
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
		const GenericDataValue* pValue = m_pModeEnum->getValueByName( name );

		sint64 intValue = 0;
		if ( pValue != nullptr && pValue->getSignedValue( intValue ) )
		{
			return (GenericDataTypeMode)intValue;
		}

		return GenericDataTypeMode_Invalid;
	}

	const GenericDataTypeValueType* GenericDataTypeCollection::getEnumDefaultBaseType() const
	{
		return m_pEnumDefaultType;
	}

	const GenericDataTypeArray* GenericDataTypeCollection::makeArrayType( const GenericDataType* pBaseType )
	{
		GenericDataTypeArray* pArrayType = nullptr;
		if ( !m_arrays.findValue( const_cast< const GenericDataTypeArray** >( &pArrayType ), pBaseType ) )
		{
			const string name = "ResArray<" + pBaseType->getName() + ">";
			pArrayType = TIKI_NEW( GenericDataTypeArray )( *this, name, GenericDataTypeMode_ToolAndRuntime, pBaseType );
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
			pReferenceType = TIKI_NEW( GenericDataTypeReference )( *this, name, GenericDataTypeMode_ToolAndRuntime, pBaseType );
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
			pPointerType = TIKI_NEW( GenericDataTypePointer )(*this, name, GenericDataTypeMode_ToolAndRuntime, pBaseType);
			m_pointers.set( pBaseType, pPointerType );

			TIKI_VERIFY( addType( *pPointerType ) );
		}

		return pPointerType;
	}

	const GenericDataType* GenericDataTypeCollection::parseType( const string& typeString )
	{
		if( GenericDataTag::isTagString( typeString ) )
		{
			GenericDataTag* pTag = TIKI_NEW( GenericDataTag );
			if( !pTag->parseTagString( typeString ) )
			{
				TIKI_DELETE( pTag );
				return false;
			}

			const GenericDataType* pType = m_tagHandler.resolveTypeTag( pTag );
			TIKI_DELETE( pTag );

			return pType;
		}
		else
		{
			return findTypeByName( typeString );
		}
	}

	bool GenericDataTypeCollection::parseValue( GenericDataValue* pTargetValue, const string& valueString, const GenericDataType* pType, const GenericDataType* pParentType )
	{
		if ( pType == nullptr )
		{
			return false;
		}

		string content = valueString;
		if( GenericDataTag::isTagString( valueString ) )
		{
			GenericDataTag* pTag = TIKI_NEW( GenericDataTag );
			if( !pTag->parseTagString( valueString ) )
			{
				TIKI_DELETE( pTag );
				return false;
			}

			if( !m_tagHandler.resolveValueTag( content, pTag, pParentType ) )
			{
				TIKI_DELETE( pTag );
				return false;
			}

			pTargetValue->setValueTag( pTag );
		}
		else
		{
			content = valueString;
		}

		if ( pType->getType() == GenericDataTypeType_ValueType )
		{
			const GenericDataTypeValueType* pTypedType = (const GenericDataTypeValueType*)pType;

			if ( pTypedType->isBoolean() )
			{
				sint64 value;
				if( isStringEquals( content.cStr(), "true" ) )
				{
					value = 1u;
				}
				else if( isStringEquals( content.cStr(), "false" ) )
				{
					value = 0u;
				}
				else if( !string_tools::tryParseSInt64( value, content.cStr() ) )
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseValue] unable to parse integer from '%s'.\n", content.cStr() );
					return false;
				}

				if( value < 0 )
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseValue] %d is not a valid value for boolean.\n", value );
					return false;
				}

				return pTargetValue->setBoolean( value != 0, pType );
			}
			else if ( pTypedType->isSignedInteger() )
			{
				sint64 value;
				if( !string_tools::tryParseSInt64( value, content.cStr() ) )
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseValue] unable to parse signed integer from '%s'.\n", content.cStr() );
					return false;
				}

				return pTargetValue->setSignedValue( value, pType );
			}
			else if ( pTypedType->isUnsignedInteger() )
			{
				uint64 value;
				if( !string_tools::tryParseUInt64( value, content.cStr() ) )
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseValue] unable to parse unsigned integer from '%s'.\n", content.cStr() );
					return false;
				}

				return pTargetValue->setUnsignedValue( value, pType );
			}
			else if ( pTypedType->isFloatingPoint() )
			{
				double value;
				if( !string_tools::tryParseFloat64( value, content.cStr() ) )
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::parseValue] unable to parse float from '%s'.\n", content.cStr() );
					return false;
				}

				return pTargetValue->setFloatingPoint( value, pType );
			}
			else if ( pTypedType->isString() )
			{
				return pTargetValue->setString( content, pType );
			}
		}
		else if ( pType->getType() == GenericDataTypeType_Enum )
		{
			return pTargetValue->setEnum( content, pType );
		}
		else if ( pType->getType() == GenericDataTypeType_Reference )
		{
			return pTargetValue->setReference( content, pType );
		}
		else if( pType->getType() == GenericDataTypeType_Pointer )
		{
			TIKI_NOT_IMPLEMENTED;
			//return pTargetValue->setReference( content, pType );
		}

		return false;
	}

	bool GenericDataTypeCollection::exportCode( GenericDataTypeMode mode, const Path& targetDir )
	{
		if ( !directory::exists( targetDir.getCompletePath() ) )
		{
			if ( !directory::create( targetDir.getCompletePath() ) )
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::exportCode] unable to create target directory(%s).\n", targetDir.getCompletePath() );
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
				factoriesCreateCode += formatDynamicString( s_pFactoriesCreateFormat, type.getName().cStr() );
				factoriesDisposeCode += formatDynamicString( s_pFactoriesDisposeFormat, type.getName().cStr() );
			}
		}

		static const char* s_pBaseFormat =	"#pragma once\n"
											"#ifndef TIKI_%s_INCLUDED\n"
											"#define TIKI_%s_INCLUDED\n"
											"\n"
											"#include \"tiki/base/types.hpp\"\n"
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
											"#endif // TIKI_%s_INCLUDED\n";

		static const char* s_pReference				= "\tclass %s;\n";
		static const char* s_pStringInclude			= "#include \"tiki/base/dynamic_string.hpp\"\n";
		static const char* s_pResourceInclude		= "#include \"tiki/genericdata/generic_data_resource.hpp\"\n";
		static const char* s_pResourceFileInclude	= "#include \"tiki/resource/resource_file.hpp\"\n";
		static const char* s_pDependencyInclude		= "#include \"%s.hpp\"\n";

		for (uint moduleIndex = 0u; moduleIndex < moduleCode.getCount(); ++moduleIndex)
		{
			const auto& kvp = moduleCode.getPairAt( moduleIndex );
			const auto& moduleData = m_modules[ kvp.key ];

			const string fileName		= kvp.key + ".hpp";
			const string fileNameDefine	= fileName.toUpper().replace('.', '_');
			const Path fullPath( targetDir.getCompletePath(), fileName.cStr() );

			if ( kvp.value.containsResource )
			{
				factoriesIncludeCode += formatDynamicString( s_pFactoriesIncludeFormat, fileName.cStr() );
			}

			string referencesCode = "";
			for (uint refIndex = 0u; refIndex < kvp.value.references.getCount(); ++refIndex)
			{
				referencesCode += formatDynamicString( s_pReference, kvp.value.references[ refIndex ]->getBaseType()->getName().cStr() );
			}

			string dependenciesIncludeCode = "";
			for (uint depIndex = 0u; depIndex < moduleData.dependencies.getCount(); ++depIndex)
			{
				dependenciesIncludeCode += formatDynamicString( s_pDependencyInclude, moduleData.dependencies[ depIndex ].cStr() );
			}
			if ( !moduleData.dependencies.isEmpty() )
			{
				dependenciesIncludeCode += "\n";
			}

			string finalCode = formatDynamicString(
				s_pBaseFormat,
				fileNameDefine.cStr(),
				fileNameDefine.cStr(),
				(kvp.value.containsString ? s_pStringInclude : ""),
				(kvp.value.containsResource ? s_pResourceInclude : ""),
				(kvp.value.containsArray || !kvp.value.references.isEmpty() ? s_pResourceFileInclude : ""),
				dependenciesIncludeCode.cStr(),
				referencesCode.cStr(),
				kvp.value.code.cStr(),
				fileNameDefine.cStr()
			);

			writeToFileIfNotEquals( fullPath, finalCode );
		}

		static const char* s_pFactoriesHeaderFormat =	"#pragma once\n"
														"#ifndef TIKI_GENERICDATAFACTORIES_HPP_INCLUDED\n"
														"#define TIKI_GENERICDATAFACTORIES_HPP_INCLUDED\n"
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
														"#endif // TIKI_GENERICDATAFACTORIES_HPP_INCLUDED\n";

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

		const Path headerFullPath( targetDir.getCompletePath(), headerFileName.cStr() );
		const Path sourceFullPath( targetDir.getCompletePath(), sourceFileName.cStr() );

		const string headerFinalCode	= s_pFactoriesHeaderFormat;
		const string sourceFinalCode	= formatDynamicString( s_pFactoriesSourceFormat, factoriesIncludeCode.cStr(), factoriesCreateCode.cStr(), factoriesDisposeCode.cStr() );

		writeToFileIfNotEquals( headerFullPath, headerFinalCode );
		writeToFileIfNotEquals( sourceFullPath, sourceFinalCode );

		return true;
	}

	bool GenericDataTypeCollection::registerDefaultValueTypes()
	{
		bool ok = true;

		GenericDataTypeValueType* pShort	= TIKI_NEW( GenericDataTypeValueType )( *this, "short",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger16 );
		GenericDataTypeValueType* pInt		= TIKI_NEW( GenericDataTypeValueType )( *this, "int",		GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger32 );
		GenericDataTypeValueType* pLong		= TIKI_NEW( GenericDataTypeValueType )( *this, "long",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger64 );

		GenericDataTypeValueType* pSInt8	= TIKI_NEW( GenericDataTypeValueType )( *this, "sint8",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger8 );
		GenericDataTypeValueType* pSInt16	= TIKI_NEW( GenericDataTypeValueType )( *this, "sint16",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger16 );
		GenericDataTypeValueType* pSInt32	= TIKI_NEW( GenericDataTypeValueType )( *this, "sint32",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger32 );
		GenericDataTypeValueType* pSInt64	= TIKI_NEW( GenericDataTypeValueType )( *this, "sint64",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger64 );

		GenericDataTypeValueType* pUInt8	= TIKI_NEW( GenericDataTypeValueType )( *this, "uint8",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_UnsingedInteger8 );
		GenericDataTypeValueType* pUInt16	= TIKI_NEW( GenericDataTypeValueType )( *this, "uint16",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_UnsingedInteger16 );
		GenericDataTypeValueType* pUInt32	= TIKI_NEW( GenericDataTypeValueType )( *this, "uint32",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_UnsingedInteger32 );
		GenericDataTypeValueType* pUInt64	= TIKI_NEW( GenericDataTypeValueType )( *this, "uint64",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_UnsingedInteger64 );

		GenericDataTypeValueType* pHalf		= TIKI_NEW( GenericDataTypeValueType )( *this, "half",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_FloatingPoint16 );
		GenericDataTypeValueType* pFloat	= TIKI_NEW( GenericDataTypeValueType )( *this, "float",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_FloatingPoint32 );
		GenericDataTypeValueType* pDouble	= TIKI_NEW( GenericDataTypeValueType )( *this, "double",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_FloatingPoint64 );

		GenericDataTypeValueType* pFloat16	= TIKI_NEW( GenericDataTypeValueType )( *this, "float16",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_FloatingPoint16 );
		GenericDataTypeValueType* pFloat32	= TIKI_NEW( GenericDataTypeValueType )( *this, "float32",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_FloatingPoint32 );
		GenericDataTypeValueType* pFloat64	= TIKI_NEW( GenericDataTypeValueType )( *this, "float64",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_FloatingPoint64 );

		GenericDataTypeValueType* pCrc32	= TIKI_NEW( GenericDataTypeValueType )( *this, "crc32",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_UnsingedInteger32 );
		GenericDataTypeValueType* pTimeMs	= TIKI_NEW( GenericDataTypeValueType )( *this, "timems",	GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_SingedInteger64 );

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
		ok &= addType( *pTimeMs );

		GenericDataTypeValueType* pBoolean	= TIKI_NEW( GenericDataTypeValueType )( *this, "bool", GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_Boolean );
		GenericDataTypeValueType* pString	= TIKI_NEW( GenericDataTypeValueType )( *this, "string", GenericDataTypeMode_ToolAndRuntime, GenericDataTypeValueTypeType_String );

		ok &= addType( *pBoolean );
		ok &= addType( *pString );

		GenericDataTypeEnum* pGenericDataTypeMode = TIKI_NEW( GenericDataTypeEnum )( *this, "GenericDataTypeMode", GenericDataTypeMode_ToolOnly, pUInt8 );
		pGenericDataTypeMode->addValue( "Invalid",			0, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "RuntimeOnly",		1, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "ToolOnly",			2, GenericDataTypeMode_ToolOnly );
		pGenericDataTypeMode->addValue( "ToolAndRuntime",	3, GenericDataTypeMode_ToolOnly );
		m_pModeEnum = pGenericDataTypeMode;
		m_pEnumDefaultType = pInt;

		ok &= addType( *pGenericDataTypeMode );

		return ok;
	}

	bool GenericDataTypeCollection::registerDefaultResourceTypes()
	{
		bool ok = true;

		GenericDataTypeResource* pAnimation	= TIKI_NEW( GenericDataTypeResource )( *this, "Animation",	GenericDataTypeMode_ToolAndRuntime, "animation",	TIKI_FOURCC( 'A', 'N', 'I', 'M' ) );
		GenericDataTypeResource* pFont		= TIKI_NEW( GenericDataTypeResource )( *this, "Font",		GenericDataTypeMode_ToolAndRuntime, "font",			TIKI_FOURCC( 'F', 'O', 'N', 'T' ) );
		GenericDataTypeResource* pModel		= TIKI_NEW( GenericDataTypeResource )( *this, "Model",		GenericDataTypeMode_ToolAndRuntime, "model",		TIKI_FOURCC( 'M', 'O', 'D', 'L' ) );
		GenericDataTypeResource* pShader	= TIKI_NEW( GenericDataTypeResource )( *this, "ShaderSet",	GenericDataTypeMode_ToolAndRuntime, "shader",		TIKI_FOURCC( 'T', 'G', 'S', 'S' ) );
		GenericDataTypeResource* pTexture	= TIKI_NEW( GenericDataTypeResource )( *this, "Texture",	GenericDataTypeMode_ToolAndRuntime, "texture",		TIKI_FOURCC( 'T', 'E', 'X', 'R' ) );

		ok &= addType( *pAnimation );
		ok &= addType( *pFont );
		ok &= addType( *pModel );
		ok &= addType( *pShader );
		ok &= addType( *pTexture );

		return ok;
	}

	bool GenericDataTypeCollection::loadFiles( const List< Path >& typeFiles )
	{
		Array< GenericDataModuleLoadingData > modules;
		if( !modules.create( typeFiles.getCount() ) )
		{
			dispose();
			return false;
		}

		bool ok = true;
		for( uint i = 0u; i < typeFiles.getCount(); ++i )
		{
			GenericDataModuleLoadingData& data = modules[ i ];
			data.filePath		= typeFiles[ i ];
			data.pRootNode		= nullptr;
			data.isLoaded		= false;
			data.data.name		= data.filePath.getFilename();

			if( data.document.loadFromFile( data.filePath.getCompletePath() ) )
			{
				data.pRootNode = data.document.findFirstChild( "generictypes" );
				if( data.pRootNode == nullptr )
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] '%s' has no root node.\n", data.filePath.getCompletePath() );
					ok = false;
					continue;
				}

				const XmlAttribute* pRootBaseAtt = data.pRootNode->findAttribute( "base" );
				if( pRootBaseAtt != nullptr )
				{
					data.data.dependencies.add( pRootBaseAtt->getValue() );
				}
			}
			else
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] '%s' can't be parsed.\n", data.filePath.getCompletePath() );
				ok = false;
			}
		}

		uint loadedModules = 0u;
		while( loadedModules < modules.getCount() )
		{
			for( uint moduleIndex = 0u; moduleIndex < modules.getCount(); ++moduleIndex )
			{
				GenericDataModuleLoadingData& data = modules[ moduleIndex ];

				if( data.pRootNode == nullptr )
				{
					continue;
				}

				bool hasAllDependencies = true;
				for( uint depIndex = 0u; depIndex < data.data.dependencies.getCount(); ++depIndex )
				{
					for( uint depModIndex = 0u; depModIndex < modules.getCount(); ++depModIndex )
					{
						GenericDataModuleLoadingData& depModData = modules[ depModIndex ];
						if( depModData.data.name == data.data.dependencies[ depIndex ] )
						{
							hasAllDependencies &= depModData.isLoaded;
						}
					}
				}

				if( !hasAllDependencies )
				{
					continue;
				}

				if( !parseFile( data.pRootNode, data.data.name ) )
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] '%s' can't be loaded.\n", data.filePath.getCompletePath() );
					ok = false;
				}

				m_modules[ data.data.name ] = data.data;
				data.pRootNode = nullptr;
				data.isLoaded = true;

				loadedModules++;
			}
		}

		modules.dispose();
		return ok;
	}

	bool GenericDataTypeCollection::parseFile( XmlElement* pRootNode, const string& moduleName )
	{
		bool ok = true;
		XmlElement* pChildNode = pRootNode->getFirstChild();
		while ( pChildNode != nullptr )
		{
			const XmlAttribute* pNameAtt = pChildNode->findAttribute( "name" );
			const XmlAttribute* pBaseAtt = pChildNode->findAttribute( "base" );
			const XmlAttribute* pModeAtt = pChildNode->findAttribute( "mode" );

			if ( pNameAtt != nullptr )
			{
				const GenericDataType* pBaseType = nullptr;
				if ( pBaseAtt != nullptr )
				{
					pBaseType = parseType( pBaseAtt->getValue() );
				}

				GenericDataTypeMode mode = GenericDataTypeMode_ToolAndRuntime;
				if ( pModeAtt != nullptr )
				{
					mode = findModeByName( pModeAtt->getValue() );
					if ( mode == GenericDataTypeMode_Invalid )
					{
						TIKI_TRACE_WARNING( "[GenericDataTypeCollection::create] type named '%s' has a invalid mode(%s).\n", pNameAtt->getValue(), pModeAtt->getValue() );
						mode = GenericDataTypeMode_ToolAndRuntime;
					}
				}

				GenericDataTypeType type = GenericDataTypeType_Invalid;
				if ( pChildNode->isName( "enum" ) )
				{
					type = GenericDataTypeType_Enum;
				}
				else if ( pChildNode->isName( "struct" ) )
				{
					type = GenericDataTypeType_Struct;
				}
				else if ( pChildNode->isName( "resource" ) )
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
							pType = TIKI_NEW( GenericDataTypeEnum )( *this, pNameAtt->getValue(), mode, pTypesBase );
						}
						else
						{
							TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] Enum types requires a value type as base type. Typename: %s\n", pNameAtt->getValue() );
							ok = false;
						}
					}
					break;

				case GenericDataTypeType_Struct:
					{
						if ( pBaseType == nullptr || pBaseType->getType() == GenericDataTypeType_Struct )
						{
							const GenericDataTypeStruct* pTypesBase = (const GenericDataTypeStruct*)pBaseType;
							pType = TIKI_NEW( GenericDataTypeStruct )( *this, pNameAtt->getValue(), mode, pTypesBase );
						}
						else
						{
							TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] Struct types requires a struct as base type. Typename: %s\n", pNameAtt->getValue() );
							ok = false;
						}
					}
					break;

				case GenericDataTypeType_Resource:
					{
						if ( pBaseType != nullptr && pBaseType->getType() == GenericDataTypeType_Struct )
						{
							const GenericDataTypeStruct* pTypesBase = (const GenericDataTypeStruct*)pBaseType;
							pType = TIKI_NEW( GenericDataTypeResource )( *this, pNameAtt->getValue(), mode, pTypesBase );
						}
						else
						{
							TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] Resource types requires a struct as base type. Typename: %s\n", pNameAtt->getValue() );
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
						if ( !pType->loadFromXml( pChildNode ) )
						{
							TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] unable to load type with name '%s' from xml.\n", pType->getName().cStr() );

							removeType( *pType );
							TIKI_DELETE( pType );
							pType = nullptr;

							ok = false;
						}
					}
					else
					{
						TIKI_DELETE( pType );
						pType = nullptr;

						ok = false;
					}
				}
				else
				{
					TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] unable to resove type: '%s'.\n", pChildNode->getName() );
					ok = false;
				}
			}
			else if ( pChildNode->getName() != nullptr )
			{
				TIKI_TRACE_ERROR( "[GenericDataTypeCollection::create] node has no name attribute and will be ignored.\n" );
				ok = false;
			}

			pChildNode = pChildNode->getNextSibling();
		}

		return ok;
	}

	void GenericDataTypeCollection::writeToFileIfNotEquals( const Path& filePath, const string& content )
	{
		Array< char > currentContent;
		file::readAllText( filePath.getCompletePath(), currentContent );
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
			file::writeAllBytes( filePath.getCompletePath(), (const uint8*)content.cStr(), content.getLength() );
		}

		currentContent.dispose();
	}
}
