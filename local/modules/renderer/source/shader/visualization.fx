// vs-features= ps-features=TIKI_VISUALIZATION_MODE[4]

#include "shader/functions.fxh"
#include "shader/platform.fxh"
#include "shader/visualization_shader.hpp"

struct VertexToPixel
{
	float4	position	: TIKI_OUTPUT_POSITION;
	float2	texCoord	: TIKI_TEXCOORD;
};

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

// types
struct VertexInput
{
	float2	position	: TIKI_INPUT_POSITION;
	float2	texCoord	: TIKI_TEXCOORD;
};

VertexToPixel main( VertexInput input )
{
	VertexToPixel output = (VertexToPixel)0;

	output.position = float4( input.position, 0.0f, 1.0f );
	output.texCoord	= input.texCoord;

	return output;
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

// constants
TIKI_DEFINE_TEXTURE2D( 0, t_gBuffer0 );
TIKI_DEFINE_TEXTURE2D( 1, t_gBuffer1 );
TIKI_DEFINE_TEXTURE2D( 2, t_gBuffer2 );

TIKI_DEFINE_SAMPLER( 0, s_linear );

float4 main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
	float4 color = float4( 0.0f, 0.0f, 0.0f, 1.0f );

#if TIKI_VISUALIZATION_MODE == TIKI_VISUALIZATION_MODE_DIFFUSE
	color.rgb = TIKI_TEX2D( t_gBuffer0, s_linear, input.texCoord ).rgb;
#elif TIKI_VISUALIZATION_MODE == TIKI_VISUALIZATION_MODE_SELFILLUMINATION
	color.rgb = TIKI_TEX2D( t_gBuffer1, s_linear, input.texCoord ).rgb;
#elif TIKI_VISUALIZATION_MODE == TIKI_VISUALIZATION_MODE_NORMAL
	color.rgb = decodeNormal( TIKI_TEX2D( t_gBuffer2, s_linear, input.texCoord ).xy );
	color.rgb = ( color.rgb / 2.0f ) + 0.5f;
#else
	color.r = 1.0f;
#endif

	return saturate( color );
}

#else

#	error Shader type not supported.

#endif
