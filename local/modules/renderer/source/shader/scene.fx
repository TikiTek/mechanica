// vs-features=TIKI_ENABLE_SKINNING ps-features=

#include "shader/platform.fxh"
#include "shader/scene_shader.hpp"

// vertex to pixel
TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( VertexToPixel )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float4, position,	TIKI_OUTPUT_POSITION )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float3, tangent,	TIKI_TANGENT )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float3, binormal,	TIKI_BINORMAL )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float3, normal,	TIKI_NORMAL )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float2, texCoord,	TIKI_TEXCOORD )
TIKI_VERTEX_TO_PIXEL_DEFINITION_END( VertexToPixel )

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

// vertex input
TIKI_VERTEX_INPUT_DEFINITION_BEGIN( VertexInput )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 0, float3, position,		TIKI_INPUT_POSITION )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 1, float3, normal,		TIKI_NORMAL )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 2, float4, tangentFlip,	TIKI_TANGENT )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 3, float2, texCoord,		TIKI_TEXCOORD )
#if TIKI_ENABLE_SKINNING
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 4, uint4,  skinIndices,	TIKI_JOINTINDICES )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 5, float4, skinWeights,	TIKI_JOINTWEIGHTS )
#endif
TIKI_VERTEX_INPUT_DEFINITION_END( VertexInput )

// constants
TIKI_DEFINE_CONSTANT( 0, SceneVertexConstantData, c_instanceData )

#if TIKI_ENABLE_SKINNING
TIKI_DEFINE_CONSTANT( 1, SceneSkinningVertexConstantData, c_skinningData )
#endif

TIKI_ENTRY_POINT( VertexInput, VertexToPixel, main )
{
    TIKI_VERTEX_TO_PIXEL_BEGIN( VertexToPixel );

	float4 position = float4( TIKI_VERTEX_INPUT_GET( position ), 1.0 );
#if TIKI_ENABLE_SKINNING
	uint4 skinIndices = TIKI_VERTEX_INPUT_GET( skinIndices );
	float4 skinWeights = TIKI_VERTEX_INPUT_GET( skinWeights );

	float4x4 skinMatrix = float4x4( 0.0 );
	for(int i = 0; i < 4; i++)	
	{
		skinMatrix += c_skinningData.matrices[ skinIndices[ i ] ] * skinWeights[ i ];
	}
	skinMatrix /= dot( skinWeights, float4( 1.0 ) );

	position = TIKI_MUL( position, skinMatrix );

	float3x3 normalMatrix = float3x3( skinMatrix );
	normalMatrix = TIKI_MUL( normalMatrix, c_instanceData.modelNormalMatrix );
#else
	float3x3 normalMatrix = c_instanceData.modelNormalMatrix;
#endif
	position = TIKI_MUL( position, c_instanceData.mvpMatrix );

	float3 normal = TIKI_VERTEX_INPUT_GET( normal );
	normal = TIKI_MUL( normal, normalMatrix );

	float3 tangent = TIKI_VERTEX_INPUT_GET( tangentFlip ).xyz;
	tangent = TIKI_MUL( tangent.xyz, normalMatrix );

	float3 binormal = cross( normal, tangent ) * TIKI_VERTEX_INPUT_GET( tangentFlip ).w;

	float2 texCoord = TIKI_VERTEX_INPUT_GET( texCoord );

	TIKI_VERTEX_TO_PIXEL_SET_POSITION( position, position );
	TIKI_VERTEX_TO_PIXEL_SET( normal, normal );
	TIKI_VERTEX_TO_PIXEL_SET( tangent, tangent );
	TIKI_VERTEX_TO_PIXEL_SET( binormal, binormal );
	TIKI_VERTEX_TO_PIXEL_SET( texCoord, texCoord );

    TIKI_VERTEX_TO_PIXEL_END( VertexToPixel );
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

#include "shader/functions.fxh"
#include "shader/geometrybuffer.fxh"

// pixel output
TIKI_GEOMETRYBUFFER_PIXEL_OUTPUT

// constants
TIKI_DEFINE_CONSTANT( 0, ScenePixelConstantData, c_instanceData )

TIKI_DEFINE_TEXTURE2D( 0, t_diffuseMap )
TIKI_DEFINE_TEXTURE2D( 1, t_normalMap )
TIKI_DEFINE_TEXTURE2D( 2, t_selfilluMap )

TIKI_DEFINE_SAMPLER( 0, s_linear )

TIKI_ENTRY_POINT( VertexToPixel, GeometryBufferPixelOutput, main )
{
	TIKI_PIXEL_OUTPUT_BEGIN( GeometryBufferPixelOutput );

	float3 normal	= TIKI_VERTEX_TO_PIXEL_GET( normal );
	float3 tangent	= TIKI_VERTEX_TO_PIXEL_GET( tangent );
	float3 binormal	= TIKI_VERTEX_TO_PIXEL_GET( binormal );
	float2 texCoord	= TIKI_VERTEX_TO_PIXEL_GET( texCoord );

	float3 diffuseColor = TIKI_TEX2D( t_diffuseMap, s_linear, texCoord ).rgb;
	float3 normalSample = TIKI_TEX2D( t_normalMap, s_linear, texCoord ).rgb * 2.0 - 1.0;
	float3 selfIlluminationColor = TIKI_TEX2D( t_selfilluMap, s_linear, texCoord ).rgb;

	float3x3 tangentSpaceNormalMatrix = float3x3( tangent, binormal, normal );
	float3 finalNormal = normalize( TIKI_MUL( normalSample, tangentSpaceNormalMatrix ) );
	float2 packedNormal = encodeNormal( finalNormal );

	TIKI_PIXEL_OUTPUT_SET( buffer0Sample, createGeometryBuffer0Sample( diffuseColor, c_instanceData.specluarBrightness ) );
	TIKI_PIXEL_OUTPUT_SET( buffer1Sample, createGeometryBuffer1Sample( selfIlluminationColor, c_instanceData.selfIlluminationFactor ) );
	TIKI_PIXEL_OUTPUT_SET( buffer2Sample, createGeometryBuffer2Sample( packedNormal, c_instanceData.specluarIntensity, c_instanceData.specluarPower ) );
	
	TIKI_PIXEL_OUTPUT_END( GeometryBufferPixelOutput );
}

#else

#	error Shader type not supported.

#endif
