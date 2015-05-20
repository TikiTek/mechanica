// vs-features= ps-features=

#include "shader/platform.fxh"
#include "shader/skybox_shader.hpp"

// vertex to pixel
TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( VertexToPixel )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float4, TIKI_OUTPUT_POSITION0 )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float3, TIKI_TEXCOORD0 )
TIKI_VERTEX_TO_PIXEL_DEFINITION_END( VertexToPixel )

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

// vertex input
TIKI_VERTEX_INPUT_DEFINITION_BEGIN( VertexInput )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 0, float3, TIKI_INPUT_POSITION0 )
TIKI_VERTEX_INPUT_DEFINITION_END( VertexInput )

// constants
TIKI_DEFINE_CONSTANT( 0, SkyboxVertexConstantData, c_instanceData )

TIKI_ENTRY_POINT( VertexInput, VertexToPixel, main )
{
    TIKI_VERTEX_TO_PIXEL_BEGIN( VertexToPixel );

	float4 position = float4( TIKI_VERTEX_INPUT_GET( TIKI_INPUT_POSITION0 ), 1.0 );
	float3 texCoord = position.xyz;

	//position = TIKI_MUL( position, c_instanceData.mvpMatrix );
	
	TIKI_VERTEX_TO_PIXEL_SET_POSITION( TIKI_OUTPUT_POSITION0, position );
	TIKI_VERTEX_TO_PIXEL_SET( TIKI_TEXCOORD0, texCoord );

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
TIKI_DEFINE_CONSTANT( 0, SkyboxPixelConstantData, c_instanceData )

TIKI_DEFINE_TEXTURECUBE( 0, t_diffuseMap )
TIKI_DEFINE_SAMPLER( 0, s_linear )

TIKI_ENTRY_POINT( VertexToPixel, GeometryBufferPixelOutput, main )
{
	TIKI_PIXEL_OUTPUT_BEGIN( GeometryBufferPixelOutput );

	float3 texCoord	= TIKI_VERTEX_TO_PIXEL_GET( TIKI_TEXCOORD0 );

	float3 diffuseColor = TIKI_TEXCUBE( t_diffuseMap, s_linear, texCoord ).rgb;
	float3 normalSample = normalize( texCoord );
	float3 selfIlluminationColor = float3( 0.0, 0.0, 0.0 );
		
	diffuseColor = 1.0;

	TIKI_PIXEL_OUTPUT_SET( TIKI_OUTPUT_COLOR0, createGeometryBuffer0Color( diffuseColor, 0.0 ) );
	TIKI_PIXEL_OUTPUT_SET( TIKI_OUTPUT_COLOR1, createGeometryBuffer1Color( selfIlluminationColor, 0.0 ) );
	TIKI_PIXEL_OUTPUT_SET( TIKI_OUTPUT_COLOR2, createGeometryBuffer2Color( normalSample, 1.0 ) );
	
	TIKI_PIXEL_OUTPUT_END( GeometryBufferPixelOutput );
}

#else

#	error Shader type not supported.

#endif
