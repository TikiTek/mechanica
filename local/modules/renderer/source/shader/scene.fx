// vs-features= ps-features=

#include "shader/functions.fxh"
#include "shader/geometrybuffer.fxh"
#include "shader/platform.fxh"
#include "shader/scene_shader.hpp"

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

// constants
TIKI_DEFINE_CONSTANT( 0, SceneVertexConstantData, c_instanceData );

VertexToPixel main( VertexInput input )
{
	VertexToPixel output = (VertexToPixel)0;

	output.position = float4( input.position, 1.0f );
	output.position = mul( output.position, c_instanceData.mvpMatrix );

	float3x3 normalMatrix = (float3x3)c_instanceData.modelViewMatrix;
	output.normal	= normalize( mul( input.normal, normalMatrix ) );
	output.tangent	= normalize( mul( input.tangentFlip.xyz, normalMatrix ) );
	output.binormal	= normalize( cross( output.normal, output.tangent ) * input.tangentFlip.w );

	output.texCoord	= input.texCoord;

	return output;
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

// constants
TIKI_DEFINE_CONSTANT( 0, ScenePixelConstantData, c_instanceData );

TIKI_DEFINE_TEXTURE2D( 0, t_diffuseMap );
TIKI_DEFINE_TEXTURE2D( 1, t_normalMap );
TIKI_DEFINE_TEXTURE2D( 2, t_selfilluMap );

TIKI_DEFINE_SAMPLER( 0, s_linear );

GeometryBufferSample main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
	float3 diffuseColor = TIKI_TEX2D( t_diffuseMap, s_linear, input.texCoord ).rgb;
	float3 normalSample = TIKI_TEX2D( t_normalMap, s_linear, input.texCoord ).rgb * 2.0f - 1.0f;
	float3 selfIlluminationColor = TIKI_TEX2D( t_selfilluMap, s_linear, input.texCoord ).rgb;

	float3x3 tangentSpaceNormalMatrix = float3x3( input.tangent, input.binormal, input.normal );
	float3 normal = normalize( mul( normalSample, tangentSpaceNormalMatrix ) );
	float2 packedNormal = encodeNormal( normal );

	return createGeometryBufferSample( diffuseColor, selfIlluminationColor, c_instanceData.selfIlluminationFactor, packedNormal, c_instanceData.specluarBrightness, c_instanceData.specluarIntensity, c_instanceData.specluarPower );
}

#else

#	error Shader type not supported.

#endif
