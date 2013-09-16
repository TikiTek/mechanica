
#include "tiki/shaderconverter/shaderconverter.hpp"

#include "tiki/base/array.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/convertermanager.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/graphicsbase/shadertype.hpp"
#include "tiki/toolbase/filestream.hpp"

#if TIKI_PLATFORM_WIN
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

		string functionNames[ ShaderType_Count ];
		for (size_t i = 0u; i < ShaderType_Count; ++i)
		{
			functionNames[ i ] = shaderStart[ i ].toUpper() + "_Main";
		}

		for (auto it = params.arguments.getMap().begin(); it != params.arguments.getMap().end(); it++)
		{
			if ( it->first.endsWith( "_function_name" ) )
			{
				for (size_t i = 1u; i < ShaderType_Count; ++i)
				{
					if ( it->first.toLower().startsWith( shaderStart[ i ] ) )
					{
						functionNames[ i ] = it->second;
						break;
					}
				}
			}
			else
			{
				TIKI_TRACE_WARNING( "unknown argument: %s\n", it->first.cStr() );
			}
		}

		for (size_t i = 0u; i < params.inputFiles.getCount(); ++i)
		{
			const ConversionInputFile& file = params.inputFiles[ i ];

			const char* pFunctionName	= nullptr;
			ShaderType type				= ShaderType_Effect;
			for (size_t i = 1u; i < ShaderType_Count; ++i)
			{
				if ( file.typeString.toLower().startsWith( shaderStart[ i ] ) )
				{
					type			= (ShaderType)i;
					pFunctionName	= functionNames[ i ].cStr();
					break;
				}
			}

			FileStream stream;
			stream.create( file.fileName, FOM_Read );

			Array< char > data;
			data.create( stream.getLength() );
			stream.read( data.getData(), data.getCount() );
			stream.dispose();
			
#if TIKI_PLATFORM_WIN
			ID3D10Blob* pBlob		= nullptr;
			ID3D10Blob* pErrorBlob	= nullptr;

			size_t shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

			if ( params.arguments.getOptionalBool( "compile_debug", false ) )
			{
				shaderFlags |= D3DCOMPILE_DEBUG;
			}

			HRESULT r = D3DCompile(
				data.getData(),
				data.getCount(),
				file.fileName.cStr(), 
				nullptr,
				nullptr,
				pFunctionName,
				file.typeString.cStr(), 
				shaderFlags,
				0,
				&pBlob,
				&pErrorBlob
			);

			data.dispose();

			if ( FAILED( r ) )
			{
				if ( r == ERROR_FILE_NOT_FOUND )
				{
					TIKI_TRACE_ERROR( "impossible file not found: %s\n", file.fileName.cStr() );
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
				openResourceWriter( &writer, TIKI_FOURCC( 'T', 'G', 'F', 'X' ), params.outputName, file.typeString );

				writer.writeUInt32( type );
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
#else
			ResourceWriter writer;
			openResourceWriter( &writer, TIKI_FOURCC( 'T', 'G', 'F', 'X' ), params.outputName, file.typeString );

			writer.writeUInt32( type );
			writer.writeUInt32( data.getCount() );
			writer.writeData( data.getData(), data.getCount() );

			closeResourceWriter( &writer );
#endif
		}

		return true;
	}
}