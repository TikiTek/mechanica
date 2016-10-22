// vs-features= ps-features=TIKI_CUTOFF[3]

#include "shader/platform.fxh"

// vertex to pixel
TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( VertexToPixel )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float4, TIKI_OUTPUT_POSITION0 )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float2, TIKI_TEXCOORD0 )
TIKI_VERTEX_TO_PIXEL_DEFINITION_END( VertexToPixel )

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

// vertex input
TIKI_VERTEX_INPUT_DEFINITION_BEGIN( VertexInput )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 0, float2, TIKI_INPUT_POSITION0 )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 1, float2, TIKI_TEXCOORD0 )
TIKI_VERTEX_INPUT_DEFINITION_END( VertexInput )

TIKI_ENTRY_POINT( VertexInput, VertexToPixel, main )
{
	TIKI_VERTEX_TO_PIXEL_BEGIN( VertexToPixel );

	float4 position = float4( TIKI_VERTEX_INPUT_GET( TIKI_INPUT_POSITION0 ), 0.0, 1.0 );
	float2 texCoord = TIKI_VERTEX_INPUT_GET( TIKI_TEXCOORD0 );

	TIKI_VERTEX_TO_PIXEL_SET_POSITION( TIKI_OUTPUT_POSITION0, position );
	TIKI_VERTEX_TO_PIXEL_SET( TIKI_TEXCOORD0, texCoord );

	TIKI_VERTEX_TO_PIXEL_END( VertexToPixel );
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

#include "shader/bloom_shader.hpp"

// pixel output
TIKI_PIXEL_OUTPUT_DEFINITION_BEGIN( PixelOutput )
	TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( 0, float4, TIKI_OUTPUT_COLOR0 )
TIKI_PIXEL_OUTPUT_DEFINITION_END( PixelOutput )

// constants
TIKI_DEFINE_SAMPLER( 0, s_samplerLinear )

#if TIKI_CUTOFF
	TIKI_DEFINE_CONSTANT( 0, BloomCutoffPixelConstantData, c_pixelData )

	TIKI_DEFINE_TEXTURE2D( 0, t_accumulation )
	#if TIKI_CUTOFF > 1
		TIKI_DEFINE_TEXTURE2D( 1, t_selfillu )
	#endif
#else
	TIKI_DEFINE_TEXTURE2D( 0, t_pass )
#endif

TIKI_ENTRY_POINT( VertexToPixel, PixelOutput, main )
{
	TIKI_PIXEL_OUTPUT_BEGIN( PixelOutput );

	float2 texCoord = TIKI_VERTEX_TO_PIXEL_GET( TIKI_TEXCOORD0 );

#if TIKI_CUTOFF
	float3 sourceColor = TIKI_TEX2D( t_accumulation, s_samplerLinear, texCoord ).rgb;
#if TIKI_CUTOFF > 1
	float3 selfilluColor = TIKI_TEX2D( t_selfillu, s_samplerLinear, texCoord ).rgb;
	sourceColor += selfilluColor;
#endif
		
	sourceColor *= (sourceColor >= getCutoffThresold( c_pixelData ));
	//float colorValue = dot( diffuseColor, float3( 0.2126f, 0.7152f, 0.0722f ) );

	float4 color = float4(sourceColor, sourceColor.r * sourceColor.g * sourceColor.b );
#else
	float4 color = TIKI_TEX2D( t_pass, s_samplerLinear, texCoord );
	color = TIKI_SATURATE( color );
#endif

	TIKI_PIXEL_OUTPUT_SET( TIKI_OUTPUT_COLOR0, color );
	TIKI_PIXEL_OUTPUT_END( PixelOutput );
}

#else

#	error Shader type not supported.

#endif
