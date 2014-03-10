// vs-features= ps-features=TIKI_FONT_MODE,TIKI_COLOR_MODE

#include "shader/platform.fxh"

struct VertexToPixel
{
	float4 position	: TIKI_OUTPUT_POSITION0;
	float2 texcoord	: TIKI_TEXCOORD;
	float4 color	: TIKI_COLOR;
};

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

// types
struct VertexInput
{
	float2 position	: TIKI_INPUT_POSITION0;
	float2 texcoord	: TIKI_TEXCOORD;
	float4 color	: TIKI_COLOR;
};

VertexToPixel main( VertexInput input )
{
    VertexToPixel output;

	float2 clipPosition = input.position.xy * float2( 2.0f, -2.0f ) + float2( -1.0f, 1.0f );	
    output.position = float4( clipPosition, 0.0f, 1.0f );	
	output.texcoord = input.texcoord;
	output.color	= input.color;
    
    return output;
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

// constants
TIKI_DEFINE_TEXTURE2D( 0, t_texture );
TIKI_DEFINE_SAMPLER( 0, s_sampler );

float4 main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
#if TIKI_COLOR_MODE == 0
	float4 color = t_texture.Sample( s_sampler, input.texcoord );
#else
	float4 color = input.color;
#endif
	
#if TIKI_FONT_MODE
	color.a		= color.r * input.color.a;
	color.rgb	= input.color.rgb;
#elif TIKI_COLOR_MODE == 0
	color *= input.color;
#endif

	return color;
}

#else

#	error Shader type not supported.

#endif
