// vs-features= ps-features=TIKI_FONT_MODE,TIKI_COLOR_MODE

#include "tiki/graphics/shader/platform.fxh"
#include "tiki/graphics/shader/immediaterenderer_shader.hpp"

TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( VertexToPixel )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float4, TIKI_OUTPUT_POSITION0 )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float2, TIKI_TEXCOORD0 )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float4, TIKI_COLOR0 )
TIKI_VERTEX_TO_PIXEL_DEFINITION_END( VertexToPixel )

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

TIKI_VERTEX_INPUT_DEFINITION_BEGIN( VertexInput )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 0, float3, TIKI_INPUT_POSITION0 )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 1, float2, TIKI_TEXCOORD0 )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 2, float4, TIKI_COLOR0 )
TIKI_VERTEX_INPUT_DEFINITION_END( VertexInput )

TIKI_DEFINE_CONSTANT( 0, ImmediateRendererConstantData, s_constantData )

TIKI_ENTRY_POINT( VertexInput, VertexToPixel, main )
{
    TIKI_VERTEX_TO_PIXEL_BEGIN( VertexToPixel );

	float4 position = float4( TIKI_VERTEX_INPUT_GET( TIKI_INPUT_POSITION0 ), 1.0 );
	float2 texCoord = TIKI_VERTEX_INPUT_GET( TIKI_TEXCOORD0 );
	float4 color	= TIKI_VERTEX_INPUT_GET( TIKI_COLOR0 );

	position = TIKI_MUL( position, s_constantData.projection );
	
	TIKI_VERTEX_TO_PIXEL_SET_POSITION( TIKI_OUTPUT_POSITION0, position );
	TIKI_VERTEX_TO_PIXEL_SET( TIKI_TEXCOORD0, texCoord );
	TIKI_VERTEX_TO_PIXEL_SET( TIKI_COLOR0, color );

    TIKI_VERTEX_TO_PIXEL_END( VertexToPixel );
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

TIKI_PIXEL_OUTPUT_DEFINITION_BEGIN( PixelOutput )
	TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( 0, float4, TIKI_OUTPUT_COLOR0 )
TIKI_PIXEL_OUTPUT_DEFINITION_END( PixelOutput )

// constants
TIKI_DEFINE_TEXTURE2D( 0, t_texture )
TIKI_DEFINE_SAMPLER( 0, s_sampler )

TIKI_ENTRY_POINT( VertexToPixel, PixelOutput, main )
{
	TIKI_PIXEL_OUTPUT_BEGIN( PixelOutput );

#if TIKI_COLOR_MODE == 0
	float2 texCoord = TIKI_VERTEX_TO_PIXEL_GET( TIKI_TEXCOORD0 );
	float4 color = TIKI_TEX2D( t_texture, s_sampler, texCoord );
#else
	float4 color = TIKI_VERTEX_TO_PIXEL_GET( TIKI_COLOR0 );
#endif
	
#if TIKI_FONT_MODE
	float4 inputColor = TIKI_VERTEX_TO_PIXEL_GET( TIKI_COLOR0 );

	color.a		= color.r * inputColor.a;
	color.rgb	= inputColor.rgb;
#elif TIKI_COLOR_MODE == 0
	color *= TIKI_VERTEX_TO_PIXEL_GET( TIKI_COLOR0 );
#endif

	TIKI_PIXEL_OUTPUT_SET( TIKI_OUTPUT_COLOR0, color );
	TIKI_PIXEL_OUTPUT_END( PixelOutput );
}

#else

#	error Shader type not supported.

#endif
