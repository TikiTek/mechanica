
#include "tiki/shaderconverter/shaderpreprocessor.hpp"

#include "tiki/base/file.hpp"
#include "tiki/base/iopath.hpp"
#include "tiki/base/stringparse.hpp"
#include "tiki/toolbase/list.hpp"

#include "trexpp.h"

namespace tiki
{
	struct ShaderFeature
	{
		string	name;

		size_t	startBit;
		size_t	bitCount;

		size_t	maxValue;			
	};

	static void addFeature( List< ShaderFeature >& features, const string& name, size_t maxValue )
	{
		const ShaderFeature* pLastFeature = nullptr;
		if ( features.getCount() > 0u )
		{
			pLastFeature = features.getLast();
		}

		ShaderFeature& feature = features.add();
		feature.name		= name;
		feature.bitCount	= 64u - countLeadingZeros64( maxValue );
		feature.maxValue	= maxValue;

		if ( pLastFeature != nullptr )
		{
			feature.startBit = pLastFeature->startBit + pLastFeature->bitCount;
		}
		else
		{
			feature.startBit = 0u;
		}
	}

	static void parseShaderFeatures( bool* pShaderEnabled, List< ShaderFeature >* pShaderFeatures, const cstring* pShaderTypes, size_t typeCount, const string& featuresLine )
	{
		for (size_t i = 0u; i < typeCount; ++i)
		{
			const string search = formatString( "%s-features=", pShaderTypes[ i ] );

			const int index = featuresLine.indexOf( search );
			pShaderEnabled[ i ] = ( index >= 0 );

			if ( index != -1 )
			{
				const int baseIndex = index + search.getLength();
				const int length = featuresLine.indexOf( ' ', baseIndex ) - baseIndex;
				const string featuresString = featuresLine.substring( baseIndex, length );

				Array< string > featuresList;
				featuresString.trim().split( featuresList, "," );

				for (size_t j = 0u; j < featuresList.getCount(); ++j)
				{
					string name = featuresList[ j ];
					size_t maxValue = 1u;

					const int arrayIndex = name.indexOf( '[' );
					if ( arrayIndex != -1 )
					{
						const int arrayLength = name.indexOf( ']' ) - arrayIndex;
						if ( arrayLength < 0u )
						{
							TIKI_TRACE_ERROR( "invalid shader feature name: %s. feature will be ignored.\n", name.cStr() );
							continue;
						}

						const string maxValueString = name.substring( arrayIndex, arrayLength );
						maxValue	= ParseString::parseUInt32( maxValueString );
						name		= name.substring( 0u, arrayIndex - 1u );
					}

					if ( i == 0u ) // fx feature will be applied to all shaders in file
					{
						for (size_t k = 1u; k < typeCount; ++k)
						{
							addFeature( pShaderFeatures[ i ], name, maxValue );
						}
					}

					addFeature( pShaderFeatures[ i ], name, maxValue );
				}

				featuresList.dispose();
			}
		}
	}

	static string resolveIncludes( const string& shaderCode, const List< string >& includeDirs )
	{
		string resultCode = shaderCode;

		TRexpp regex;
		regex.Compile( "^\\s*#include\\s+(<([^\"'<>|\\b]+)>|\"([^\"'<>|\\b]+)\")" );

		cstring beginPath;
		cstring endPath;
		while ( regex.Search( resultCode.cStr(), &beginPath, &endPath ) )
		{
			string path = string( beginPath, endPath - beginPath );
			
			for (size_t i = 0u; i < includeDirs.getCount(); ++i)
			{
				const string fullPath = path::combine( includeDirs[ i ], path );

				if ( file::exists( fullPath ) )
				{
					path = fullPath;
					break;
				}
			}

			if ( !file::exists( path ) )
			{
				TIKI_TRACE_ERROR( "include file not found: %s\n", path.cStr() );
				return nullptr;
			}

			Array< uint8 > fileData;
			if ( !file::readAllBytes( path, fileData ) )
			{
				TIKI_TRACE_ERROR( "open include file failed: %s\n", path.cStr() );
				return nullptr;
			}

			const string fileText = string( reinterpret_cast< const char* >( fileData.getData() ) );			
			resultCode = resultCode.replace( path, fileText );
		}

		return resultCode;
	}

	void ShaderPreprocessor::create( const string& shaderText )
	{
		// resolve includes
		List< string > includeDirs;
		
		includeDirs.add( "../../../../../../library/modules/runtime/graphics/include/tiki/graphics/shader" );

		m_sourceCode = resolveIncludes( shaderText, includeDirs );

		// parse features
		const string featuresLine = shaderText.substring( 0u, shaderText.indexOf( '\n' ) );

		if ( featuresLine.startsWith( "//" ) == false )
		{
			TIKI_TRACE_ERROR( "no feature list found. shader can't converted.\n" );
			return;
		}
		
		const cstring shaderStart[]	= { "fx", "vs", "ps", "gs", "hs", "ds", "cs" };
		bool shaderEnabled[ TIKI_COUNT( shaderStart ) ];
		List< ShaderFeature > shaderFeatures[ TIKI_COUNT( shaderStart ) ];
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( shaderStart ) == ShaderType_Count );

		parseShaderFeatures( shaderEnabled, shaderFeatures, shaderStart, TIKI_COUNT( shaderStart ), featuresLine );

		for (size_t i = 0u; i < TIKI_COUNT( shaderStart ); ++i)
		{
			if ( shaderEnabled[ i ] && shaderFeatures[ i ].getCount() > 0u )
			{
				List< ShaderVariant > variants;

				for (size_t j = 0u; j < shaderFeatures[ i ].getCount(); ++j)
				{
					const ShaderFeature& feature = shaderFeatures[ i ][ j ];
					List< ShaderVariant > featureVariants;

					for (size_t l = 0u; l < feature.maxValue; ++l)
					{
						const size_t bitMask = l << feature.startBit;

						for (size_t k = 0u; k < variants.getCount(); ++k)
						{
							ShaderVariant variant = variants[ k ];
							variant.bitMask |= bitMask;
							
							featureVariants.add( variant );
						}
					}

					variants.addRange( featureVariants );
				}

				m_variants[ i ].create( variants.getData(), variants.getCount() );
			}
			else if ( shaderEnabled[ i ] )
			{
				ShaderVariant variant;
				variant.bitMask = 0u;

				m_variants[ i ].create( &variant, 1u );
			}
		}
	}

	void ShaderPreprocessor::dispose()
	{
		for (size_t i = 0u; i < TIKI_COUNT( m_variants ); ++i)
		{
			m_variants[ i ].dispose();
		}
	}
}