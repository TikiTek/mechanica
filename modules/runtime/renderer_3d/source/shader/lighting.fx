// vs-features= ps-features=TIKI_DIRECTION_LIGHT_COUNT[2],TIKI_POINT_LIGHT_COUNT[8],TIKI_SPOT_LIGHT_COUNT[2]

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

#include "shader/functions.fxh"
#include "shader/geometrybuffer.fxh"
#include "shader/positionspace.fxh"

#include "shader/lighting_shader.hpp"
#include "shader/cameraparameter.hpp"

// pixel output
TIKI_PIXEL_OUTPUT_DEFINITION_BEGIN( PixelOutput )
	TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( 0, float4, TIKI_OUTPUT_COLOR0 )
TIKI_PIXEL_OUTPUT_DEFINITION_END( PixelOutput )

// constants
TIKI_DEFINE_SAMPLER( 0, s_samplerNearst )
TIKI_DEFINE_SAMPLER( 1, s_samplerLinear )

TIKI_DEFINE_TEXTURE2D( 0, t_gBuffer0 )
TIKI_DEFINE_TEXTURE2D( 1, t_gBuffer1 )
TIKI_DEFINE_TEXTURE2D( 2, t_gBuffer2 )
TIKI_DEFINE_TEXTURE2D( 3, t_depthBuffer )
TIKI_DEFINE_TEXTURECUBE( 4, t_reflectionMap )

TIKI_DEFINE_CONSTANT( 0, LightingPixelConstantData, c_pixelData )
TIKI_DEFINE_CONSTANT( 1, CameraParameter, c_cameraParameter )

float GGX_V1(in float m2, in float nDotX)
{
    return 1.0 / (nDotX + sqrt(m2 + (1 - m2) * nDotX * nDotX));
}

float GGX_Specular(in float m, in float3 n, in float3 h, in float3 v, in float3 l)
{
    float nDotH = TIKI_SATURATE(dot(n, h));
    float nDotL = TIKI_SATURATE(dot(n, l));
    float nDotV = TIKI_SATURATE(dot(n, v));

    float nDotH2 = nDotH * nDotH;
    float m2 = m * m;

    // Calculate the distribution term
    float d = m2 / (3.14159f * pow(nDotH * nDotH * (m2 - 1) + 1, 2.0));

    // Calculate the matching visibility term
    float v1i = GGX_V1(m2, nDotL);
    float v1o = GGX_V1(m2, nDotV);
    float vis = v1i * v1o;

    return d * vis;
}

TIKI_ENTRY_POINT( VertexToPixel, PixelOutput, main )
{
	TIKI_PIXEL_OUTPUT_BEGIN( PixelOutput );

	float2 texCoord = TIKI_VERTEX_TO_PIXEL_GET( TIKI_TEXCOORD0 );

	GeometryBufferSample gSample	= sampleGeometryBuffer( t_gBuffer0, t_gBuffer1, t_gBuffer2, s_samplerNearst, texCoord );
	float3 diffuseColor				= getDiffuseColor( gSample );
	float3 normal					= getNormal( gSample );

	float nonLinearDepth		= TIKI_TEX2D( t_depthBuffer, s_samplerNearst, texCoord ).x;
	float4 projectionPosition	= reconstructProjectionSpacePosition( texCoord, nonLinearDepth );
	float3 worldPosition		= reconstructWorldSpacePosition( projectionPosition, c_cameraParameter );
	float3 viewDirection		= normalize( getCameraWorldPosition( c_pixelData ) - worldPosition );

	float4 viewPosition2 = TIKI_MUL( projectionPosition, c_pixelData.inverseProjection );
	float3 viewPosition = viewPosition2.xyz / viewPosition2.w;

	float3 reflectionVector = reflect( -viewDirection, normal );
	float3 reflectionColor = TIKI_TEXCUBE( t_reflectionMap, s_samplerLinear, reflectionVector ).xyz;

	float3 color = float3( 0.0, 0.0, 0.0 );

	float3 specularColor	= getSpecluarColor( gSample );
	float specularIntensity	= getSpecluarIntensity( gSample );
	float specularPower		= getSpecluarPower( gSample );

	// directional
	for (int i = 0; i < TIKI_DIRECTION_LIGHT_COUNT; i++)
	{
		float3 lightDirection	= getDirectionalLightDirection( c_pixelData.directionalLights[ i ] );
		float lightIntensity	= max( dot( normal, lightDirection ), 0 );
		float3 lightColor		= lightIntensity * getDirectionalLightColor( c_pixelData.directionalLights[ i ] );
		float3 ambientColor		= ( 1 - lightIntensity ) * float3( 0.5, 0.25, 0.25 );
		float ambientIntensity	= 0.1f;
  
		float3 halfVector		= normalize( viewDirection + lightDirection );
		float3 specularLight	= lightIntensity * specularIntensity * specularColor * max( GGX_Specular( specularPower, normal, halfVector, viewDirection, lightDirection ), 0 );

		color += ( diffuseColor * lightColor ) + specularLight;
		color += diffuseColor * ambientColor * ambientIntensity;
		//color += lightIntensity;
	}

	// point
	for (int i = 0; i < TIKI_POINT_LIGHT_COUNT; i++)
	{
		float3 lightPosition	= getPointLightPosition( c_pixelData.pointLights[ i ] );
		float3 lightDistance	= lightPosition - worldPosition;
		float3 lightDirection	= normalize( lightDistance );
		float lightIntensity	= max( dot( normal, lightDirection ), 0 );
		float3 lightColor		= lightIntensity * getPointLightColor( c_pixelData.pointLights[ i ] );
  
		float attenuation		= 1.0 - TIKI_SATURATE( length( lightDistance ) * getPointLightInverseRange( c_pixelData.pointLights[ i ] ) );
		float3 halfVector		= normalize( viewDirection + lightDirection );
		float3 specularLight	= lightIntensity * specularIntensity * specularColor * max( GGX_Specular( specularPower, normal, halfVector, viewDirection, lightDirection ), 0 );

		//color += ( ( diffuseColor * lightColor ) + specularLight ) * attenuation;
	}

	// spot
	for (int i = 0; i < TIKI_SPOT_LIGHT_COUNT; i++)
	{
		float3 lightPosition	= getSpotLightPosition( c_pixelData.spotLights[ i ] );
		float3 lightDistance	= lightPosition - worldPosition;
		float3 lightDirection	= normalize( lightDistance );
		float lightIntensity	= max( dot( normal, lightDirection ), 0.0 );
		float3 lightColor		= lightIntensity * getSpotLightColor( c_pixelData.spotLights[ i ] );

		float angle				= acos( dot( lightDirection, normalize( getSpotLightDirection( c_pixelData.spotLights[ i ] ) ) ) );
		float lightSpot			= TIKI_SATURATE( smoothstep( getSpotLightPhi( c_pixelData.spotLights[ i ] ), getSpotLightTheta( c_pixelData.spotLights[ i ] ), angle ) );
  
		float attenuation		= 1.0 - TIKI_SATURATE( length( lightDistance ) * getSpotLightInverseRange( c_pixelData.spotLights[ i ] ) );
		float3 halfVector		= normalize( viewDirection + lightDirection );
		float3 specularLight	= lightIntensity * specularIntensity * specularColor * max( GGX_Specular( specularPower, normal, halfVector, viewDirection, lightDirection ), 0 );

		//color += ( ( diffuseColor * lightColor ) + specularLight ) * lightSpot * attenuation;
	}

	TIKI_PIXEL_OUTPUT_SET( TIKI_OUTPUT_COLOR0, float4( color, 1.0 ) );
	TIKI_PIXEL_OUTPUT_END( PixelOutput );
}

#else

#	error Shader type not supported.

#endif
