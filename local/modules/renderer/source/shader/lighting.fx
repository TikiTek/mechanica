// vs-features= ps-features=

#include "shader/platform.fxh"

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
#include "shader/geometrybuffer.fxh"

#include "shader/lighting_shader.hpp"

// constants
TIKI_DEFINE_SAMPLER( 0, s_samplerNearst );

TIKI_DEFINE_TEXTURE2D( 0, t_gBuffer0 );
TIKI_DEFINE_TEXTURE2D( 1, t_gBuffer1 );
TIKI_DEFINE_TEXTURE2D( 2, t_gBuffer2 );
TIKI_DEFINE_TEXTURE2D( 3, t_depthBuffer );

TIKI_DEFINE_CONSTANT( 0, LightingPixelConstantData, c_pixelData );

float4 main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
	GeometryBufferSample sample = sampleGeometryBuffer( t_gBuffer0, t_gBuffer1, t_gBuffer2, s_samplerNearst, input.texCoord );

	float3 lightDirection = getLightDirection( c_pixelData );

	float3 color = getDiffuseColor( sample );
	float3 normal = getNormal( sample );
	float lightIntensity = max( dot( normal, lightDirection ), 0 );
	float3 light = lightIntensity * getLightColor( c_pixelData );
  
	float4 projectedPosition = getProjectedPosition( input.texCoord, t_depthBuffer.Sample( s_samplerNearst, input.texCoord ).r );
	float3 viewPosition = reconstructViewSpacePosition( projectedPosition, c_pixelData.inverseProjection );

	float3 specularColor	= getSpecluarBrightness( sample ).xxx;
	float specularIntensity	= getSpecluarIntensity( sample );
	float specularPower		= getSpecluarPower( sample );

	float3 H = normalize( lightDirection - viewPosition );
	float3 specular = lightIntensity * specularIntensity * specularColor * max( pow( saturate( dot( H, normal ) ), specularPower ), 0 );

	return float4( ( color * light ) + specular, 1.0f );
}

#else

#	error Shader type not supported.

#endif
