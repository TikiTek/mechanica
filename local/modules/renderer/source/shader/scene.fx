// vs-features=TIKI_ENABLE_SKINNING ps-features=

#include "shader/platform.fxh"
#include "shader/scene_shader.hpp"

// vertex to pixel
TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( VertexToPixel )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float4, TIKI_OUTPUT_POSITION0 )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float3, TIKI_TANGENT0 )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float3, TIKI_BINORMAL0 )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float3, TIKI_NORMAL0 )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float2, TIKI_TEXCOORD0 )
TIKI_VERTEX_TO_PIXEL_DEFINITION_END( VertexToPixel )

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

// vertex input
TIKI_VERTEX_INPUT_DEFINITION_BEGIN( VertexInput )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 0, float3, TIKI_INPUT_POSITION0 )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 1, float3, TIKI_NORMAL0 )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 2, float4, TIKI_TANGENT0 )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 3, float2, TIKI_TEXCOORD0 )
#if TIKI_ENABLE_SKINNING
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 4, uint4,  TIKI_JOINTINDICES0 )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 5, float4, TIKI_JOINTWEIGHTS0 )
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

	float4 position = float4( TIKI_VERTEX_INPUT_GET( TIKI_INPUT_POSITION0 ), 1.0 );
#if TIKI_ENABLE_SKINNING
	uint4 skinIndices = TIKI_VERTEX_INPUT_GET( TIKI_JOINTINDICES0 );
	float4 skinWeights = TIKI_VERTEX_INPUT_GET( TIKI_JOINTWEIGHTS0 );

	float4x4 skinMatrix = TIKI_CAST( float4x4, 0.0 );
	for(int i = 0; i < 4; i++)	
	{
		skinMatrix += c_skinningData.matrices[ skinIndices[ i ] ] * skinWeights[ i ];
	}
	skinMatrix /= dot( skinWeights, float4( 1.0, 1.0, 1.0, 1.0 ) );

	position = TIKI_MUL( position, skinMatrix );

	float3x3 normalMatrix = TIKI_CAST( float3x3, skinMatrix );
	normalMatrix = TIKI_MUL( normalMatrix, TIKI_CAST( float3x3, c_instanceData.modelNormalMatrix ) );
#else
	float3x3 normalMatrix = TIKI_CAST( float3x3, c_instanceData.modelNormalMatrix );
#endif
	position = TIKI_MUL( position, c_instanceData.mvpMatrix );

	float3 normal = normalize( TIKI_VERTEX_INPUT_GET( TIKI_NORMAL0 ) );
	normal = normalize( TIKI_MUL( normal, normalMatrix ) );

	float3 tangent = normalize( TIKI_VERTEX_INPUT_GET( TIKI_TANGENT0 ).xyz );
	tangent = normalize( TIKI_MUL( tangent.xyz, normalMatrix ) );

	float3 binormal = normalize( cross( normal, tangent ) ); //TIKI_VERTEX_INPUT_GET( TIKI_TANGENT0 ).www; // * TIKI_VERTEX_INPUT_GET( TIKI_TANGENT0 ).w );

	float2 texCoord = TIKI_VERTEX_INPUT_GET( TIKI_TEXCOORD0 );
	texCoord.y = 1 - texCoord.y;

	TIKI_VERTEX_TO_PIXEL_SET_POSITION( TIKI_OUTPUT_POSITION0, position );
	TIKI_VERTEX_TO_PIXEL_SET( TIKI_NORMAL0,		normal );
	TIKI_VERTEX_TO_PIXEL_SET( TIKI_TANGENT0,	tangent );
	TIKI_VERTEX_TO_PIXEL_SET( TIKI_BINORMAL0,	binormal );
	TIKI_VERTEX_TO_PIXEL_SET( TIKI_TEXCOORD0,	texCoord );

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
TIKI_DEFINE_TEXTURE2D( 2, t_specularMap )
TIKI_DEFINE_TEXTURE2D( 3, t_glossMap )

TIKI_DEFINE_SAMPLER( 0, s_linear )

TIKI_ENTRY_POINT( VertexToPixel, GeometryBufferPixelOutput, main )
{
	TIKI_PIXEL_OUTPUT_BEGIN( GeometryBufferPixelOutput );

	float3 normal	= TIKI_VERTEX_TO_PIXEL_GET( TIKI_NORMAL0 );
	float3 tangent	= TIKI_VERTEX_TO_PIXEL_GET( TIKI_TANGENT0 );
	float3 binormal	= TIKI_VERTEX_TO_PIXEL_GET( TIKI_BINORMAL0 );
	float2 texCoord	= TIKI_VERTEX_TO_PIXEL_GET( TIKI_TEXCOORD0 );

	float3 diffuseColor		= TIKI_TEX2D( t_diffuseMap, s_linear, texCoord ).rgb;
	float3 normalSample		= TIKI_TEX2D( t_normalMap, s_linear, texCoord ).rgb * 2.0 - 1.0;
	float3 specularColor	= TIKI_TEX2D( t_specularMap, s_linear, texCoord ).rgb;
	float3 glossColor		= TIKI_TEX2D( t_glossMap, s_linear, texCoord ).rgb;
		
	float3x3 tangentSpaceNormalMatrix = float3x3( tangent, binormal, normal );
	float3 finalNormal = normalize( TIKI_MUL( normalSample, tangentSpaceNormalMatrix ) );
	//float2 packedNormal = encodeNormal( finalNormal );

	//diffuseColor = binormal;

	TIKI_PIXEL_OUTPUT_SET( TIKI_OUTPUT_COLOR0, createGeometryBuffer0Color( diffuseColor, c_instanceData.selfIlluminationFactor ) );
	TIKI_PIXEL_OUTPUT_SET( TIKI_OUTPUT_COLOR1, createGeometryBuffer1Color( specularColor, c_instanceData.specluarIntensity ) );
	TIKI_PIXEL_OUTPUT_SET( TIKI_OUTPUT_COLOR2, createGeometryBuffer2Color( finalNormal, c_instanceData.specluarPower ) );
	
	TIKI_PIXEL_OUTPUT_END( GeometryBufferPixelOutput );
}

#else

#	error Shader type not supported.

#endif
