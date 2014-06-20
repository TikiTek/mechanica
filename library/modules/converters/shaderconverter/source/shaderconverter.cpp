
#include "tiki/shaderconverter/shaderconverter.hpp"

#include "tiki/base/array.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/file.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/iopath.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/convertermanager.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/graphics/shadertype.hpp"
#include "tiki/io/filestream.hpp"
#include "tiki/shaderconverter/shaderpreprocessor.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include <d3dcompiler.h>
#endif

#include "ShaderLang.h"
#include "fpp.h"
#include "trexpp.h"

namespace tiki
{
	struct ShaderVariantData
	{
		ShaderType		type;
		crc32			variantKey;
		uint32			codeLength;
		ReferenceKey	key;
	};

	class ShaderIncludeHandler : public ID3DInclude
	{
		TIKI_NONCOPYABLE_CLASS( ShaderIncludeHandler );

	public:

		ShaderIncludeHandler( ConverterManager* pManager )
		{
			TIKI_ASSERT( pManager != nullptr );
			m_pManager = pManager;

			m_includeDirs.add( "./" );

			string text;
			if ( file::readAllText( "../../shaderinc.lst", text ) )
			{
				Array< string > dirs;
				text.split( dirs, "\n" );

				for (uint i = 0u; i < dirs.getCount(); ++i)
				{
					m_includeDirs.add( dirs[ i ].trim() );
				} 
				
				dirs.dispose();
			}
		}

		~ShaderIncludeHandler()
		{
			for (uint i = 0u; i < TIKI_COUNT( m_fileData ); ++i)
			{
				if ( m_fileData[ i ].getCount() > 0u )
				{
					m_fileData[ i ].dispose();
				}
			}

			m_includeDirs.dispose();
		}

		virtual HRESULT	STDMETHODCALLTYPE Open( D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes )
		{
			const string inputFilename = pFileName;

			bool found = false;
			string fullName = inputFilename;

			if ( file::exists( fullName ) )
			{
				found = true;
			}
			else
			{
				for (uint i = 0u; i < m_includeDirs.getCount(); ++i)
				{
					fullName = path::combine( m_includeDirs[ i ], inputFilename );

					if ( file::exists( fullName ) )
					{
						found = true;
						break;
					}
				} 
			}
			
			if ( found )
			{	
				m_pManager->addDependency( ConversionResult::DependencyType_File, fullName, "", 0u );

				uint freeStreamIndex = TIKI_SIZE_T_MAX;
				for (uint j = 0u; j < TIKI_COUNT( m_fileData ); ++j)
				{
					if ( m_fileData[ j ].getCount() == 0u )
					{
						freeStreamIndex = j;
						break;
					}
				} 

				if ( freeStreamIndex == TIKI_SIZE_T_MAX )
				{
					TIKI_TRACE_ERROR( "No free Filestream.\n" );
					return S_FALSE;
				}

				if ( file::readAllBytes( fullName, m_fileData[ freeStreamIndex ] ) == false )
				{
					TIKI_TRACE_ERROR( "Could not read File: %s.\n", fullName.cStr() );
					return S_FALSE;
				}

				*ppData = m_fileData[ freeStreamIndex ].getBegin();
				*pBytes	= UINT( m_fileData[ freeStreamIndex ].getCount() );
				return S_OK;
			}

			TIKI_TRACE_ERROR( "Could find File: %s.\n", pFileName );
			return S_FALSE;
		}

		virtual HRESULT	STDMETHODCALLTYPE Close( LPCVOID pData )
		{
			for (uint i = 0u; i < TIKI_COUNT( m_fileData ); ++i)
			{
				if ( m_fileData[ i ].getBegin() == pData )
				{
					m_fileData[ i ].dispose();
					return S_OK;
				}
			} 

			return S_FALSE;
		}

		const List< string > getIncludeDirs() const
		{
			return m_includeDirs;
		}

	private:

		enum
		{
			MaxFileStreams = 8u
		};

		ConverterManager*	m_pManager;

		List< string >		m_includeDirs;
		Array< uint8 >		m_fileData[ MaxFileStreams ];

	};

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

	crc32 ShaderConverter::getInputType() const
	{
		return crcString( "shader" );
	}

	crc32 ShaderConverter::getOutputType() const
	{
		return crcString( "shader" );
	}

	void ShaderConverter::getDependingType( List< crc32 >& types ) const
	{
	}

	bool ShaderConverter::initializeConverter()
	{
		m_baseSourceCode =	"#define TIKI_ON 2-\n"
							"#define TIKI_OFF 1-\n"
							"#define TIKI_ENABLED( value ) ( ( value 0 ) == 2 )\n"
							"#define TIKI_DISABLED( value ) ( ( value 0 ) != 2 )\n\n";

		m_pIncludeHandler = TIKI_NEW ShaderIncludeHandler( getManager() );

		return true;
	}

	void ShaderConverter::disposeConverter()
	{
		TIKI_DEL m_pIncludeHandler;
	}
	
	bool ShaderConverter::startConversionJob( const ConversionParameters& params ) const
	{
		const string shaderStart[]	= { "fx", "vs", "ps", "gs", "hs", "ds", "cs" };
		const string shaderDefine[]	= { nullptr, "TIKI_VERTEX_SHADER", "TIKI_PIXEL_SHADER", "TIKI_GEOMETRY_SHADER", "TIKI_HULL_SHADER", "TIKI_DOMAIN_SHADER", "TIKI_COMPUTE_SHADER" };

		string functionNames[ ShaderType_Count ];
		for (uint i = 0u; i < TIKI_COUNT( shaderStart ); ++i)
		{
			functionNames[ i ] = params.arguments.getOptionalString( shaderStart[ i ] + "_function_name", "main" );
		}
		
		for (uint i = 0u; i < params.inputFiles.getCount(); ++i)
		{
			const ConversionParameters::InputFile& file = params.inputFiles[ i ];

			string sourceCode;
			if ( file::readAllText( file.fileName, sourceCode ) == false )
			{
				TIKI_TRACE_ERROR( "Can't open file.\n" );
				continue;
			}

			const bool debugMode = params.arguments.getOptionalBool( "compile_debug", false );

			ShaderPreprocessor preprocessor;
			preprocessor.create( sourceCode );

			ResourceWriter writer;
			openResourceWriter( writer, params.outputName, "shader", params.targetPlatform );
			writer.openResource( params.outputName + ".shader", TIKI_FOURCC( 'T', 'G', 'S', 'S' ), getConverterRevision() );

			List< ShaderVariantData > shaderVariants;
			for (uint i = 1u; i < ShaderType_Count; ++i)
			{
				const ShaderType type = (ShaderType)i;

				if ( preprocessor.isTypeEnabled( type ) == false )
				{
					continue;
				}

				const uint variantCount = preprocessor.getVariantCount( type );
				for (uint j = 0u; j < variantCount; ++j)
				{
					const ShaderVariant& variant = preprocessor.getVariantByIndex( type, j );

					ShaderArguments args;
					args.type		= type;

					args.fileName	= file.fileName;
					args.outputName	= params.outputName;

					args.entryPoint	= functionNames[ type ];
					args.version	= shaderStart[ type ] + "_4_0";
					args.debugMode	= debugMode;

					args.defineCode = m_baseSourceCode;
					args.defineCode += variant.defineCode;

					for (uint k = 1u; k < ShaderType_Count; ++k)
					{
						args.defineCode	+= formatString( "#define %s %s\n", shaderDefine[ k ].cStr(), ( i == k ? "TIKI_ON" : "TIKI_OFF" ) );
					} 

					Array< uint8 > variantData;
					if ( compilePlatformShader( variantData, args, GraphicsApi_OpenGL4 ) )
					{
						uint32 keyData[] = { type, variant.bitMask };

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

			for (uint i = 0u; i < shaderVariants.getCount(); ++i)
			{
				const ShaderVariantData& shaderVarName = shaderVariants[ i ];
				writer.writeUInt32( shaderVarName.type );
				writer.writeUInt32( shaderVarName.codeLength );
				writer.writeUInt32( shaderVarName.variantKey );
				writer.writeReference( &shaderVarName.key );
			}

			writer.closeDataSection();

			writer.closeResource();
			closeResourceWriter( writer );

			preprocessor.dispose();
		}

		return true;
	}

	bool ShaderConverter::compilePlatformShader( Array< uint8 >& targetData, const ShaderArguments& args, GraphicsApi targetApi ) const
	{
		switch ( targetApi )
		{
		case GraphicsApi_D3D11:
			return compileD3d11Shader( targetData, args );

		case GraphicsApi_OpenGL4:
			return compileOpenGl4Shader( targetData, args );

		default:
			TIKI_TRACE_ERROR( "Graphics API not supported.\n" );
			break;
		}

		return false;
	}

	bool ShaderConverter::compileD3d11Shader( Array< uint8 >& targetData, const ShaderArguments& args ) const
	{
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
			m_pIncludeHandler,
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
	}

	bool ShaderConverter::compileOpenGl4Shader( Array< uint8 >& targetData, const ShaderArguments& args ) const
	{
		string sourceCode = "#version 440\n" + args.defineCode + formatString( "\n#define TIKI_HLSL4 TIKI_OFF\n#define TIKI_OPENGL4 TIKI_ON\n#include \"%s\"\n", args.fileName.cStr() );

		// preprocessor
		{
			fppTag aTags[ 64u ];
			fppTag* pCurrentTag = aTags;

			FppContext context;
			context.pSourceData		= sourceCode.cStr();
			context.sourceLength	= sourceCode.getLength();
			context.sourcePosition	= 0;
			context.pTargetData		= (char*)TIKI_MEMORY_ALLOC( 1024u * 1024u );
			context.targetLength	= 1024u * 1024u;
			context.targetPosition	= 0u;

			for (uint i = 0u; i < m_pIncludeHandler->getIncludeDirs().getCount(); ++i)
			{
				pCurrentTag->tag	= FPPTAG_INCLUDE_DIR;
				pCurrentTag->data	= (void*)m_pIncludeHandler->getIncludeDirs()[ i ].cStr();
				pCurrentTag++;
			}

			const string fileName = path::getFilename( args.fileName );
			pCurrentTag->tag	= FPPTAG_INPUT_NAME;
			pCurrentTag->data	= (void*)fileName.cStr();
			pCurrentTag++;

			pCurrentTag->tag	= FPPTAG_INPUT;
			pCurrentTag->data	= (void*)fppRead;
			pCurrentTag++;

			pCurrentTag->tag	= FPPTAG_OUTPUT;
			pCurrentTag->data	= (void*)fppWrite;
			pCurrentTag++;

			pCurrentTag->tag	= FPPTAG_ERROR;
			pCurrentTag->data	= (void*)fppError;
			pCurrentTag++;

			pCurrentTag->tag	= FPPTAG_USERDATA;
			pCurrentTag->data	= (void*)&context;
			pCurrentTag++;

			pCurrentTag->tag	= FPPTAG_LINE;
			pCurrentTag->data	= (void*)FALSE;
			pCurrentTag++;
						
			pCurrentTag->tag	= FPPTAG_IGNOREVERSION;
			pCurrentTag->data	= (void*)FALSE;
			pCurrentTag++;

			pCurrentTag->tag	= FPPTAG_END;
			pCurrentTag->data	= nullptr;
			pCurrentTag++;

			if ( fppPreProcess( aTags ) != 0 )
			{
				TIKI_TRACE_INFO( "[shaderconverter] preprocessor failed.\n" );
			}

			context.pTargetData[ context.targetPosition ] = '\0';
			sourceCode = context.pTargetData;

			TIKI_MEMORY_FREE( context.pTargetData );
		}

		// parse code
		if ( false )
		{
			static const EShLanguage s_aLanguageMapping[] =
			{
				EShLangCount,			// ShaderType_Effect
				EShLangVertex,			// ShaderType_VertexShader
				EShLangFragment,		// ShaderType_PixelShader
				EShLangGeometry,		// ShaderType_GeometrieShader
				EShLangTessControl,		// ShaderType_HullShader
				EShLangTessEvaluation,	// ShaderType_DomainShader
				EShLangCompute			// ShaderType_ComputeShader
			};
			TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aLanguageMapping ) == ShaderType_Count );

			glslang::TShader shaderTest( s_aLanguageMapping[ args.type ] );

			const char* pSourceCodeString = sourceCode.cStr();
			shaderTest.setStrings( &pSourceCodeString, 1u );

			TBuiltInResource shaderResources = { 0 };
			if ( !shaderTest.parse( &shaderResources, 400, true, EShMsgDefault ) )
			{
				TIKI_TRACE_INFO( "[shaderconverter] parser has return an error.\n" );
				//TIKI_TRACE_INFO( sourceCode.cStr() );
				//TIKI_TRACE_INFO( shaderTest.getInfoLog() );				
			}
		}

		targetData.create(
			(const uint8*)sourceCode.cStr(),
			sourceCode.getLength() + 1u
		);

		return true;
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
}