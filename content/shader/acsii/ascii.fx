// vs-features= ps-features=TIKI_DOWNSAMPLE

#include "shader/platform.fxh"

struct VertexToPixel
{
	float4 position	: TIKI_OUTPUT_POSITION0;
	float2 texcoord	: TIKI_TEXCOORD;
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
	output.texcoord = input.texcoord;
    
    return output;
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

// constants
TIKI_DEFINE_SAMPLER( 0, s_samplerLinear );

#if TIKI_DOWNSAMPLE

TIKI_DEFINE_TEXTURE2D( 0, t_gBufferDiffuse );
TIKI_DEFINE_TEXTURE2D( 1, t_gBufferNormal );

#else

TIKI_DEFINE_TEXTURE2D( 0, t_downsample );
TIKI_DEFINE_TEXTURE3D( 1, t_ascii );
TIKI_DEFINE_SAMPLER( 1, s_samplerNearst );

#endif

float4 main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
#if TIKI_DOWNSAMPLE
	float4 color = t_gBufferDiffuse.Sample( s_samplerLinear, input.texcoord );
#else
	float4 color = t_downsample.Sample( s_samplerNearst, input.texcoord );

	float3 uvw = float3( input.texcoord, 0.0f );
	uvw.x *= 105.0f;
	uvw.y *= 60.0f;
	uvw.z = color.r;

	float4 ascii = t_ascii.Sample( s_samplerLinear, uvw );

	color = float4( ascii.rrr, 1.0f ); //float4( color.rgb * ascii.rrr, 1.0f);
#endif

	return color;
}

#else

#	error Shader type not supported.

#endif
