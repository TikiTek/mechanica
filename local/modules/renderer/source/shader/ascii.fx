// vs-features= ps-features=TIKI_DOWNSAMPLE

#include "shader/platform.fxh"
#include "shader/geometrybuffer.fxh"

struct VertexToPixel
{
	float4 position	: TIKI_OUTPUT_POSITION0;
	float2 texCoord	: TIKI_TEXCOORD;
};

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

// types
struct VertexInput
{
	float2 position	: TIKI_INPUT_POSITION0;
	float2 texCoord	: TIKI_TEXCOORD;
};

VertexToPixel main( VertexInput input )
{
    VertexToPixel output;

    output.position = float4( input.position, 0.0f, 1.0f );
	output.texCoord = input.texCoord;
    
    return output;
}

#elif TIKI_ENABLED( TIKI_PIXEL_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////

#include "shader/functions.fxh"
#include "shader/ascii_shader.hpp"

// constants
TIKI_DEFINE_SAMPLER( 0, s_samplerLinear );
TIKI_DEFINE_SAMPLER( 1, s_samplerNearst );

#if TIKI_DOWNSAMPLE

TIKI_DEFINE_TEXTURE2D( 0, t_gBuffer0 );
TIKI_DEFINE_TEXTURE2D( 1, t_gBuffer2 );
TIKI_DEFINE_TEXTURE2D( 2, t_depthBuffer );

TIKI_DEFINE_CONSTANT( 0, AsciiPixelConstantData, c_pixelData );

#else

TIKI_DEFINE_TEXTURE2D( 0, t_downsample );
TIKI_DEFINE_TEXTURE3D( 1, t_ascii );

#endif

float4 main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
#if TIKI_DOWNSAMPLE
	float4 projectedPosition = getProjectedPosition( input.texCoord, t_depthBuffer.Sample( s_samplerNearst, input.texCoord ).r );
	float3 viewPosition = reconstructViewSpacePosition( projectedPosition, c_pixelData.inverseProjection );

	float2 screenSize = getScreenSize( c_pixelData );
	
    // directional search matrix.
    float3x3 GX = float3x3( -1.0f, 0.0f, 1.0f, -2.0f, 0.0f, 2.0f, -1.0f,  0.0f,  1.0f );
    float3x3 GY = float3x3(  1.0f, 2.0f, 1.0f,  0.0f, 0.0f, 0.0f, -1.0f, -2.0f, -1.0f );

    float4 fTotalSum	= float4( 0.0f, 0.0f, 0.0f, 0.0f );
	
    // Findout X , Y index of incoming pixel
	float fWidth = screenSize.x / 4;
	float fHeight = screenSize.y / 4;

    // from its texture coordinate.
    float fXIndex = input.texCoord.x * fWidth;
    float fYIndex = input.texCoord.y * fHeight;

    /* image boundaries Top, Bottom, Left, Right pixels*/
    if( ! ( fYIndex < 1.0 || fYIndex > fHeight - 1.0 || fXIndex < 1.0 || fXIndex > fWidth - 1.0 ) )
    {
		float4 fSumX		= float4( 0.0f, 0.0f, 0.0f, 0.0f );
		float4 fSumY		= float4( 0.0f, 0.0f, 0.0f, 0.0f );

	    for(float I=-1.0; I <= 1.0f; I = I + 1.0f)
        {
            for(float J=-1.0; J <= 1.0f; J = J + 1.0f)
            {
                float fTempX = ( fXIndex + I + 0.5f ) / fWidth ;
                float fTempY = ( fYIndex + J + 0.5f ) / fHeight ;
                float4 fTempSumX = TIKI_TEX2D( t_gBuffer0, s_samplerLinear, float2( fTempX, fTempY ) );
                fSumX = fSumX + ( fTempSumX * float4( GX[int(I+1.0)][int(J+1.0)],
                                                    GX[int(I+1.0)][int(J+1.0)],
                                                    GX[int(I+1.0)][int(J+1.0)],
                                                    GX[int(I+1.0)][int(J+1.0)]));

                float4 fTempSumY = TIKI_TEX2D( t_gBuffer0, s_samplerLinear, float2( fTempX, fTempY ) );
                fSumY = fSumY + ( fTempSumY * float4( GY[int(I+1.0)][int(J+1.0)],
                                                    GY[int(I+1.0)][int(J+1.0)],
                                                    GY[int(I+1.0)][int(J+1.0)],
                                                    GY[int(I+1.0)][int(J+1.0)]));
            }
        }

        float4 fTem = fSumX * fSumX + fSumY * fSumY;
        fTotalSum = sqrt( fTem );
    }
	
	float edgeAlpha = ( fTotalSum.x + fTotalSum.y + fTotalSum.z ) / 6.0f;
	float4 color = t_gBuffer0.Sample( s_samplerLinear, input.texCoord );
	color.a = viewPosition.z + edgeAlpha;
#else
	float4 color = t_downsample.Sample( s_samplerNearst, input.texCoord );

	float3 uvw = float3( input.texCoord, 0.0f );
	uvw.x *= 105.0f;
	uvw.y *= 60.0f;
	uvw.z = color.a;

	float ascii = t_ascii.Sample( s_samplerLinear, uvw ).r;
	color = float4( color.rgb * ascii.rrr, 1.0f);
#endif

	return color;
}

#else

#	error Shader type not supported.

#endif
