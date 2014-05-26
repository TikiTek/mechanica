// vs-features= ps-features=TIKI_CUTOFF_PASS,TIKI_ACCUMULATION_PASS

#include "shader/platform.fxh"

struct VertexToPixel
{
	float4 position	: TIKI_OUTPUT_POSITION;
	float2 texCoord	: TIKI_TEXCOORD;
};

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

// types
struct VertexInput
{
	float2 position	: TIKI_INPUT_POSITION;
	float2 texCoord	: TIKI_TEXCOORD;
};

VertexToPixel main( VertexInput input )
{
    VertexToPixel output;

    output.position = float4( input.position, 0.0f, 1.0f );
	output.texCoord = input.texCoord;
    
    return output;
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

#include "shader/bloom_shader.hpp"

// constants
TIKI_DEFINE_SAMPLER( 0, s_samplerLinear );

#if TIKI_CUTOFF_PASS
TIKI_DEFINE_TEXTURE2D( 0, t_accumulation );
TIKI_DEFINE_TEXTURE2D( 1, t_selfillu );
#else
TIKI_DEFINE_TEXTURE2D( 0, t_pass );
#endif

float4 main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
#if TIKI_CUTOFF_PASS
	float3 accumulationColor = TIKI_TEX2D( t_accumulation, s_samplerLinear, input.texCoord ).rgb;
	float4 selfilluColor = TIKI_TEX2D( t_selfillu, s_samplerLinear, input.texCoord );

	float3 color = accumulationColor + selfilluColor.rgb; // * selfilluColor.a );
	float colorValue = dot( color, float3( 0.2126f, 0.7152f, 0.0722f ) );

	return ( colorValue > 1.0f ? float4( color, 1.0f ) : float4( 0.0f, 0.0f, 0.0f, 0.0f ) );
#else
	float4 color = TIKI_TEX2D( t_pass, s_samplerLinear, input.texCoord );
	return saturate( color );
#endif
}

#else

#	error Shader type not supported.

#endif
