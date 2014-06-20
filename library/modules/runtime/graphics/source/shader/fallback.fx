// vs-features= ps-features=

#include "shader/platform.fxh"

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
TIKI_VERTEX_INPUT_DEFINITION_END( VertexInput )

struct VertexConstants
{
	float4x4	mvpMatrix;
	float4x4	modelMatrix;
	float4x4	modelViewMatrix;
};

// constants
TIKI_DEFINE_CONSTANT( 0, VertexConstants, c_instanceData )

TIKI_ENTRY_POINT( VertexInput, VertexToPixel, main )
{
    TIKI_VERTEX_TO_PIXEL_BEGIN( VertexToPixel );

	float3x3 normalMatrix = float3x3( c_instanceData.modelMatrix );

	float4 position = float4( TIKI_VERTEX_INPUT_GET( position ), 1.0 );
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

// pixel output
TIKI_PIXEL_OUTPUT_DEFINITION_BEGIN( PixelOutput )
	TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( 0, float4, color, TIKI_OUTPUT_COLOR )
TIKI_PIXEL_OUTPUT_DEFINITION_END( PixelOutput )

// constants
//TIKI_DEFINE_CONSTANT( 0, PixelConstants, c_instanceData )

TIKI_DEFINE_TEXTURE2D( 0, t_diffuseMap )
TIKI_DEFINE_SAMPLER( 0, s_linear )

TIKI_ENTRY_POINT( VertexToPixel, PixelOutput, main )
{
	TIKI_PIXEL_OUTPUT_BEGIN( PixelOutput );

	float2 texCoord = TIKI_VERTEX_TO_PIXEL_GET( texCoord );

	float4 color = TIKI_TEX2D( t_diffuseMap, s_linear, texCoord );
	color = TIKI_SATURATE( color );

	TIKI_PIXEL_OUTPUT_SET( color, color );
	TIKI_PIXEL_OUTPUT_END( PixelOutput );
}

#else

#	error Shader type not supported.

#endif
