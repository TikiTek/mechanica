// vs-features= ps-features=

#include "shader/platform.fxh"

struct VertexToPixel
{
	float4	position	: TIKI_OUTPUT_POSITION;
	float3	tangent		: TIKI_TANGENT;
	float3	binormal	: TIKI_BINORMAL;
	float3	normal		: TIKI_NORMAL;
	float2	texCoord	: TIKI_TEXCOORD;
};

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

// types
struct VertexInput
{
	float3	position	: TIKI_INPUT_POSITION;
	float3	normal		: TIKI_NORMAL;
	float4	tangentFlip	: TIKI_TANGENT;
	float2	texCoord	: TIKI_TEXCOORD;
};

struct VertexConstants
{
	matrix	mvpMatrix;
	matrix	modelMatrix;
	matrix	modelViewMatrix;
};

// constants
TIKI_DEFINE_CONSTANT( 0, VertexConstants, c_instanceData );

VertexToPixel main( VertexInput input )
{
	VertexToPixel output = (VertexToPixel)0;

	output.position = float4( input.position, 1.0f );
	output.position = mul( output.position, c_instanceData.mvpMatrix );

	float3x3 normalMatrix = (float3x3)c_instanceData.modelMatrix;
	output.normal	= mul( input.normal, normalMatrix );
	output.tangent	= mul( input.tangentFlip.xyz, normalMatrix );
	output.binormal	= cross( output.normal, output.tangent ) * input.tangentFlip.w;

	output.texCoord	= input.texCoord;

	return output;
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

// types
//struct PixelConstants
//{
//	float4 vertexColor;
//};

// constants
//TIKI_DEFINE_CONSTANT( 0, PixelConstants, c_instanceData );
TIKI_DEFINE_TEXTURE2D( 0, t_diffuseMap );
TIKI_DEFINE_SAMPLER( 0, s_linear );

float4 main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
	float4 output = TIKI_TEX2D( t_diffuseMap, s_linear, input.texCoord );
	return saturate( output );
}

#else

#	error Shader type not supported.

#endif
