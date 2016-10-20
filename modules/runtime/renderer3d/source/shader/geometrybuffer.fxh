#ifndef __TIKI_GBUFFER_HPP_INCLUDED__
#define __TIKI_GBUFFER_HPP_INCLUDED__

#include "shader/functions.fxh"
#include "shader/platform.fxh"

struct GeometryBufferSample
{
	float4	buffer0Sample;
	float4	buffer1Sample;
	float4	buffer2Sample;
};

// see gamerenderer.hpp -> GeometryBuffer
#define TIKI_GEOMETRYBUFFER_PIXEL_OUTPUT										\
	TIKI_PIXEL_OUTPUT_DEFINITION_BEGIN( GeometryBufferPixelOutput )				\
		TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( 0, float4, TIKI_OUTPUT_COLOR0 )	\
		TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( 1, float4, TIKI_OUTPUT_COLOR1 )	\
		TIKI_PIXEL_OUTPUT_DEFINITION_ELEMENT( 2, float4, TIKI_OUTPUT_COLOR2 )	\
	TIKI_PIXEL_OUTPUT_DEFINITION_END( GeometryBufferPixelOutput )

float4 createGeometryBuffer0Color( TIKI_IN float3 diffuseColor, TIKI_IN float selfIlluminationFactor )
{
	return float4( diffuseColor, selfIlluminationFactor / 32.0 );
}

float4 createGeometryBuffer1Color( TIKI_IN float3 specluarColor, TIKI_IN float specluarIntensity )
{
	return float4( specluarColor, specluarIntensity );
}

float4 createGeometryBuffer2Color( TIKI_IN float3 normal, TIKI_IN float specluarPower )
{
	return float4( normal, specluarPower );
}

GeometryBufferSample createGeometryBufferSample(
	TIKI_IN float3	diffuseColor,
	TIKI_IN float	selfIlluminationFactor,
	TIKI_IN float3	specluarColor,
	TIKI_IN float	specluarIntensity,
	TIKI_IN float3	normal,
	TIKI_IN float	specluarPower )
{
	GeometryBufferSample gSample;

	gSample.buffer0Sample.xyz	= diffuseColor;
	gSample.buffer0Sample.w		= selfIlluminationFactor / 32.0;
	gSample.buffer1Sample.xyz	= specluarColor;
	gSample.buffer1Sample.w		= specluarIntensity;
	gSample.buffer2Sample.xyz	= normal;
	gSample.buffer2Sample.w		= specluarPower;

	return gSample;
}

float3	getDiffuseColor( TIKI_IN GeometryBufferSample gSample )				{ return gSample.buffer0Sample.xyz; }
float	getSelfIlluminationFactor( TIKI_IN GeometryBufferSample gSample )	{ return gSample.buffer0Sample.a * 32.0; }
float3	getSpecluarColor( TIKI_IN GeometryBufferSample gSample )			{ return gSample.buffer1Sample.xyz; }
float	getSpecluarIntensity( TIKI_IN GeometryBufferSample gSample )		{ return gSample.buffer1Sample.w; }
float3	getNormal( TIKI_IN GeometryBufferSample gSample )					{ return gSample.buffer2Sample.xyz; }
float	getSpecluarPower( TIKI_IN GeometryBufferSample gSample )			{ return gSample.buffer2Sample.a; }

GeometryBufferSample sampleGeometryBuffer( TIKI_TEXTURE2D gBuffer0, TIKI_TEXTURE2D gBuffer1, TIKI_TEXTURE2D gBuffer2, TIKI_SAMPLER sam, float2 texCoord )
{
	GeometryBufferSample gSample;

	gSample.buffer0Sample = TIKI_TEX2D( gBuffer0, sam, texCoord );
	gSample.buffer1Sample = TIKI_TEX2D( gBuffer1, sam, texCoord );
	gSample.buffer2Sample = TIKI_TEX2D( gBuffer2, sam, texCoord );

	return gSample;
}

float3 sampleDiffuseColor( TIKI_TEXTURE2D gBuffer0, TIKI_SAMPLER sam, float2 texCoord )
{
	return TIKI_TEX2D( gBuffer0, sam, texCoord ).xyz;
}

float3 sampleSpecluarColor( TIKI_TEXTURE2D gBuffer1, TIKI_SAMPLER sam, float2 texCoord )
{
	return TIKI_TEX2D( gBuffer1, sam, texCoord ).xyz;
}

float3 sampleNormal( TIKI_TEXTURE2D gBuffer2, TIKI_SAMPLER sam, float2 texCoord )
{
	return TIKI_TEX2D( gBuffer2, sam, texCoord ).xyz;
}

#endif // __TIKI_GBUFFER_HPP_INCLUDED__
