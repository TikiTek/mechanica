#include "tiki/shader_converter/shader_converter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/container/array.hpp"
#include "tiki/container/map.hpp"
#include "tiki/converter_base/conversion_result.hpp"
#include "tiki/converter_base/conversion_types.hpp"
#include "tiki/converter_base/resource_writer.hpp"
#include "tiki/graphics/shader_type.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/file_stream.hpp"
#include "tiki/io/path.hpp"
#include "tiki/shader_converter/shader_preprocessor.hpp"
#include "tiki/tool_project/project.hpp"

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
#	include <d3dcompiler.h>
#endif

#include <TRexpp.h>
#include <stdarg.h>

namespace tiki
{
	static const char* s_pShaderTypeName = "shader";
	static const crc32 s_shaderTypeCrc = crcString( s_pShaderTypeName );

	struct ShaderVariantData
	{
		ShaderType		type;
		crc32			variantKey;
		uint32			codeLength;
		ReferenceKey	key;
	};

	struct ShaderConstantInfo
	{
		uint32			slotIndex;
		DynamicString	name;
	};

	class BasicIncludeHandler
	{
		TIKI_NONCOPYABLE_CLASS( BasicIncludeHandler );

	public:

		BasicIncludeHandler( ConversionResult& result, const List< DynamicString >& includePathes )
			: m_result( result )
			, m_includePathes( includePathes )
		{
		}

		virtual ~BasicIncludeHandler()
		{
		}

		bool loadFile( const char* pFileName, const void** ppData, uintreg* pSizeInBytes )
		{
			bool found = false;

			const DynamicString inputFilename = pFileName;
			DynamicString fullName = inputFilename;

			if( file::exists( inputFilename.cStr() ) )
			{
				found = true;
			}
			else
			{
				for( uintreg i = 0u; i < m_includePathes.getCount(); ++i )
				{
					fullName = path::combine( m_includePathes[ i ], inputFilename );

					if( file::exists( fullName.cStr() ) )
					{
						found = true;
						break;
					}
				}
			}

			if( found )
			{
				Array< char > text;
				if( !file::readAllText( fullName.cStr(), text ) )
				{
					TIKI_TRACE_ERROR( "Could not read File: %s.\n", fullName.cStr() );
					return false;
				}

				*ppData = dublicateString( text.getBegin() );
				*pSizeInBytes = text.getCapacity();
				text.dispose();

				m_result.addDependency( ConversionResult::DependencyType_InputFile, fullName, 0 );
				return true;
			}

			TIKI_TRACE_ERROR( "Could find File: %s.\n", pFileName );
			return false;
		}

		void freeFile( void* pData )
		{
			TIKI_FREE( pData );
		}

	private:

		ConversionResult&		m_result;
		const List< DynamicString >&	m_includePathes;
	};

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
	class ShaderIncludeHandler : public ID3DInclude, public BasicIncludeHandler
	{
		TIKI_NONCOPYABLE_CLASS( ShaderIncludeHandler );

	public:

		ShaderIncludeHandler( ConversionResult& result, const List< DynamicString >& includePathes )
			: BasicIncludeHandler( result, includePathes )
		{
		}

		virtual HRESULT	STDMETHODCALLTYPE Open( D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes )
		{
			const void* pData = nullptr;
			uintreg dataSize = 0u;
			if ( loadFile( pFileName, &pData, &dataSize ) )
			{
				*ppData = pData;
				*pBytes = (UINT)dataSize;

				return S_OK;
			}

			return S_FALSE;
		}

		virtual HRESULT	STDMETHODCALLTYPE Close( LPCVOID pData )
		{
			freeFile( (void*)pData );
			return S_OK;
		}

	};
#else
	class ShaderIncludeHandler : public BasicIncludeHandler
	{
		TIKI_NONCOPYABLE_CLASS( ShaderIncludeHandler );

	public:

		ShaderIncludeHandler( ConversionResult& result, ShaderFileStorage& storage )
			: BasicIncludeHandler( result, storage )
		{
		}
	};
#endif

	struct FppContext
	{
		const char* pSourceData;
		uintreg		sourceLength;
		uintreg		sourcePosition;

		char*		pTargetData;
		uintreg		targetLength;
		uintreg		targetPosition;
	};

	static char*	fppRead( char* pBuffer, int size, void* pUserData );
	static void		fppWrite( int c, void* pUserData );
	static void		fppError( void* pUserData, char* pFormatString, va_list data );

	//static DynamicString	preprocessSourceCode( const DynamicString& sourceCode, const DynamicString& fileName, const ShaderIncludeHandler* pIncludeHandler );

	ShaderConverter::ShaderConverter()
	{
	}

	ShaderConverter::~ShaderConverter()
	{
	}

	uint32 ShaderConverter::getConverterRevision( crc32 typeCrc ) const
	{
		return 1u;
	}

	bool ShaderConverter::canConvertType( crc32 typeCrc ) const
	{
		return typeCrc == s_shaderTypeCrc;
	}

	void ShaderConverter::getInputExtensions( List< DynamicString >& extensions ) const
	{
		extensions.pushBack( ".fx" );
	}

	const char* ShaderConverter::getOutputType() const
	{
		return s_pShaderTypeName;
	}

	bool ShaderConverter::initializeConverter( const ConversionContext& context )
	{
		m_pBaseSourceCode =	"#define TIKI_ON 2-\n"
							"#define TIKI_OFF 1-\n"
							"#define TIKI_ENABLED( value ) ( ( value 0 ) == 2 )\n"
							"#define TIKI_DISABLED( value ) ( ( value 0 ) != 2 )\n\n";

		// TODO: put shader include dirs in package config
		Array< char > charArray;
		if ( file::readAllText( "../../shaderinc.lst", charArray ) )
		{
			const DynamicString text = charArray.getBegin();

			Array< DynamicString > dirs;
			text.split( dirs, "\n" );

			for (uintreg i = 0u; i < dirs.getCount(); ++i)
			{
				m_includePathes.add( dirs[ i ].trim() );
			}

			dirs.dispose();
		}
		charArray.dispose();

		m_openGlMutex.create();

		return true;
	}

	void ShaderConverter::disposeConverter()
	{
		m_openGlMutex.dispose();

		m_includePathes.dispose();
	}

	bool ShaderConverter::startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const
	{
		List< DynamicString > includePathes = m_includePathes;
		includePathes.pushBack( asset.inputFilePath.getDirectoryWithPrefix() );

		Path additionalPath;
		additionalPath.setCombinedPath( context.pProject->getProjectPath().getCompletePath(), "library/modules/runtime/render_effects/source/shader" );
		includePathes.add( additionalPath.getCompletePath() );

		additionalPath.setCombinedPath( context.pProject->getProjectPath().getCompletePath(), "library/modules/runtime/renderer_2d/source/shader" );
		includePathes.add( additionalPath.getCompletePath() );

		ShaderIncludeHandler includeHandler( result, includePathes );

		const DynamicString shaderStart[]	= { "vs", "ps", "gs", "hs", "ds", "cs" };
		const DynamicString shaderDefine[]	= { "TIKI_VERTEX_SHADER", "TIKI_PIXEL_SHADER", "TIKI_GEOMETRY_SHADER", "TIKI_HULL_SHADER", "TIKI_DOMAIN_SHADER", "TIKI_COMPUTE_SHADER" };

		DynamicString functionNames[ ShaderType_Count ];
		for (uintreg i = 0u; i < TIKI_COUNT( shaderStart ); ++i)
		{
			functionNames[ i ] = asset.parameters.getOptionalString( shaderStart[ i ] + "_function_name", "main" );
		}

		Array< char > charArray;
		if ( !file::readAllText( asset.inputFilePath.getCompletePath(), charArray ) )
		{
			TIKI_TRACE_ERROR( "Can't read '%s'.\n", asset.inputFilePath.getCompletePath() );
			return false;
		}

		const DynamicString sourceCode = charArray.getBegin();
		charArray.dispose();

		const bool debugMode = asset.parameters.getOptionalBool( "compile_debug", false );

		ShaderPreprocessor preprocessor;
		preprocessor.create( sourceCode, includePathes );

		ResourceWriter resourceWriter;
		openResourceWriter( resourceWriter, result, asset.assetName.cStr(), "shader" );
		for( const ResourceDefinition& definition : getResourceDefinitions( FlagMask8< ResourceDefinitionFeature >( ResourceDefinitionFeature_GraphicsApi ) ) )
		{
			resourceWriter.openResource( asset.assetName + ".shader", TIKI_FOURCC( 'T', 'G', 'S', 'S' ), definition, getConverterRevision( s_shaderTypeCrc ) );

			ResourceSectionWriter sectionWriter;
			resourceWriter.openDataSection( sectionWriter, SectionType_Main );

			List< ShaderVariantData > shaderVariants;
			for (uintreg typeIndex = 0u; typeIndex < ShaderType_Count; ++typeIndex )
			{
				const ShaderType type = (ShaderType)typeIndex;

				if ( preprocessor.isTypeEnabled( type ) == false )
				{
					continue;
				}

				const uintreg variantCount = preprocessor.getVariantCount( type );
				for (uintreg variantIndex = 0u; variantIndex < variantCount; ++variantIndex )
				{
					const ShaderVariant& variant = preprocessor.getVariantByIndex( type, variantIndex );

					ShaderArguments args;
					args.type		= type;

					args.fileName	= asset.inputFilePath.getCompletePath();
					args.outputName	= asset.assetName;

					args.entryPoint	= functionNames[ type ];
					args.version	= shaderStart[ type ] + "_4_0";
					args.debugMode	= debugMode;

					args.defineCode = m_pBaseSourceCode;
					args.defineCode += variant.defineCode;

					for( uintreg defineTypeIndex = 0u; defineTypeIndex < ShaderType_Count; ++defineTypeIndex )
					{
						args.defineCode	+= formatDynamicString( "#define %s %s\n", shaderDefine[ defineTypeIndex ].cStr(), ( typeIndex == defineTypeIndex ? "TIKI_ON" : "TIKI_OFF" ) );
					}

					Array< uint8 > variantData;
					if ( compilePlatformShader( variantData, args, includeHandler, definition.getGraphicsApi() ) )
					{
						uint32 keyData[] = { (uint32)type, variant.bitMask };

						ShaderVariantData& variantVarName = shaderVariants.add();
						variantVarName.type			= type;
						variantVarName.codeLength	= uint32( variantData.getCount() );
						variantVarName.variantKey	= crcBytes( keyData, sizeof( keyData ) );

						variantVarName.key = sectionWriter.addDataPoint();
						sectionWriter.writeData( variantData.getBegin(), variantData.getCount() );

						variantData.dispose();
					}
				}
			}

			resourceWriter.closeDataSection( sectionWriter );
			resourceWriter.openDataSection( sectionWriter, SectionType_Initializaion );

			sectionWriter.writeUInt32( uint32( shaderVariants.getCount() ) );
			sectionWriter.writeAlignment( 8u );

			for( uintreg variantIndex = 0u; variantIndex < shaderVariants.getCount(); ++variantIndex )
			{
				const ShaderVariantData& shaderVarName = shaderVariants[ variantIndex ];

				sectionWriter.writeUInt32( shaderVarName.type );
				sectionWriter.writeUInt32( shaderVarName.codeLength );
				sectionWriter.writeUInt32( shaderVarName.variantKey );
				sectionWriter.writeReference( &shaderVarName.key );
			}

			resourceWriter.closeDataSection( sectionWriter );
			resourceWriter.closeResource();
		}

		closeResourceWriter( resourceWriter );
		preprocessor.dispose();
		return true;
	}

	bool ShaderConverter::compilePlatformShader( Array< uint8 >& targetData, const ShaderArguments& args, ShaderIncludeHandler& includeHandler, GraphicsApi targetApi ) const
	{
		switch ( targetApi )
		{
		case GraphicsApi_D3D11:
		case GraphicsApi_D3D12:
			return compileD3dShader( targetData, args, includeHandler );

		case GraphicsApi_Vulkan:
			return compileVulkanShader( targetData, args, includeHandler );

		default:
			TIKI_TRACE_ERROR( "Graphics API not supported.\n" );
			break;
		}

		return false;
	}

	bool ShaderConverter::compileD3dShader( Array< uint8 >& targetData, const ShaderArguments& args, ShaderIncludeHandler& includeHandler ) const
	{
#if TIKI_ENABLED( TIKI_BUILD_MSVC )
		ID3D10Blob* pBlob		= nullptr;
		ID3D10Blob* pErrorBlob	= nullptr;

		UINT shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
		if ( args.debugMode )
		{
			shaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		}

		const DynamicString sourceCode = args.defineCode + formatDynamicString( "\n#define TIKI_HLSL4 TIKI_ON\n#define TIKI_OPENGL4 TIKI_OFF\n#include \"%s\"", args.fileName.cStr() );

		HRESULT result = D3DCompile(
			sourceCode.cStr(),
			SIZE_T( sourceCode.getLength() ),
			args.fileName.cStr(),
			nullptr,
			&includeHandler,
			args.entryPoint.cStr(),
			args.version.cStr(),
			shaderFlags,
			0,
			&pBlob,
			&pErrorBlob
		);

		if ( FAILED( result ) )
		{
			if ( result == ERROR_FILE_NOT_FOUND )
			{
				TIKI_TRACE_ERROR( "impossible file not found: %s\n", args.fileName.cStr() );
			}
			else if ( pErrorBlob )
			{
				const char* pError = (const char*)pErrorBlob->GetBufferPointer();
				TIKI_TRACE_ERROR( "failed to compile shader. error message:\n%s\n", pError );
			}
			else
			{
				TIKI_TRACE_ERROR( "failed to compile effect: unknown error.\n" );
			}

			return false;
		}
		else
		{
			targetData.create( static_cast< uint8* >( pBlob->GetBufferPointer() ), pBlob->GetBufferSize() );
		}

		if ( pBlob != nullptr )
		{
			pBlob->Release();
			pBlob = nullptr;
		}

		if ( pErrorBlob != nullptr )
		{
			pErrorBlob->Release();
			pErrorBlob = nullptr;
		}

		return true;
#else
		return false;
#endif
	}

	bool ShaderConverter::compileVulkanShader( Array< uint8 >& targetData, const ShaderArguments& args, ShaderIncludeHandler& includeHandler ) const
	{
		m_openGlMutex.lock();

		DynamicString sourceCode = "#version 440\n" + args.defineCode + formatDynamicString( "\n#define TIKI_HLSL4 TIKI_OFF\n#define TIKI_OPENGL4 TIKI_ON\n#include \"%s\"\n", args.fileName.cStr() );
		List< ShaderConstantInfo > constants;

		// preprocessor
		//sourceCode = preprocessSourceCode( sourceCode, args.fileName, &includeHandler );

		// parse uniforms
		{
			TRexpp regex;
			regex.Compile( "[\\s]*layout\\([\\s]*location[\\s]*=[\\s]*([0-9]+[\\s]*)\\)[\\s]*uniform[\\s]+([\\w]+)" );

			const char* pSearchBegin	= nullptr;
			const char* pSearchEnd		= nullptr;
			while ( regex.Search( sourceCode.cStr(), &pSearchBegin, &pSearchEnd ) )
			{
				int length;
				const char* pArgumentBegin;
				regex.GetSubExp( 1, &pArgumentBegin, &length );
				const DynamicString slotIndex = DynamicString( pArgumentBegin, length );

				regex.GetSubExp( 2, &pArgumentBegin, &length );
				const DynamicString name = DynamicString( pArgumentBegin, length );

				ShaderConstantInfo& info = constants.add();
				info.slotIndex	= string_tools::parseSInt32( slotIndex.cStr() );
				info.name		= name;

				const uintreg startIndex	= pSearchBegin - sourceCode.cStr();
				const uintreg length2		= pSearchEnd - pSearchBegin;
				sourceCode = sourceCode.remove( startIndex, length2 );

				const DynamicString newCode = formatDynamicString( "layout(shared) uniform %s", name.cStr() );
				sourceCode = sourceCode.insert( newCode, startIndex );
			}
		}

		//// parse code
		//if ( false )
		//{
		//	static const EShLanguage s_aLanguageMapping[] =
		//	{
		//		EShLangCount,			// ShaderType_Effect
		//		EShLangVertex,			// ShaderType_VertexShader
		//		EShLangFragment,		// ShaderType_PixelShader
		//		EShLangGeometry,		// ShaderType_GeometrieShader
		//		EShLangTessControl,		// ShaderType_HullShader
		//		EShLangTessEvaluation,	// ShaderType_DomainShader
		//		EShLangCompute			// ShaderType_ComputeShader
		//	};
		//	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aLanguageMapping ) == ShaderType_Count );

		//	glslang::TShader shaderTest( s_aLanguageMapping[ args.type ] );

		//	const char* pSourceCodeString = sourceCode.cStr();
		//	shaderTest.setStrings( &pSourceCodeString, 1u );

		//	TBuiltInResource shaderResources = { 0 };
		//	if ( !shaderTest.parse( &shaderResources, 400, true, EShMsgDefault ) )
		//	{
		//		TIKI_TRACE_INFO( "[shaderconverter] parser has return an error.\n" );
		//		//TIKI_TRACE_INFO( sourceCode.cStr() );
		//		//TIKI_TRACE_INFO( shaderTest.getInfoLog() );
		//	}
		//}

		MemoryStream stream;
		stream.create( sourceCode.getLength() );

		const uint32 constantCount = (uint32)constants.getCount();
		stream.write( &constantCount, sizeof( constantCount ) );

		for (uintreg i = 0u; i < constantCount; ++i)
		{
			const ShaderConstantInfo& info = constants[ i ];
			const uint32 nameLength = (uint32)info.name.getLength();

			stream.write( &info.slotIndex, sizeof( info.slotIndex ) );
			stream.write( &nameLength, sizeof( nameLength ) );
			stream.write( info.name.cStr(), nameLength + 1u );
		}

		stream.write( sourceCode.cStr(), sourceCode.getLength() + 1u );

		m_openGlMutex.unlock();

		return targetData.create( static_cast< const uint8* >( stream.getData() ), uintreg( stream.getLength() ) );
	}

	char* fppRead( char* pBuffer, int size, void* pUserData )
	{
		TIKI_ASSERT( pBuffer != nullptr );
		TIKI_ASSERT( pUserData != nullptr );

		FppContext* pContext = (FppContext*)pUserData;

		if ( pContext->sourcePosition == pContext->sourceLength )
		{
			return nullptr;
		}

		int bufferPosition = 0;
		char c = 0;
		do
		{
			c = pContext->pSourceData[ pContext->sourcePosition++ ];
			pBuffer[ bufferPosition++ ] = c;
		}
		while ( c != '\n' && bufferPosition < size - 1 && pContext->sourcePosition < pContext->sourceLength );

		pBuffer[ bufferPosition ] = '\0';
		return pBuffer;
	}

	void fppWrite( int c, void* pUserData )
	{
		TIKI_ASSERT( pUserData != nullptr );

		FppContext* pContext = (FppContext*)pUserData;
		if ( pContext->targetPosition == pContext->targetLength )
		{
			TIKI_TRACE_ERROR( "[shaderconverter] Target Buffer is too small.\n" );
			return;
		}

		pContext->pTargetData[ pContext->targetPosition++ ] = c;
	}

	void fppError( void* /*pUserData*/, char* pFormatString, va_list data )
	{
		TIKI_ASSERT( pFormatString != nullptr );

		debug::trace( pFormatString, TraceLevel_None, data );
	}

	//DynamicString preprocessSourceCode( const DynamicString& sourceCode, const DynamicString& fileName, const ShaderIncludeHandler* pIncludeHandler )
	//{
	//	fppTag aTags[ 64u ];
	//	fppTag* pCurrentTag = aTags;

	//	FppContext context;
	//	context.pSourceData			= sourceCode.cStr();
	//	context.sourceLength		= sourceCode.getLength();
	//	context.sourcePosition		= 0;
	//	context.pTargetData			= (char*)TIKI_ALLOC( 1024u * 1024u );
	//	context.pTargetData[ 0u ]	= '\0';
	//	context.targetLength		= 1024u * 1024u;
	//	context.targetPosition		= 0u;

	//	for (uintreg i = 0u; i < pIncludeHandler->getIncludeDirs().getCount(); ++i)
	//	{
	//		pCurrentTag->tag	= FPPTAG_INCLUDE_DIR;
	//		pCurrentTag->data	= (void*)pIncludeHandler->getIncludeDirs()[ i ].cStr();
	//		pCurrentTag++;
	//	}

	//	const DynamicString sourceFileName = path::getFilename( fileName );
	//	pCurrentTag->tag	= FPPTAG_INPUT_NAME;
	//	pCurrentTag->data	= (void*)sourceFileName.cStr();
	//	pCurrentTag++;

	//	pCurrentTag->tag	= FPPTAG_INPUT;
	//	pCurrentTag->data	= (void*)fppRead;
	//	pCurrentTag++;

	//	pCurrentTag->tag	= FPPTAG_OUTPUT;
	//	pCurrentTag->data	= (void*)fppWrite;
	//	pCurrentTag++;

	//	pCurrentTag->tag	= FPPTAG_ERROR;
	//	pCurrentTag->data	= (void*)fppError;
	//	pCurrentTag++;

	//	pCurrentTag->tag	= FPPTAG_USERDATA;
	//	pCurrentTag->data	= (void*)&context;
	//	pCurrentTag++;

	//	pCurrentTag->tag	= FPPTAG_LINE;
	//	pCurrentTag->data	= (void*)FALSE;
	//	pCurrentTag++;

	//	pCurrentTag->tag	= FPPTAG_IGNOREVERSION;
	//	pCurrentTag->data	= (void*)FALSE;
	//	pCurrentTag++;

	//	pCurrentTag->tag	= FPPTAG_END;
	//	pCurrentTag->data	= nullptr;
	//	pCurrentTag++;

	//	if ( fppPreProcess( aTags ) != 0 )
	//	{
	//		TIKI_TRACE_ERROR( "[shaderconverter] preprocessor failed.\n" );
	//	}

	//	context.pTargetData[ context.targetPosition ] = '\0';
	//	DynamicString resultSourceCode = context.pTargetData;

	//	TIKI_FREE( context.pTargetData );

	//	return resultSourceCode;
	//}
}
