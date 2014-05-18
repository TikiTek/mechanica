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

#include "shader/positionspace.fxh"
#include "shader/ascii_shader.hpp"

// constants
TIKI_DEFINE_SAMPLER( 0, s_samplerLinear );
TIKI_DEFINE_SAMPLER( 1, s_samplerNearst );

#if TIKI_DOWNSAMPLE

TIKI_DEFINE_TEXTURE2D( 0, t_accumulationBuffer );
TIKI_DEFINE_TEXTURE2D( 1, t_depthBuffer );

TIKI_DEFINE_CONSTANT( 0, AsciiPixelConstantData, c_pixelData );

#else

TIKI_DEFINE_TEXTURE2D( 0, t_downsample );
TIKI_DEFINE_TEXTURE3D( 1, t_ascii );

#endif

float4 main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
#if TIKI_DOWNSAMPLE
	float4 projectedPosition = float4( reconstructClipSpacePosition( input.texCoord ), t_depthBuffer.Sample( s_samplerNearst, input.texCoord ).r, 1.0f );
	float4 viewPosition2 = mul( projectedPosition, c_pixelData.inverseProjection );
	float3 viewPosition = viewPosition2.xyz / viewPosition2.w;

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
    if( !( fYIndex < 1.0 || fYIndex > fHeight - 1.0 || fXIndex < 1.0 || fXIndex > fWidth - 1.0 ) )
    {
		float4 sumX		= float4( 0.0f, 0.0f, 0.0f, 0.0f );
		float4 sumY		= float4( 0.0f, 0.0f, 0.0f, 0.0f );

	    for(float I=-1.0; I <= 1.0f; I = I + 1.0f)
        {
            for(float J=-1.0; J <= 1.0f; J = J + 1.0f)
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
	
	float edgeAlpha = dot( fTotalSum, 1.0f ) / 8.0f;
	float4 color = TIKI_TEX2D( t_accumulationBuffer, s_samplerLinear, input.texCoord );
	color.a = ( viewPosition.z / 10.0f ) + ( edgeAlpha * 2.0f );
#else
	float4 color = TIKI_TEX2D( t_downsample, s_samplerNearst, input.texCoord );

	float3 uvw = float3( input.texCoord, 0.0f );
	uvw.x *= 105.0f;
	uvw.y *= 60.0f;
	uvw.z = color.a;

	float ascii = TIKI_TEX3D( t_ascii, s_samplerLinear, uvw ).r;
	color = float4( color.rgb * ascii.rrr, 1.0f);
#endif

	return color;
}

#else

#	error Shader type not supported.

#endif
