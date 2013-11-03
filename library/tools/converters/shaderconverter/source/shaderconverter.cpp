
#include "tiki/shaderconverter/shaderconverter.hpp"

#include "tiki/base/array.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/file.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/convertermanager.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/graphicsbase/shadertype.hpp"
#include "tiki/shaderconverter/shaderpreprocessor.hpp"
#include "tiki/toolbase/filestream.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include <d3dcompiler.h>
#endif

namespace tiki
{
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
		return true;
	}

	void ShaderConverter::disposeConverter()
	{
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

					args.sourceCode	= formatString( "#define %s TIKI_ON\n", shaderDefine[ type ].cStr() );
					args.sourceCode += variant.defineCode;
					args.sourceCode += preprocessor.getSourceCode();

					Array< uint8 > variantData;
					compilePlatformShader( variantData, args, GraphicsApi_D3D11 );
				} 
			}

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

		HRESULT result = D3DCompile(
			args.sourceCode.cStr(),
			args.sourceCode.getLength(),
			args.fileName.cStr(), 
			nullptr,
			nullptr,
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
		}
		else
		{
			ResourceWriter writer;
			openResourceWriter( &writer, TIKI_FOURCC( 'T', 'G', 'F', 'X' ), args.outputName, args.version.substring( 0u, 2u ) );

			writer.writeUInt32( args.type );
			writer.writeUInt32( pBlob->GetBufferSize() );
			writer.writeData( pBlob->GetBufferPointer(), pBlob->GetBufferSize() );

			closeResourceWriter( &writer );
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