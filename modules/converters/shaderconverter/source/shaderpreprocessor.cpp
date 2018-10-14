
#include "tiki/shaderconverter/shaderpreprocessor.hpp"

#include "tiki/base/bits.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/container/list.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/path.hpp"

#include "TRexpp.h"
#include <pcre2.h>

namespace tiki
{
	struct ShaderFeature
	{
		string	name;

		uint32	startBit;
		uint32	bitCount;

		uint32	maxValue;
	};

	static void addFeature( List< ShaderFeature >& features, const string& name, uint32 maxValue )
	{
		const ShaderFeature* pLastFeature = nullptr;
		if ( features.getCount() > 0u )
		{
			pLastFeature = &features.getLast();
		}

		ShaderFeature feature;
		feature.name		= name;
		feature.bitCount	= uint32( 64u - countLeadingZeros64( maxValue ) );
		feature.maxValue	= maxValue;

		if ( pLastFeature != nullptr )
		{
			feature.startBit = pLastFeature->startBit + pLastFeature->bitCount;
		}
		else
		{
			feature.startBit = 0u;
		}

		features.add( feature );
	}

	static void parseShaderFeatures( bool* pShaderEnabled, List< ShaderFeature >* pShaderFeatures, const char** pShaderTypes, uint typeCount, const string& featuresLine )
	{
		for (uint i = 0u; i < typeCount; ++i)
		{
			const string search = formatDynamicString( "%s-features=", pShaderTypes[ i ] );

			const int index = featuresLine.indexOf( search );
			pShaderEnabled[ i ] = ( index >= 0 );

			if ( index != -1 )
			{
				const int baseIndex = int( index + search.getLength() );
				const int length = featuresLine.indexOf( ' ', baseIndex ) - baseIndex;
				const string featuresString = featuresLine.subString( baseIndex, length );

				Array< string > featuresList;
				featuresString.trim().split( featuresList, "," );

				for (uint j = 0u; j < featuresList.getCount(); ++j)
				{
					string name = featuresList[ j ];
					uint32 maxValue = 1u;

					int arrayIndex = name.indexOf( '[' );
					if ( arrayIndex != -1 )
					{
						arrayIndex++;
						const int arrayLength = name.indexOf( ']' ) - arrayIndex;
						if ( arrayLength < 0u )
						{
							TIKI_TRACE_ERROR( "invalid shader feature name: %s. feature will be ignored.\n", name.cStr() );
							continue;
						}

						const string maxValueString = name.subString( arrayIndex, arrayLength );
						maxValue	= string_tools::parseUInt32( maxValueString.cStr() ) - 1u;
						name		= name.subString( 0u, arrayIndex - 1 );
					}

					if ( i == 0u ) // fx feature will be applied to all shaders in file
					{
						for (uint k = 1u; k < typeCount; ++k)
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

		int errorCode;
		PCRE2_SIZE errorOffset;
		pcre2_code* pExpression = pcre2_compile( (PCRE2_SPTR8)"^\\s*#\\s*include\\s+[<\"](.+)[>\"]\\s*$", PCRE2_ZERO_TERMINATED, PCRE2_MULTILINE, &errorCode, &errorOffset, nullptr );
		if( !pExpression )
		{
			TIKI_TRACE_ERROR( "failed to compile RegEx. Error: %d\n", errorCode );
			return "";
		}
		pcre2_match_data* pMatchData = pcre2_match_data_create_from_pattern( pExpression, nullptr );

		Path lastPath;
		while( pcre2_match( pExpression, (PCRE2_SPTR8)resultCode.cStr(), PCRE2_ZERO_TERMINATED, 0u, 0u, pMatchData, nullptr ) > 1 )
		{
			const uint32 offsetCount = pcre2_get_ovector_count( pMatchData );
			if( offsetCount < 2u )
			{
				TIKI_TRACE_ERROR( "not enought offset to replace include: %d\n", offsetCount );
				return "";
			}

			const PCRE2_SIZE* pOffsets = pcre2_get_ovector_pointer( pMatchData );

			PCRE2_SIZE expBeginIndex = pOffsets[ 0u ];
			PCRE2_SIZE expLength = pOffsets[ 1u ] - pOffsets[ 0u ];

			const char* pMatchBegin = resultCode.cStr() + pOffsets[ 2u ];
			PCRE2_SIZE matchLength = pOffsets[ 3u ] - pOffsets[ 2u ];

			string path = string( pMatchBegin, matchLength );
			for (uint i = 0u; i < includeDirs.getCount(); ++i)
			{
				const string fullPath = path::combine( includeDirs[ i ], path );
				if ( file::exists( fullPath.cStr() ) )
				{
					path = fullPath;
					break;
				}
			}

			if ( !file::exists( path.cStr() ) )
			{
				const string fullPath = path::combine( lastPath.getDirectoryWithPrefix(), path );
				if( !file::exists( fullPath.cStr() ) )
				{
					TIKI_TRACE_ERROR( "include file not found: %s\n", path.cStr() );
					resultCode = resultCode.remove( expBeginIndex, uint( expLength ) );
					continue;
				}
				else
				{
					path = fullPath;
				}
			}

			lastPath.setCompletePath( path.cStr() );

			Array< uint8 > fileData;
			if ( !file::readAllBytes( path.cStr(), fileData ) )
			{
				TIKI_TRACE_ERROR( "open include file failed: %s\n", path.cStr() );
				return "";
			}

			string fileText = string( reinterpret_cast< const char* >( fileData.getBegin() ), fileData.getCount() );
			//fileText = fileText.replace( "\r\n", "\n" );

			resultCode = resultCode.remove( expBeginIndex, uint( expLength ) );
			resultCode = resultCode.insert( fileText, expBeginIndex );

			//resultCode = resultCode.cStr() + resultCode.indexOf( '#' );

			fileData.dispose();
		}

		pcre2_match_data_free( pMatchData );
		pcre2_code_free( pExpression );
		return resultCode;
	}

	static string createDefineString( List< ShaderFeature >& features, uint32 bitMask )
	{
		string defineString = "";

		for (uint i = 0u; i < features.getCount(); ++i)
		{
			const ShaderFeature& feature = features[ i ];
			const uint value = getBitValue( bitMask, feature.startBit, feature.bitCount );

			defineString += formatDynamicString( "#define %s %u\n", feature.name.cStr(), value );
		}

		return defineString;
	}

	void ShaderPreprocessor::create( const string& shaderText, const string& projectDir, const string& assetDir )
	{
		// resolve includes
		List< string > includeDirs;
		includeDirs.add( assetDir );

		Path additionalPath( projectDir.cStr() );
		additionalPath.push( "library/modules/runtime/graphics/include" );
		includeDirs.add( additionalPath.getCompletePath() );

		additionalPath.setCombinedPath( projectDir.cStr(), "library/modules/runtime/renderer2d/source/shader" );
		includeDirs.add( additionalPath.getCompletePath() );

		m_sourceCode = resolveIncludes( shaderText, includeDirs );

		// parse features
		const string featuresLine = m_sourceCode.subString( 0u, m_sourceCode.indexOf( '\n' ) );

		if ( featuresLine.startsWith( "//" ) == false )
		{
			TIKI_TRACE_ERROR( "no feature list found. shader can't converted.\n" );
			return;
		}

		const char* shaderStart[]	= { "fx", "vs", "ps", "gs", "hs", "ds", "cs" };
		bool shaderEnabled[ TIKI_COUNT( shaderStart ) ];
		List< ShaderFeature > shaderFeatures[ TIKI_COUNT( shaderStart ) ];
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( shaderStart ) == ShaderType_Count );

		parseShaderFeatures( shaderEnabled, shaderFeatures, shaderStart, TIKI_COUNT( shaderStart ), featuresLine );

		for (uint typeIndex = 0u; typeIndex < TIKI_COUNT( shaderStart ); ++typeIndex)
		{
			if ( shaderEnabled[ typeIndex ] && shaderFeatures[ typeIndex ].getCount() > 0u )
			{
				List< ShaderVariant > variants;

				for (uint featureIndex = 0u; featureIndex< shaderFeatures[ typeIndex ].getCount(); ++featureIndex)
				{
					const ShaderFeature& feature = shaderFeatures[ typeIndex ][ featureIndex ];
					List< ShaderVariant > featureVariants;

					if ( variants.getCount() == 0u )
					{
						for (uint32 featureValue = 0u; featureValue <= feature.maxValue; ++featureValue)
						{
							const uint32 bitMask = featureValue << feature.startBit;

							ShaderVariant variant;
							variant.bitMask		= bitMask;
							variant.defineCode	= createDefineString( shaderFeatures[ typeIndex ], variant.bitMask );
							featureVariants.add( variant );
						}
					}
					else
					{
						for (uint32 featureValue = 1u; featureValue <= feature.maxValue; ++featureValue)
						{
							const uint32 bitMask = featureValue << feature.startBit;

							for (uint k = 0u; k < variants.getCount(); ++k)
							{
								ShaderVariant variant;
								variant.bitMask		= variants[ k ].bitMask | bitMask;
								variant.defineCode	= createDefineString( shaderFeatures[ typeIndex ], variant.bitMask );
								featureVariants.add( variant );
							}
						}
					}

					variants.addRange( featureVariants );
				}

				m_variants[ typeIndex ].create( variants.getBegin(), variants.getCount() );
			}
			else if ( shaderEnabled[ typeIndex ] )
			{
				ShaderVariant variant;
				variant.bitMask = 0u;

				m_variants[ typeIndex ].create( &variant, 1u );
			}
		}
	}

	void ShaderPreprocessor::dispose()
	{
		for (uint i = 0u; i < TIKI_COUNT( m_variants ); ++i)
		{
			m_variants[ i ].dispose();
		}
	}
}
