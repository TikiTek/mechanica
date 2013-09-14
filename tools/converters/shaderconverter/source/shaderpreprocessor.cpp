
#include "tiki/shaderconverter/shaderpreprocessor.hpp"

#include "tiki/base/stringparse.hpp"
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
		const ShaderFeature* pLastFeature = features.getLast();

		ShaderFeature& feature = features.add();
		feature.name		= name;
		feature.startBit	= pLastFeature->startBit + pLastFeature->bitCount;
		feature.bitCount	= 64u - countLeadingZeros64( maxValue );
		feature.maxValue	= maxValue;
	}

	static void parseShaderFeatures( bool* pShaderEnabled, List< ShaderFeature >* pShaderFeatures, const cstring* pShaderTypes, size_t typeCount, const string& featuresLine )
	{
		for (size_t i = 0u; i < typeCount; ++i)
		{
			const string search = formatString( "%s-features=", pShaderTypes[ i ] );

			const int index = featuresLine.indexOf( search );
			if ( index != -1 )
			{
				const int length = featuresLine.indexOf( ' ', index ) - index;
				const string featuresString = featuresLine.substring( index, length );

				Array< string > featuresList;
				featuresString.split( featuresList, "," );

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
			}

			pShaderEnabled[ i ] = ( index >= 0 );
		}
	}

	static string resolveIncludes( const string& shaderCode )
	{
		string resultCode = shaderCode;

		TRexpp regex;
		regex.Compile( "^\\s*#include\\s+(<([^\"'<>|\\b]+)>|\"([^\"'<>|\\b]+)\")" );

		cstring beginPath;
		cstring endPath;
		while ( regex.Search( resultCode.cStr(), &beginPath, &endPath ) )
		{
			const string path = string( beginPath, endPath - beginPath );
			
			resultCode = resultCode.replace( path, "penis" );
		}

		return resultCode;
	}

	void ShaderPreprocessor::create( const string& shaderText )
	{
		string featuresLine = shaderText.substring( 0u, shaderText.indexOf( '\n' ) );

		if ( featuresLine.startsWith( "//" ) == false )
		{
			ShaderVariant variant;
			variant.variantHash = 0u;
			variant.sourceCode = shaderText;

			m_variants.create( &variant, 1u );
			return;
		}

		const cstring shaderStart[]	= { "fx", "vs", "ps", "gs", "hs", "ds", "cs" };
		bool shaderEnabled[ TIKI_COUNT( shaderStart ) ];
		List< ShaderFeature > shaderFeatures[ TIKI_COUNT( shaderStart ) ];

		parseShaderFeatures( shaderEnabled, shaderFeatures, shaderStart, TIKI_COUNT( shaderStart ), featuresLine );



	}

	void ShaderPreprocessor::dispose()
	{
		m_variants.dispose();
	}
}