// vs-features= ps-features=TIKI_FONT_MODE,TIKI_COLOR_MODE

#include "shader/platform.fxh"
#include "tiki/graphics/immediaterenderer_shader.hpp"

TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( VertexToPixel )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float4, position,	TIKI_OUTPUT_POSITION )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float2, texCoord,	TIKI_TEXCOORD )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float4, color,		TIKI_COLOR )
TIKI_VERTEX_TO_PIXEL_DEFINITION_END( VertexToPixel )

//struct VertexToPixel
//{
//	float4 position	: TIKI_OUTPUT_POSITION0;
//	float2 texcoord	: TIKI_TEXCOORD;
//	float4 color	: TIKI_COLOR;
//};

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

TIKI_VERTEX_INPUT_DEFINITION_BEGIN( VertexInput )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 0, float3, position,	TIKI_INPUT_POSITION )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 1, float2, texCoord,	TIKI_TEXCOORD )
	TIKI_VERTEX_INPUT_DEFINITION_ELEMENT( 3, float4, color,		TIKI_COLOR )
TIKI_VERTEX_INPUT_DEFINITION_END( VertexInput )

//struct VertexInput
//{
//	float3 position	: TIKI_INPUT_POSITION0;
//	float2 texcoord	: TIKI_TEXCOORD;
//	float4 color	: TIKI_COLOR;
//};

TIKI_DEFINE_CONSTANT( 0, ImmediateRendererConstantData, s_constantData )

TIKI_ENTRY_POINT( VertexInput, VertexToPixel, main )
{
    TIKI_VERTEX_TO_PIXEL_BEGIN( VertexToPixel );

	float4 position = float4( TIKI_VERTEX_INPUT_GET( position ), 1.0 );
	float2 texCoord = TIKI_VERTEX_INPUT_GET( texCoord );
	float4 color	= TIKI_VERTEX_INPUT_GET( color );

	position = TIKI_MUL( position, s_constantData.projection );
	
	TIKI_VERTEX_TO_PIXEL_SET_POSITION( position, position );
	TIKI_VERTEX_TO_PIXEL_SET( texCoord, texCoord );
	TIKI_VERTEX_TO_PIXEL_SET( color, color );

 //   output.position = float4( input.position, 1.0f );
	//output.position = mul( output.position, s_constantData.projection );
	//output.texcoord = input.texcoord;
	//output.color	= input.color;
    
    TIKI_VERTEX_TO_PIXEL_END( VertexToPixel );
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

TIKI_PIXEL_OUTPUT_DEFINITION_BEGIN( PixelOutput )
	TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( 0, float4, color, TIKI_OUTPUT_COLOR )
TIKI_PIXEL_OUTPUT_DEFINITION_END( PixelOutput )

// constants
TIKI_DEFINE_TEXTURE2D( 0, t_texture )
TIKI_DEFINE_SAMPLER( 0, s_sampler )

TIKI_ENTRY_POINT( VertexToPixel, PixelOutput, main )
{
	TIKI_PIXEL_OUTPUT_BEGIN( PixelOutput );

#if TIKI_COLOR_MODE == 0
	float2 texCoord = TIKI_VERTEX_TO_PIXEL_GET( texCoord );
	float4 color = TIKI_TEX2D( t_texture, s_sampler, texCoord );
#else
	float4 color = TIKI_VERTEX_TO_PIXEL_GET( color );
#endif
	
#if TIKI_FONT_MODE
	float4 inputColor = TIKI_VERTEX_TO_PIXEL_GET( color );

	color.a		= color.r * inputColor.a;
	color.rgb	= inputColor.rgb;
#elif TIKI_COLOR_MODE == 0
	color *= TIKI_VERTEX_TO_PIXEL_GET( color );
#endif

	TIKI_PIXEL_OUTPUT_SET( color, color );
	TIKI_PIXEL_OUTPUT_END( PixelOutput );
}

#else

#	error Shader type not supported.

#endif
