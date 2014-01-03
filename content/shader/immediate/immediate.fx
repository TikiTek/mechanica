// vs-features= ps-features=

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

#if 0
	float4 vertexId	: TIKI_TEXCOORD0;
	float2 size		: TIKI_TEXCOORD1;
#endif
};

VertexToPixel main( VertexInput input )
{
    VertexToPixel output;

	//float2 clipPosition = input.position * float2( 2.0f, -2.0f ) + float2( -1.0f, 1.0f );
	
    output.position = float4( input.position, 0.0f, 1.0f );
	output.texcoord = input.texcoord;
    
#if 0
    output.texcoord = float2(
		( input.texcoord.x * input.vertexId.x ) + ( input.texcoord.z * input.vertexId.z ),
		( input.texcoord.y * input.vertexId.y ) + ( input.texcoord.w * input.vertexId.w )
	);
#endif
    
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
#if 0
	return float4( 1.0f, 0.0f, 0.0f, 1.0f );
#endif
	return t_texture.Sample( s_sampler, input.texcoord );
}

#else

#	error Shader type not supported.

#endif
