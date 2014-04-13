// vs-features= ps-features=TIKI_DOWNSAMPLE

#include "shader/platform.fxh"

struct VertexToPixel
{
	float4 position	: TIKI_OUTPUT_POSITION0;
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
};

VertexToPixel main( VertexInput input )
{
    VertexToPixel output;

    output.position = float4( input.position, 0.0f, 1.0f );
    
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
#if TIKI_DOWNSAMPLE
	float4 color = float4( 1.0f, 1.0f, 1.0f, 1.0f );
#else
	float4 color = float4( 0.0f, 0.0f, 0.0f, 0.0f );
#endif

	return color;
}

#else

#	error Shader type not supported.

#endif
