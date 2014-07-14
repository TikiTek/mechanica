// vs-features= ps-features=TIKI_DOWNSAMPLE

#include "shader/platform.fxh"

// vertex to pixel
TIKI_VERTEX_TO_PIXEL_DEFINITION_BEGIN( VertexToPixel )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float4, TIKI_OUTPUT_POSITION0 )
	TIKI_VERTEX_TO_PIXEL_DEFINITION_ELEMENT( float2, TIKI_TEXCOORD0 )
TIKI_VERTEX_TO_PIXEL_DEFINITION_END( VertexToPixel )

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

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

// pixel output
TIKI_PIXEL_OUTPUT_DEFINITION_BEGIN( PixelOutput )
	TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( 0, float4, TIKI_OUTPUT_COLOR0 )
TIKI_PIXEL_OUTPUT_DEFINITION_END( PixelOutput )

#include "shader/positionspace.fxh"
#include "shader/geometrybuffer.fxh"
#include "shader/ascii_shader.hpp"

// constants
TIKI_DEFINE_SAMPLER( 0, s_samplerLinear )
TIKI_DEFINE_SAMPLER( 1, s_samplerNearst )

#if TIKI_DOWNSAMPLE

TIKI_DEFINE_TEXTURE2D( 0, t_accumulationBuffer )
TIKI_DEFINE_TEXTURE2D( 1, t_depthBuffer )

TIKI_DEFINE_CONSTANT( 0, AsciiPixelConstantData, c_pixelData )

#else

TIKI_DEFINE_TEXTURE2D( 0, t_downsample )
TIKI_DEFINE_TEXTURE3D( 1, t_ascii )

#endif

TIKI_ENTRY_POINT( VertexToPixel, PixelOutput, main )
{
	TIKI_PIXEL_OUTPUT_BEGIN( PixelOutput );

	float2 texCoord = TIKI_VERTEX_TO_PIXEL_GET( TIKI_TEXCOORD0 );

#if TIKI_DOWNSAMPLE
	float4 projectedPosition = float4( reconstructClipSpacePosition( texCoord ), t_depthBuffer.Sample( s_samplerNearst, texCoord ).r, 1.0 );
	float4 viewPosition2 = TIKI_MUL( projectedPosition, c_pixelData.inverseProjection );
	float3 viewPosition = viewPosition2.xyz / viewPosition2.w;

	float2 screenSize = getScreenSize( c_pixelData );
	
    // directional search matrix.
    float3x3 GX = float3x3( -1.0, 0.0, 1.0, -2.0, 0.0, 2.0, -1.0,  0.0,  1.0 );
    float3x3 GY = float3x3(  1.0, 2.0, 1.0,  0.0, 0.0, 0.0, -1.0, -2.0, -1.0 );

    float4 fTotalSum = float4( 0.0, 0.0, 0.0, 0.0 );
	
    // Findout X , Y index of incoming pixel
	float fWidth = screenSize.x / 4.0;
	float fHeight = screenSize.y / 4.0;

    // from its texture coordinate.
    float fXIndex = texCoord.x * fWidth;
    float fYIndex = texCoord.y * fHeight;
    if( !( fYIndex < 1.0 || fYIndex > fHeight - 1.0 || fXIndex < 1.0 || fXIndex > fWidth - 1.0 ) )
    {
		float4 sumX		= float4( 0.0, 0.0, 0.0, 0.0 );
		float4 sumY		= float4( 0.0, 0.0, 0.0, 0.0 );

	    for(float I=-1.0; I <= 1.0; I = I + 1.0)
        {
            for(float J=-1.0; J <= 1.0; J = J + 1.0)
            {
                float texCoordX = ( fXIndex + I + 0.5f ) / fWidth;
                float texCoordY = ( fYIndex + J + 0.5f ) / fHeight;

				float valueX = GX[int(I+1.0)][int(J+1.0)];
				float valueY = GY[int(I+1.0)][int(J+1.0)];
                float4 sampleX = TIKI_TEX2D( t_accumulationBuffer, s_samplerLinear, float2( texCoordX, texCoordY ) );
                float4 sampleY = TIKI_TEX2D( t_accumulationBuffer, s_samplerLinear, float2( texCoordX, texCoordY ) );
                sumX += sampleX * valueX.xxxx;
                sumY += sampleY * valueY.xxxx;
            }
        }

        float4 fTem = sumX * sumX + sumY * sumY;
        fTotalSum = sqrt( fTem );
    }
	
	float edgeAlpha = dot( fTotalSum, 1.0 ) / 8.0;
	float4 color = TIKI_TEX2D( t_accumulationBuffer, s_samplerLinear, texCoord );
	color.a = ( viewPosition.z / 10.0 ) + ( edgeAlpha * 2.0 );
#else
	float4 color = float4( TIKI_TEX2D( t_downsample, s_samplerNearst, texCoord ) );

	float3 uvw = float3( texCoord, 0.0 );
	uvw.x *= 105.0;
	uvw.y *= 60.0;
	uvw.z = color.a;

	float ascii = TIKI_TEX3D( t_ascii, s_samplerLinear, uvw.xyz ).r;
	color = float4( color.xyz * ascii, 1.0 );
#endif

	TIKI_PIXEL_OUTPUT_SET( TIKI_OUTPUT_COLOR0, color );
	TIKI_PIXEL_OUTPUT_END( PixelOutput );
}

#else

#	error Shader type not supported.

#endif
