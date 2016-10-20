// vs-features= ps-features=TIKI_VISUALIZATION_MODE[4]

#include "shader/platform.fxh"

// vertex to pixel
TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( VertexToPixel )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float4, TIKI_OUTPUT_POSITION0 )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float2, TIKI_TEXCOORD0 )
TIKI_VERTEX_TO_PIXEL_DEFINITION_END( VertexToPixel )

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )

// vertex input
TIKI_VERTEX_INPUT_DEFINITION_BEGIN( VertexInput )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 0, float2, TIKI_INPUT_POSITION0 )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 1, float2, TIKI_TEXCOORD0 )
TIKI_VERTEX_INPUT_DEFINITION_END( VertexInput )

TIKI_ENTRY_POINT( VertexInput, VertexToPixel, main )
{
    TIKI_VERTEX_TO_PIXEL_BEGIN( VertexToPixel );

	float4 position = float4( TIKI_VERTEX_INPUT_GET( TIKI_INPUT_POSITION0 ), 0.0, 1.0 );
	float2 texCoord = TIKI_VERTEX_INPUT_GET( TIKI_TEXCOORD0 );

	TIKI_VERTEX_TO_PIXEL_SET_POSITION( TIKI_OUTPUT_POSITION0, position );
	TIKI_VERTEX_TO_PIXEL_SET( TIKI_TEXCOORD0, texCoord );
    
    TIKI_VERTEX_TO_PIXEL_END( VertexToPixel );
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

#include "shader/functions.fxh"
#include "shader/visualization_shader.hpp"

// pixel output
TIKI_PIXEL_OUTPUT_DEFINITION_BEGIN( PixelOutput )
	TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( 0, float4, TIKI_OUTPUT_COLOR0 )
TIKI_PIXEL_OUTPUT_DEFINITION_END( PixelOutput )

// constants
TIKI_DEFINE_TEXTURE2D( 0, t_gBuffer0 );
TIKI_DEFINE_TEXTURE2D( 1, t_gBuffer1 );
TIKI_DEFINE_TEXTURE2D( 2, t_gBuffer2 );

TIKI_DEFINE_SAMPLER( 0, s_linear );

TIKI_ENTRY_POINT( VertexToPixel, PixelOutput, main )
{
	TIKI_PIXEL_OUTPUT_BEGIN( PixelOutput );

	float2 texCoord = TIKI_VERTEX_TO_PIXEL_GET( TIKI_TEXCOORD0 );

	float4 color = float4( 0.0, 0.0, 0.0, 1.0 );
#if TIKI_VISUALIZATION_MODE == TIKI_VISUALIZATION_MODE_DIFFUSE
	color.rgb = TIKI_TEX2D( t_gBuffer0, s_linear, texCoord ).rgb;
#elif TIKI_VISUALIZATION_MODE == TIKI_VISUALIZATION_MODE_SELFILLUMINATION
	color.rgb = TIKI_TEX2D( t_gBuffer1, s_linear, texCoord ).rgb;
#elif TIKI_VISUALIZATION_MODE == TIKI_VISUALIZATION_MODE_NORMAL
	color.rgb = TIKI_TEX2D( t_gBuffer2, s_linear, texCoord ).rgb;
#else
	color.r = 1.0;
#endif

	TIKI_PIXEL_OUTPUT_SET( TIKI_OUTPUT_COLOR0, TIKI_SATURATE( color ) );
	TIKI_PIXEL_OUTPUT_END( PixelOutput );
}

#else

#	error Shader type not supported.

#endif
