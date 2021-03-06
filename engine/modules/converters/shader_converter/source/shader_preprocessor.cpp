#include "tiki/shader_converter/shader_preprocessor.hpp"

#include "tiki/base/bits.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/container/list.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/path.hpp"

#include "TRexpp.h"

namespace tiki
{
	struct ShaderFeature
	{
		DynamicString	name;

		uint32			startBit;
		uint32			bitCount;

		uint32			maxValue;
	};

	static void addFeature( List< ShaderFeature >& features, const DynamicString& name, uint32 maxValue )
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

	static void parseShaderFeatures( bool* pShaderEnabled, List< ShaderFeature >* pShaderFeatures, const char** pShaderTypes, uint typeCount, const DynamicString& featuresLine )
	{
		for (uint i = 0u; i < typeCount; ++i)
		{
			const DynamicString search = formatDynamicString( "%s-features=", pShaderTypes[ i ] );

			const int index = featuresLine.indexOf( search );
			if( i != 0 )
			{
				pShaderEnabled[ i - 1u ] = (index >= 0);
			}

			if ( index != -1 )
			{
				const int baseIndex = int( index + search.getLength() );
				const int length = featuresLine.indexOf( ' ', baseIndex ) - baseIndex;
				const DynamicString featuresString = featuresLine.subString( baseIndex, length );

				Array< DynamicString > featuresList;
				featuresString.trim().split( featuresList, "," );

				for (uint j = 0u; j < featuresList.getCount(); ++j)
				{
					DynamicString name = featuresList[ j ];
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

						const DynamicString maxValueString = name.subString( arrayIndex, arrayLength );
						maxValue	= string_tools::parseUInt32( maxValueString.cStr() ) - 1u;
						name		= name.subString( 0u, arrayIndex - 1 );
					}

					if ( i == 0u ) // fx feature will be applied to all shaders in file
					{
						for (uint k = 0u; k < ShaderType_Count; ++k)
						{
							addFeature( pShaderFeatures[ k ], name, maxValue );
						}
					}
					else
					{
						addFeature( pShaderFeatures[ i - 1u ], name, maxValue );
					}
				}

				featuresList.dispose();
			}
		}
	}

	static DynamicString resolveIncludes( const DynamicString& shaderCode, const List< Path >& includeDirs )
	{
		DynamicString resultCode = shaderCode;

		TRexpp includeExpression;
		includeExpression.Compile( "^\\s*#\\s*include\\s+[<\"](.+)[>\"]\\s*$" );

		//TRexpp featuresExpression;
		//featuresExpression.Compile( "^\\s*\\/\\/\\s+(fx|vs|ps|gs|hs|ds|cs)-features=" );

		Path lastPath;
		while( includeExpression.Match( resultCode.cStr() ) )
		{
			const char* pExpressionBegin = nullptr;
			int expLength = 0u;
			if( !includeExpression.GetSubExp( 0, &pExpressionBegin, &expLength ) )
			{
				TIKI_TRACE_ERROR( "Unable to get include expression.\n" );
				break;
			}
			const uintreg expBeginIndex = pExpressionBegin - resultCode.cStr();

			const char* pMatchBegin = nullptr;
			int matchLength = 0u;
			if( !includeExpression.GetSubExp( 1, &pMatchBegin, &matchLength ) )
			{
				TIKI_TRACE_ERROR( "Unable to get include match.\n" );
				break;
			}

			DynamicString path = DynamicString( pMatchBegin, matchLength );
			Path fullPath;
			for (uint i = 0u; i < includeDirs.getCount(); ++i)
			{
				fullPath.setCombinedPath( includeDirs[ i ], path.cStr() );
				if ( file::exists( fullPath.getCompletePath() ) )
				{
					path = fullPath.getCompletePath();
					break;
				}
			}

			if ( !file::exists( path.cStr() ) )
			{
				fullPath.setCombinedPath( lastPath.getDirectoryWithPrefix(), path.cStr() );
				if( !file::exists( fullPath.getCompletePath() ) )
				{
					TIKI_TRACE_ERROR( "include file not found: %s\n", path.cStr() );
					resultCode = resultCode.remove( expBeginIndex, uint( expLength ) );
					continue;
				}
				else
				{
					path = fullPath.getCompletePath();
				}
			}

			lastPath.setCompletePath( path.cStr() );

			Array< char > fileData;
			if ( !file::readAllText( path.cStr(), fileData ) )
			{
				TIKI_TRACE_ERROR( "open include file failed: %s\n", path.cStr() );
				break;
			}

			const DynamicString fileText = DynamicString( fileData.getBegin() );
			resultCode = resultCode.remove( expBeginIndex, uint( expLength ) );
			resultCode = resultCode.insert( fileText, expBeginIndex );

			fileData.dispose();

			break;
		}

		return resultCode;
	}

	static DynamicString createDefineString( List< ShaderFeature >& features, uint32 bitMask )
	{
		DynamicString defineString = "";

		for (uint i = 0u; i < features.getCount(); ++i)
		{
			const ShaderFeature& feature = features[ i ];
			const uint value = getBitValue( bitMask, feature.startBit, feature.bitCount );

			defineString += formatDynamicString( "#define %s %u\n", feature.name.cStr(), value );
		}

		return defineString;
	}

	void ShaderPreprocessor::create( const DynamicString& shaderText, const List< Path >& includePathes )
	{
		// resolve includes
		m_sourceCode = resolveIncludes( shaderText, includePathes );

		// parse features
		const DynamicString featuresLine = m_sourceCode.subString( 0u, m_sourceCode.indexOf( '\n' ) );

		if ( featuresLine.startsWith( "//" ) == false )
		{
			TIKI_TRACE_ERROR( "no feature list found. shader can't converted.\n" );
			return;
		}

		bool shaderEnabled[ ShaderType_Count ];
		List< ShaderFeature > shaderFeatures[ ShaderType_Count ];

		const char* shaderStart[]	={ "fx", "vs", "ps", "gs", "hs", "ds", "cs" };
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( shaderStart ) == ShaderType_Count + 1u );
		parseShaderFeatures( shaderEnabled, shaderFeatures, shaderStart, TIKI_COUNT( shaderStart ), featuresLine );

		for (uint typeIndex = 0u; typeIndex < ShaderType_Count; ++typeIndex)
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
