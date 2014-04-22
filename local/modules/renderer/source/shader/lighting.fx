// vs-features= ps-features=TIKI_DIRECTION_LIGHT_COUNT[4],TIKI_POINT_LIGHT_COUNT[8],TIKI_SPOT_LIGHT_COUNT

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
	float3 diffuseColor			= getDiffuseColor( sample );
	float3 normal				= getNormal( sample );

	float4 projectedPosition	= getProjectedPosition( input.texCoord, TIKI_TEX2D( t_depthBuffer, s_samplerNearst, input.texCoord ).r );
	float3 viewPosition			= reconstructViewSpacePosition( projectedPosition, c_pixelData.inverseProjection );
	float3 worldPosition		= reconstructWorldSpacePosition( projectedPosition, c_pixelData.inverseProjection );

	float3 color = float3( 0.0f, 0.0f, 0.0f );
	for (int i = 0; i < TIKI_DIRECTION_LIGHT_COUNT; i++)
	{
		float3 lightDirection	= getDirectionalLightDirection( c_pixelData.directionalLights[ i ] );
		float lightIntensity	= max( dot( normal, lightDirection ), 0 );
		float3 lightColor		= lightIntensity * getDirectionalLightColor( c_pixelData.directionalLights[ i ] );
  
		float3 specularColor	= getSpecluarBrightness( sample ).xxx;
		float specularIntensity	= getSpecluarIntensity( sample );
		float specularPower		= getSpecluarPower( sample );

		float3 H = normalize( lightDirection - viewPosition );
		float3 specularLight	= lightIntensity * specularIntensity * specularColor * max( pow( saturate( dot( H, normal ) ), specularPower ), 0 );

		color += ( diffuseColor * lightColor ) + specularLight;
	}

	for (int i = 0; i < TIKI_POINT_LIGHT_COUNT; i++)
	{
		float3 lightPosition	= getPointLightPosition( c_pixelData.pointLights[ i ] );
		float3 lightDistance	= lightPosition - worldPosition;
		float3 lightDirection	= normalize( lightDistance );
		float lightIntensity	= max( dot( normal, lightDirection ), 0 );
		float3 lightColor		= lightIntensity * getPointLightColor( c_pixelData.pointLights[ i ] );
  
		float3 specularColor	= getSpecluarBrightness( sample ).xxx;
		float specularIntensity	= getSpecluarIntensity( sample );
		float specularPower		= getSpecluarPower( sample );

		float attenuation		= 1.0f - saturate( length( lightDistance ) * getPointLightInverseRange( c_pixelData.pointLights[ i ] ) );
		float3 H				= normalize( lightDirection - worldPosition );
		float3 specularLight	= lightIntensity * specularIntensity * specularColor * max( pow( saturate( dot( H, normal ) ), specularPower ), 0 );

		color += ( ( diffuseColor * lightColor ) + specularLight ) * attenuation;
	}

	// spot
	//float angle = acos(dot(-input.LightDir, normalize(LightDir)));
	//float lightSpot = saturate(smoothstep(radians(phi), radians(theta), angle));
	//float lightNormal = dot(input.Normal, input.LightDir);
	//float lightAttenuation = saturate(1 - dot(input.LightDir / LightRange, input.LightDir / LightRange));
	//
	//float4 termDiffuse = tex1.Sample(sam, input.UV) * diffuseIntensity * lightNormal * LightColor * lightSpot * lightAttenuation;
	
	return float4( color, 1.0f );
}

#else

#	error Shader type not supported.

#endif
