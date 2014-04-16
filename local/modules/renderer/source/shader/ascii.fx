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

#include "shader/functions.fxh"
#include "shader/ascii_shader.hpp"

// constants
TIKI_DEFINE_SAMPLER( 0, s_samplerLinear );
TIKI_DEFINE_SAMPLER( 1, s_samplerNearst );

#if TIKI_DOWNSAMPLE

TIKI_DEFINE_TEXTURE2D( 0, t_gBufferDiffuse );
TIKI_DEFINE_TEXTURE2D( 1, t_gBufferNormal );
TIKI_DEFINE_TEXTURE2D( 2, t_depthBuffer );

TIKI_DEFINE_CONSTANT( 0, AsciiPixelConstantData, c_pixelData );

#else

TIKI_DEFINE_TEXTURE2D( 0, t_downsample );
TIKI_DEFINE_TEXTURE3D( 1, t_ascii );

#endif

float4 main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
#if TIKI_DOWNSAMPLE
	float4 projectedPosition = getProjectedPosition( input.texcoord, t_depthBuffer.Sample( s_samplerNearst, input.texcoord ).r );
	float3 viewPosition = reconstructViewSpacePosition( projectedPosition, c_pixelData.inverseProjection );

	float4 color = t_gBufferDiffuse.Sample( s_samplerLinear, input.texcoord );
	color.a = viewPosition.z;
#else
	float4 color = t_downsample.Sample( s_samplerNearst, input.texcoord );

	float3 uvw = float3( input.texcoord, 0.0f );
	uvw.x *= 105.0f;
	uvw.y *= 60.0f;
	uvw.z = color.a;

	float ascii = t_ascii.Sample( s_samplerLinear, uvw ).r;
	color = float4( color.rgb * ascii.rrr, 1.0f);
#endif

	return color;
}

#else

#	error Shader type not supported.

#endif
