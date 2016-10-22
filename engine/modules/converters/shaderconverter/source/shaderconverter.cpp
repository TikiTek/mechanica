
#include "tiki/shaderconverter/shaderconverter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/stringparse.hpp"
#include "tiki/container/array.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/convertermanager.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/graphics/shadertype.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/filestream.hpp"
#include "tiki/io/path.hpp"
#include "tiki/shaderconverter/shaderpreprocessor.hpp"

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
#	include <d3dcompiler.h>
#endif

#include <TRexpp.h>
#include <stdarg.h>

namespace tiki
{
	static const crc32 s_typeCrc = crcString( "shader" );

	struct ShaderVariantData
	{
		ShaderType		type;
		crc32			variantKey;
		uint32			codeLength;
		ReferenceKey	key;
	};

	struct ShaderConstantInfo
	{
		uint32	slotIndex;
		string	name;
	};

	class ShaderFileStorage
	{
		TIKI_NONCOPYABLE_CLASS( ShaderFileStorage );

	public:

		ShaderFileStorage( const List< string >& includeDirs )
			: m_includeDirs( includeDirs )
			, m_jobCount(0u)
		{
			m_mutex.create();
		}

		~ShaderFileStorage()
		{
			m_mutex.dispose();
		}
		
		void beginJob()
		{
			m_jobCount++;
		}

		void endJob()
		{
			m_jobCount--;
			if ( !m_jobCount )
			{
				m_fileMap.clear();
			}
		}

		bool getFile( const char* pFileName, const char** ppFullName, const void** ppData, uint* pSizeInBytes )
		{
			TIKI_ASSERT( ppData != nullptr );
			TIKI_ASSERT( pSizeInBytes != nullptr );

			FileData data;

			m_mutex.lock();
			const bool found = m_fileMap.findValue( &data, pFileName );
			if ( !found )
			{
				if ( !loadFile( pFileName, data ) )
				{
					m_mutex.unlock();
					return false;
				}
			}
			m_mutex.unlock();

			*ppFullName		= data.pFullName;
			*ppData			= data.pData;
			*pSizeInBytes	= getStringSize( data.pData );
			
			return true;
		}

		const List< string >& getIncludeDirs() const
		{
			return m_includeDirs;
		}

	private:

		struct FileData
		{
			const char*	pFullName;
			const char*	pData;
		};

		Mutex					m_mutex;
		volatile uint			m_jobCount;

		List< string >			m_includeDirs;
		Map< string, FileData >	m_fileMap;

		bool loadFile( const char* pFileName, FileData& data )
		{
			bool found = false;

			const string inputFilename = pFileName;
			string fullName = inputFilename;

			if ( file::exists( inputFilename.cStr() ) )
			{
				found = true;
			}
			else
			{
				for (uint i = 0u; i < m_includeDirs.getCount(); ++i)
				{
					fullName = path::combine( m_includeDirs[ i ], inputFilename );

					if ( file::exists( fullName.cStr() ) )
					{
						found = true;
						break;
					}
				}
			}

			if ( found )
			{
				Array< char > text;
				if ( !file::readAllText( fullName.cStr(), text ) )
				{
					TIKI_TRACE_ERROR( "Could not read File: %s.\n", fullName.cStr() );
					return false;
				}

				data.pFullName	= dublicateString( fullName.cStr() );
				data.pData		= dublicateString( text.getBegin() );
				text.dispose();

				m_fileMap.set( pFileName, data );
				return true;
			}

			TIKI_TRACE_ERROR( "Could find File: %s.\n", pFileName );
			return false;
		}

	};

	class BasicIncludeHandler
	{
        TIKI_NONCOPYABLE_CLASS( BasicIncludeHandler );

    public:

        BasicIncludeHandler( ConversionResult& result, ShaderFileStorage& storage )
			: m_result( result )
			, m_storage( storage )
		{
			m_storage.beginJob();
		}

		virtual ~BasicIncludeHandler()
		{
			m_storage.endJob();
		}

		bool loadFile( const char* pFileName, const void** ppData, uint* pSizeInBytes )
		{
			const char* pFullName;
			if ( m_storage.getFile( pFileName, &pFullName, ppData, pSizeInBytes ) )
			{
				m_result.addDependency( ConversionResult::DependencyType_File, pFullName, 0u );

				return true;
			}
			
			return false;
		}

		bool freeFile( const void* pData )
		{
			// nothing to do

			return true;
		}

		const List< string >& getIncludeDirs() const
		{
			return m_storage.getIncludeDirs();
		}

	private:

		ConversionResult&	m_result;
		ShaderFileStorage&	m_storage;

	};

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
	class ShaderIncludeHandler : public ID3DInclude, public BasicIncludeHandler
	{
		TIKI_NONCOPYABLE_CLASS( ShaderIncludeHandler );

	public:

		ShaderIncludeHandler( ConversionResult& result, ShaderFileStorage& storage )
            : BasicIncludeHandler( result, storage )
		{
		}

		virtual HRESULT	STDMETHODCALLTYPE Open( D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes )
		{
		    const void* pData = nullptr;
		    uint dataSize = 0u;
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
			if ( freeFile( pData ) )
			{
				return S_OK;
			}

			return S_FALSE;
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
		uint		sourceLength;
		uint		sourcePosition;

		char*		pTargetData;
		uint		targetLength;
		uint		targetPosition;
	};

	static char*	fppRead( char* pBuffer, int size, void* pUserData );
	static void		fppWrite( int c, void* pUserData );
	static void		fppError( void* pUserData, char* pFormatString, va_list data );

	static string	preprocessSourceCode( const string& sourceCode, const string& fileName, const ShaderIncludeHandler* pIncludeHandler );

	ShaderConverter::ShaderConverter()
	{
		m_pFileStorage = nullptr;
	}

	ShaderConverter::~ShaderConverter()
	{
		TIKI_ASSERT( m_pFileStorage == nullptr );
	}

	uint32 ShaderConverter::getConverterRevision( crc32 typeCrc ) const
	{
		return 1u;
	}

	bool ShaderConverter::canConvertType( crc32 typeCrc ) const
	{
		return typeCrc == s_typeCrc;
	}

	crc32 ShaderConverter::getOutputType() const
	{
		return s_typeCrc;
	}

	void ShaderConverter::getDependingType( List< crc32 >& types ) const
	{
	}

	bool ShaderConverter::initializeConverter()
	{
		m_pBaseSourceCode =	"#define TIKI_ON 2-\n"
							"#define TIKI_OFF 1-\n"
							"#define TIKI_ENABLED( value ) ( ( value 0 ) == 2 )\n"
							"#define TIKI_DISABLED( value ) ( ( value 0 ) != 2 )\n\n";

		m_includeDirs.add( "./" );

		Array< char > charArray;
		if ( file::readAllText( "../../shaderinc.lst", charArray ) )
		{
			const string text = charArray.getBegin();

			Array< string > dirs;
			text.split( dirs, "\n" );

			for (uint i = 0u; i < dirs.getCount(); ++i)
			{
				m_includeDirs.add( dirs[ i ].trim() );
			}

			dirs.dispose();
		}
		charArray.dispose();

		m_pFileStorage = TIKI_NEW( ShaderFileStorage )( m_includeDirs );

		m_openGlMutex.create();

		return true;
	}

	void ShaderConverter::disposeConverter()
	{
		m_openGlMutex.dispose();

		TIKI_DELETE( m_pFileStorage );
		m_pFileStorage = nullptr;

		m_includeDirs.dispose();
	}

	bool ShaderConverter::startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const
	{
		ShaderIncludeHandler includeHandler( result, *m_pFileStorage );

		const string shaderStart[]	= { "fx", "vs", "ps", "gs", "hs", "ds", "cs" };
		const string shaderDefine[]	= { "", "TIKI_VERTEX_SHADER", "TIKI_PIXEL_SHADER", "TIKI_GEOMETRY_SHADER", "TIKI_HULL_SHADER", "TIKI_DOMAIN_SHADER", "TIKI_COMPUTE_SHADER" };

		string functionNames[ ShaderType_Count ];
		for (uint i = 0u; i < TIKI_COUNT( shaderStart ); ++i)
		{
			functionNames[ i ] = parameters.arguments.getOptionalString( shaderStart[ i ] + "_function_name", "main" );
		}

		for( uint fileIndex = 0u; fileIndex < parameters.inputFiles.getCount(); ++fileIndex )
		{
			const ConversionParameters::InputFile& file = parameters.inputFiles[ fileIndex ];

			Array< char > charArray;
			if ( !file::readAllText( file.fileName.cStr(), charArray ) )
			{
				TIKI_TRACE_ERROR( "Can't open file.\n" );
				continue;
			}

			const string sourceCode = charArray.getBegin();
			charArray.dispose();

			const bool debugMode = parameters.arguments.getOptionalBool( "compile_debug", false );

			ShaderPreprocessor preprocessor;
			preprocessor.create( sourceCode );

			ResourceWriter writer;
			openResourceWriter( writer, result, parameters.outputName, "shader" );

			for (const ResourceDefinition& definition : getResourceDefinitions())
			{
				writer.openResource( parameters.outputName + ".shader", TIKI_FOURCC( 'T', 'G', 'S', 'S' ), definition, getConverterRevision( s_typeCrc ) );

				List< ShaderVariantData > shaderVariants;
				for (uint typeIndex = 1u; typeIndex < ShaderType_Count; ++typeIndex )
				{
					const ShaderType type = (ShaderType)typeIndex;

					if ( preprocessor.isTypeEnabled( type ) == false )
					{
						continue;
					}

					const uint variantCount = preprocessor.getVariantCount( type );
					for (uint variantIndex = 0u; variantIndex < variantCount; ++variantIndex )
					{
						const ShaderVariant& variant = preprocessor.getVariantByIndex( type, variantIndex );

						ShaderArguments args;
						args.type		= type;

						args.fileName	= file.fileName;
						args.outputName	= parameters.outputName;

						args.entryPoint	= functionNames[ type ];
						args.version	= shaderStart[ type ] + "_4_0";
						args.debugMode	= debugMode;

						args.defineCode = m_pBaseSourceCode;
						args.defineCode += variant.defineCode;

						for( uint defineTypeIndex = 1u; defineTypeIndex < ShaderType_Count; ++defineTypeIndex )
						{
							args.defineCode	+= formatString( "#define %s %s\n", shaderDefine[ defineTypeIndex ].cStr(), ( typeIndex == defineTypeIndex ? "TIKI_ON" : "TIKI_OFF" ) );
						}

						Array< uint8 > variantData;
						if ( compilePlatformShader( variantData, args, includeHandler, definition.getGraphicsApi() ) )
						{
							uint32 keyData[] = { (uint32)type, variant.bitMask };

							ShaderVariantData& variantVarName = shaderVariants.add();
							variantVarName.type			= type;
							variantVarName.codeLength	= uint32( variantData.getCount() );
							variantVarName.variantKey	= crcBytes( keyData, sizeof( keyData ) );

							writer.openDataSection( 0u, AllocatorType_MainMemory );
							variantVarName.key = writer.addDataPoint();
							writer.writeData( variantData.getBegin(), variantData.getCount() );
							writer.closeDataSection();

							variantData.dispose();
						}
					}
				}

				writer.openDataSection( 0u, AllocatorType_InitializaionMemory );

				writer.writeUInt32( uint32( shaderVariants.getCount() ) );
				writer.writeAlignment( 8u );

				for( uint variantIndex = 0u; variantIndex < shaderVariants.getCount(); ++variantIndex )
				{
					const ShaderVariantData& shaderVarName = shaderVariants[ variantIndex ];

					writer.writeUInt32( shaderVarName.type );
					writer.writeUInt32( shaderVarName.codeLength );
					writer.writeUInt32( shaderVarName.variantKey );
					writer.writeReference( &shaderVarName.key );
				}

				writer.closeDataSection();

				writer.closeResource();
			}

			closeResourceWriter( writer );

			preprocessor.dispose();
		}

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

		const string sourceCode = args.defineCode + formatString( "\n#define TIKI_HLSL4 TIKI_ON\n#define TIKI_OPENGL4 TIKI_OFF\n#include \"%s\"", args.fileName.cStr() );

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
				string error = (const char*)pErrorBlob->GetBufferPointer();
				TIKI_TRACE_ERROR( "failed to compile shader. error message:\n%s\n", error.cStr() );
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

		string sourceCode = "#version 440\n" + args.defineCode + formatString( "\n#define TIKI_HLSL4 TIKI_OFF\n#define TIKI_OPENGL4 TIKI_ON\n#include \"%s\"\n", args.fileName.cStr() );
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
				const string slotIndex = string( pArgumentBegin, length );

				regex.GetSubExp( 2, &pArgumentBegin, &length );
				const string name = string( pArgumentBegin, length );

				ShaderConstantInfo& info = constants.add();
				info.slotIndex	= ParseString::parseInt32( slotIndex.cStr() );
				info.name		= name;

				const uint startIndex	= pSearchBegin - sourceCode.cStr();
				const uint length2		= pSearchEnd - pSearchBegin;
				sourceCode = sourceCode.remove( startIndex, length2 );

				const string newCode = formatString( "layout(shared) uniform %s", name.cStr() );
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

		for (uint i = 0u; i < constantCount; ++i)
		{
			const ShaderConstantInfo& info = constants[ i ];
			const uint32 nameLength = (uint32)info.name.getLength();

			stream.write( &info.slotIndex, sizeof( info.slotIndex ) );
			stream.write( &nameLength, sizeof( nameLength ) );
			stream.write( info.name.cStr(), nameLength + 1u );
		}

		stream.write( sourceCode.cStr(), sourceCode.getLength() + 1u );

		m_openGlMutex.unlock();

		return targetData.create( static_cast< const uint8* >( stream.getData() ), stream.getLength() );
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

	//string preprocessSourceCode( const string& sourceCode, const string& fileName, const ShaderIncludeHandler* pIncludeHandler )
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

	//	for (uint i = 0u; i < pIncludeHandler->getIncludeDirs().getCount(); ++i)
	//	{
	//		pCurrentTag->tag	= FPPTAG_INCLUDE_DIR;
	//		pCurrentTag->data	= (void*)pIncludeHandler->getIncludeDirs()[ i ].cStr();
	//		pCurrentTag++;
	//	}

	//	const string sourceFileName = path::getFilename( fileName );
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
	//	string resultSourceCode = context.pTargetData;

	//	TIKI_FREE( context.pTargetData );

	//	return resultSourceCode;
	//}
}
