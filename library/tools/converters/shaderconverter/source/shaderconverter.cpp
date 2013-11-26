
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
#include "tiki/graphicsbase/shadertype.hpp"
#include "tiki/io/filestream.hpp"
#include "tiki/shaderconverter/shaderpreprocessor.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include <d3dcompiler.h>
#endif

namespace tiki
{
	struct ShaderVariantData
	{
		ShaderType		type;
		crc32			variantKey;
		uint			codeLength;
		ReferenceKey	key;
	};

	class ShaderIncludeHandler : public ID3DInclude
	{
		TIKI_NONCOPYABLE_CLASS( ShaderIncludeHandler );

	public:

		ShaderIncludeHandler()
		{
			m_includeDirs.add( "./" );

			string text;
			if ( file::readAllText( "../../shaderinc.lst", text ) )
			{
				Array< string > dirs;
				text.split( dirs, "\n" );

				m_includeDirs.addRange( dirs.getData(), dirs.getCount() );
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
			for (uint i = 0u; i < m_includeDirs.getCount(); ++i)
			{
				const string fullName = path::combine( m_includeDirs[ i ], pFileName );

				if ( file::exists( fullName ) )
				{			
					size_t freeStreamIndex = TIKI_SIZE_T_MAX;
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

					*ppData = m_fileData[ freeStreamIndex ].getData();
					*pBytes	= m_fileData[ freeStreamIndex ].getCount();
					return S_OK;
				}
			} 

			TIKI_TRACE_ERROR( "Could find File: %s.\n", pFileName );
			return S_FALSE;
		}

		virtual HRESULT	STDMETHODCALLTYPE Close( LPCVOID pData )
		{
			for (uint i = 0u; i < TIKI_COUNT( m_fileData ); ++i)
			{
				if ( m_fileData[ i ].getData() == pData )
				{
					m_fileData[ i ].dispose();
					return S_OK;
				}
			} 

			return S_FALSE;
		}

	private:

		enum
		{
			MaxFileStreams = 8u
		};

		List< string >	m_includeDirs;
		Array< uint8 >	m_fileData[ MaxFileStreams ];

	};


	tiki::crc32 ShaderConverter::getInputType() const
	{
		return crcString( "shader" );
	}

	tiki::crc32 ShaderConverter::getOutputType() const
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

		m_pIncludeHandler = TIKI_NEW ShaderIncludeHandler();

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
		for (size_t i = 0u; i < TIKI_COUNT( shaderStart ); ++i)
		{
			functionNames[ i ] = params.arguments.getOptionalString( shaderStart[ i ] + "_function_name", "main" );
		}

		for (size_t i = 0u; i < params.inputFiles.getCount(); ++i)
		{
			const ConversionInputFile& file = params.inputFiles[ i ];

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
			openResourceWriter( &writer, params.outputName, "shader", params.targetPlatform );
			writer.openResource( params.outputName + ".shader", TIKI_FOURCC( 'T', 'G', 'F', 'X' ), getConverterRevision() );

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
					args.version	= shaderStart[ type ] + "_5_0";
					args.debugMode	= debugMode;

					args.defineCode = m_baseSourceCode;
					args.defineCode += variant.defineCode;

					for (uint k = 1u; k < ShaderType_Count; ++k)
					{
						args.defineCode	+= formatString( "#define %s %s\n", shaderDefine[ k ].cStr(), ( i == k ? "TIKI_ON" : "TIKI_OFF" ) );
					} 

					Array< uint8 > variantData;
					if ( compilePlatformShader( variantData, args, GraphicsApi_D3D11 ) )
					{
						ShaderVariantData& variantVarName = shaderVariants.add();
						variantVarName.type			= type;
						variantVarName.codeLength	= variantData.getCount();
						variantVarName.variantKey	= variant.bitMask;

						writer.openDataSection( 0u, AllocatorType_MainMemory );
						variantVarName.key = writer.addDataPoint();
						writer.writeData( variantData.getData(), variantData.getCount() );
						writer.closeDataSection();

						variantData.dispose();
					}
				} 
			}

			writer.openDataSection( 0u, AllocatorType_InitializaionMemory );

			writer.writeSInt32( shaderVariants.getCount() );
			for (uint i = 0u; i < shaderVariants.getCount(); ++i)
			{
				const ShaderVariantData& shaderVarName = shaderVariants[ i ];
				writer.writeSInt16( shaderVarName.type );
				writer.writeSInt16( (uint16)shaderVarName.codeLength );
				writer.writeSInt32( shaderVarName.variantKey );
				writer.writeReference( shaderVarName.key );
			}

			writer.closeDataSection();

			writer.closeResource();
			closeResourceWriter( &writer );

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

		size_t shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
		if ( args.debugMode )
		{
			shaderFlags |= D3DCOMPILE_DEBUG;
		}

		const string sourceCode = args.defineCode + formatString( "\n#include \"%s\"", args.fileName.cStr() );

		HRESULT result = D3DCompile(
			sourceCode.cStr(),
			sourceCode.getLength(),
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
				TIKI_TRACE_ERROR( "failed to compile shader. error message: %s\n", error.cStr() );
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
		return false;
	}
}