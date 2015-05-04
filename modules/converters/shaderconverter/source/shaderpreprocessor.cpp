
#include "tiki/shaderconverter/shaderpreprocessor.hpp"

#include "tiki/base/bits.hpp"
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

	static void parseShaderFeatures( bool* pShaderEnabled, List< ShaderFeature >* pShaderFeatures, const cstring* pShaderTypes, uint typeCount, const string& featuresLine )
	{
		for (uint i = 0u; i < typeCount; ++i)
		{
			const string search = formatString( "%s-features=", pShaderTypes[ i ] );

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
						maxValue	= ParseString::parseUInt32( maxValueString ) - 1u;
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

		TRexpp regex;
		regex.Compile( "^\\s*#include\\s+(<([^\"'<>|\\b]+)>|\"([^\"'<>|\\b]+)\")" );

		cstring beginPath;
		cstring endPath;
		while ( regex.Search( resultCode.cStr(), &beginPath, &endPath ) )
		{
			string path = string( beginPath, endPath - beginPath );

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
				TIKI_TRACE_ERROR( "include file not found: %s\n", path.cStr() );
				return "";
			}

			Array< uint8 > fileData;
			if ( !file::readAllBytes( path.cStr(), fileData ) )
			{
				TIKI_TRACE_ERROR( "open include file failed: %s\n", path.cStr() );
				return "";
			}

			const string fileText = string( reinterpret_cast< const char* >( fileData.getBegin() ) );
			resultCode = resultCode.replace( path, fileText );
		}

		return resultCode;
	}

	static string createDefineString( List< ShaderFeature >& features, uint32 bitMask )
	{
		string defineString = "";

		for (uint i = 0u; i < features.getCount(); ++i)
		{
			const ShaderFeature& feature = features[ i ];
			const uint value = getBitValue( bitMask, feature.startBit, feature.bitCount );

			defineString += formatString( "#define %s %u\n", feature.name.cStr(), value );
		}

		return defineString;
	}

	void ShaderPreprocessor::create( const string& shaderText )
	{
		// resolve includes
		List< string > includeDirs;

		includeDirs.add( "../../../../../../library/modules/runtime/graphics/include/tiki/graphics/shader" );

		m_sourceCode = resolveIncludes( shaderText, includeDirs );

		// parse features
		const string featuresLine = shaderText.subString( 0u, shaderText.indexOf( '\n' ) );

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
