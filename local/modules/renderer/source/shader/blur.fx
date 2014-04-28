// vs-features= ps-features=

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

#include "shader/blur_shader.hpp"

// constants
TIKI_DEFINE_SAMPLER( 0, s_samplerLinear );

TIKI_DEFINE_TEXTURE2D( 0, t_source );

TIKI_DEFINE_CONSTANT( 0, BlurPixelConstantData, c_pixelData );

float4 main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
	float4 color = float4( 0.0f, 0.0f, 0.0f, 0.0f );
	color += 0.15f * TIKI_TEX2D( t_source, s_samplerLinear, input.texCoord + ( float2( -2, -2 ) * getBlurPixelOffset( c_pixelData ) ) );
	color += 0.20f * TIKI_TEX2D( t_source, s_samplerLinear, input.texCoord + ( float2( -1, -1 ) * getBlurPixelOffset( c_pixelData ) ) );
	color += 0.30f * TIKI_TEX2D( t_source, s_samplerLinear, input.texCoord );
	color += 0.20f * TIKI_TEX2D( t_source, s_samplerLinear, input.texCoord + ( float2( +1, +1 ) * getBlurPixelOffset( c_pixelData ) ) );
	color += 0.15f * TIKI_TEX2D( t_source, s_samplerLinear, input.texCoord + ( float2( +2, +2 ) * getBlurPixelOffset( c_pixelData ) ) );
	


	return color;
}

#else

#	error Shader type not supported.

#endif
