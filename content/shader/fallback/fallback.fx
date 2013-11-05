// vs-features= ps-features=

#include "shader/platform.fxh"

struct VertexToPixel
{
	float4	position	: TIKI_POSITION;
	float4	color		: TIKI_COLOR;
	float2	texcoord	: TIKI_TEXCOORD;
};

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

// types
struct VertexInput
{
	float3	position	: TIKI_POSITION;
	float4	color		: TIKI_COLOR;
	float2	texcoord	: TIKI_TEXCOORD;
};

struct VertexConstants
{
	matrix	worldMatrix;
	matrix	viewMatrix;
	matrix	projectionMatrix;
};

// constants
TIKI_DEFINE_CONSTANT( 0, VertexConstants, c_instanceData );

VertexToPixel main( VertexInput input )
{
	VertexToPixel output = (VertexToPixel)0;

	output.position = float4( input.position, 1.0f );
	output.position = mul( output.position, c_instanceData.worldMatrix );
	output.position = mul( output.position, c_instanceData.viewMatrix );
	output.position = mul( output.position, c_instanceData.projectionMatrix );

	output.color	= input.color;
	output.texcoord	= input.texcoord;

	return output;
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

// types
struct PixelConstants
{
	float4 vertexColor;
};

// constants
TIKI_DEFINE_CONSTANT( 0, PixelConstants, c_instanceData );
TIKI_DEFINE_TEXTURE2D( 0, t_diffuseMap );
TIKI_DEFINE_SAMPLER( 0, s_linear );

float4 main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
	float4 output = input.color;

	output *= TIKI_TEX2D( t_diffuseMap, s_linear, input.texcoord );
	output *= c_instanceData.vertexColor;

	return saturate( output );
}

#else

#	error Shader type not supported.

#endif
