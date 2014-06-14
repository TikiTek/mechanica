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

GeometryBufferSample createGeometryBufferSample(
	TIKI_IN float3	diffuseColor,
	TIKI_IN float3	selfIlluminationColor,
	TIKI_IN float	selfIlluminationFactor,
	TIKI_IN float2	normal,
	TIKI_IN float	specluarBrightness,
	TIKI_IN float	specluarIntensity,
	TIKI_IN float	specluarPower )
{
	GeometryBufferSample gSample;

	gSample.buffer0Sample.xyz	= diffuseColor;
	gSample.buffer0Sample.a		= specluarBrightness;
	gSample.buffer1Sample.xyz	= selfIlluminationColor;
	gSample.buffer1Sample.a		= selfIlluminationFactor / 32.0;
	gSample.buffer2Sample.xy	= normal;
	gSample.buffer2Sample.z		= specluarIntensity;
	gSample.buffer2Sample.w		= specluarPower;

	return gSample;
}

float3	getDiffuseColor( TIKI_IN GeometryBufferSample gSample )				{ return gSample.buffer0Sample.xyz; }
float3	getSelfIlluminationColor( TIKI_IN GeometryBufferSample gSample )	{ return gSample.buffer1Sample.xyz; }
float	getSelfIlluminationFactor( TIKI_IN GeometryBufferSample gSample )	{ return gSample.buffer1Sample.a * 32.0; }
float2	getCompressedNormal( TIKI_IN GeometryBufferSample gSample )			{ return gSample.buffer2Sample.xy; }
float3	getNormal( TIKI_IN GeometryBufferSample gSample )					{ return decodeNormal( getCompressedNormal( gSample ) ); }
float	getSpecluarBrightness( TIKI_IN GeometryBufferSample gSample )		{ return gSample.buffer0Sample.a; }
float	getSpecluarIntensity( TIKI_IN GeometryBufferSample gSample )		{ return gSample.buffer2Sample.z; }
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

float3 sampleSelfIlluminationColor( TIKI_TEXTURE2D gBuffer1, TIKI_SAMPLER sam, float2 texCoord )
{
	return TIKI_TEX2D( gBuffer1, sam, texCoord ).xyz;
}

float3 sampleNormal( TIKI_TEXTURE2D gBuffer2, TIKI_SAMPLER sam, float2 texCoord )
{
	return decodeNormal( TIKI_TEX2D( gBuffer2, sam, texCoord ).xy );
}

#endif // __TIKI_GBUFFER_HPP_INCLUDED__
