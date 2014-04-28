// vs-features= ps-features=TIKI_DIRECTION_LIGHT_COUNT[2],TIKI_POINT_LIGHT_COUNT[8],TIKI_SPOT_LIGHT_COUNT[2]

#include "shader/platform.fxh"

struct VertexToPixel
{
	float4 position	: TIKI_OUTPUT_POSITION;
	float2 texCoord	: TIKI_TEXCOORD;
};

#if TIKI_ENABLED( TIKI_VERTEX_SHADER )
////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

// types
struct VertexInput
{
	float2 position	: TIKI_INPUT_POSITION;
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
#include "shader/positionspace.fxh"

#include "shader/lighting_shader.hpp"
#include "shader/cameraparameter.hpp"

// constants
TIKI_DEFINE_SAMPLER( 0, s_samplerNearst );

TIKI_DEFINE_TEXTURE2D( 0, t_gBuffer0 );
TIKI_DEFINE_TEXTURE2D( 1, t_gBuffer1 );
TIKI_DEFINE_TEXTURE2D( 2, t_gBuffer2 );
TIKI_DEFINE_TEXTURE2D( 3, t_depthBuffer );

TIKI_DEFINE_CONSTANT( 0, LightingPixelConstantData, c_pixelData );
TIKI_DEFINE_CONSTANT( 1, CameraParameter, c_cameraParameter );

//float GGX_V1(in float m2, in float nDotX)
//{
//    return 1.0f / (nDotX + sqrt(m2 + (1 - m2) * nDotX * nDotX));
//}
//
//float GGX_Specular(in float m, in float3 n, in float3 h, in float3 v, in float3 l)
//{
//    float nDotH = saturate(dot(n, h));
//    float nDotL = saturate(dot(n, l));
//    float nDotV = saturate(dot(n, v));
//
//    float nDotH2 = nDotH * nDotH;
//    float m2 = m * m;
//
//    // Calculate the distribution term
//    float d = m2 / (3.14159f * pow(nDotH * nDotH * (m2 - 1) + 1, 2.0f));
//
//    // Calculate the matching visibility term
//    float v1i = GGX_V1(m2, nDotL);
//    float v1o = GGX_V1(m2, nDotV);
//    float vis = v1i * v1o;
//
//    return d * vis;
//}

float4 main( VertexToPixel input ) : TIKI_OUTPUT_COLOR
{
	GeometryBufferSample sample = sampleGeometryBuffer( t_gBuffer0, t_gBuffer1, t_gBuffer2, s_samplerNearst, input.texCoord );
	float3 diffuseColor			= getDiffuseColor( sample );
	float3 normal				= getNormal( sample );

	float nonLinearDepth		= TIKI_TEX2D( t_depthBuffer, s_samplerNearst, input.texCoord ).x;
	float4 projectionPosition	= reconstructProjectionSpacePosition( input.texCoord, nonLinearDepth );
	float3 worldPosition		= reconstructWorldSpacePosition( projectionPosition, c_cameraParameter );
	float3 viewDirection		= normalize( getCameraWorldPosition( c_pixelData ) - worldPosition );

	float4 viewPosition2 = mul( projectionPosition, c_pixelData.inverseProjection );
	float3 viewPosition = viewPosition2.xyz / viewPosition2.w;

	float3 color = float3( 0.0f, 0.0f, 0.0f );

	// directional
	for (int i = 0; i < TIKI_DIRECTION_LIGHT_COUNT; i++)
	{
		float3 lightDirection	= getDirectionalLightDirection( c_pixelData.directionalLights[ i ] );
		float lightIntensity	= max( dot( normal, lightDirection ), 0 );
		float3 lightColor		= lightIntensity * getDirectionalLightColor( c_pixelData.directionalLights[ i ] );
		float3 ambientColor		= ( 1 - lightIntensity ) * float3( 0.5f, 0.25f, 0.25f );
		float ambientIntensity	= 0.1f;
  
		float3 specularColor	= getSpecluarBrightness( sample ).xxx;
		float specularIntensity	= getSpecluarIntensity( sample );
		float specularPower		= getSpecluarPower( sample );

		float3 halfVector		= normalize( viewDirection + lightDirection );
		float3 specularLight	= float3( 0.0f, 0.0f, 0.0f ); //lightIntensity * specularIntensity * specularColor * max( GGX_Specular( specularPower, normal, halfVector, viewDirection, lightDirection ), 0 );

		color += ( diffuseColor * lightColor ) + specularLight;
		color += diffuseColor * ambientColor * ambientIntensity;
	}

	// point
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
		float3 halfVector		= normalize( viewDirection + lightDirection );
		float3 specularLight	= float3( 0.0f, 0.0f, 0.0f ); //lightIntensity * specularIntensity * specularColor * max( GGX_Specular( specularPower, normal, halfVector, viewDirection, lightDirection ), 0 );

		color += ( ( diffuseColor * lightColor ) + specularLight ) * attenuation;
	}

	// spot
	for (int i = 0; i < TIKI_SPOT_LIGHT_COUNT; i++)
	{
		float3 lightPosition	= getSpotLightPosition( c_pixelData.spotLights[ i ] );
		float3 lightDistance	= lightPosition - worldPosition;
		float3 lightDirection	= normalize( lightDistance );
		float lightIntensity	= max( dot( normal, lightDirection ), 0 );
		float3 lightColor		= lightIntensity * getSpotLightColor( c_pixelData.spotLights[ i ] );

		float angle				= acos( dot( lightDirection, normalize( getSpotLightDirection( c_pixelData.spotLights[ i ] ) ) ) );
		float lightSpot			= saturate( smoothstep( getSpotLightPhi( c_pixelData.spotLights[ i ] ), getSpotLightTheta( c_pixelData.spotLights[ i ] ), angle ) );
  
		float3 specularColor	= getSpecluarBrightness( sample ).xxx;
		float specularIntensity	= getSpecluarIntensity( sample );
		float specularPower		= getSpecluarPower( sample );

		float attenuation		= 1.0f - saturate( length( lightDistance ) * getSpotLightInverseRange( c_pixelData.spotLights[ i ] ) );
		float3 halfVector		= normalize( viewDirection + lightDirection );
		float3 specularLight	= float3( 0.0f, 0.0f, 0.0f ); //lightIntensity * specularIntensity * specularColor * max( GGX_Specular( specularPower, normal, halfVector, viewDirection, lightDirection ), 0 );

		color += ( ( diffuseColor * lightColor ) + specularLight ) * lightSpot * attenuation;
	}

	return float4( color, 1.0f );
}

#else

#	error Shader type not supported.

#endif
