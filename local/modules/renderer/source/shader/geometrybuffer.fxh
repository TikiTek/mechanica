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

GeometryBufferSample createGeometryBufferSample( float3 diffuseColor, float3 selfIlluminationColor, float selfIlluminationFactor, float2 normal, float specluarBrightness, float specluarIntensity, float specluarPower )
{
	GeometryBufferSample sample;
	sample.buffer0Sample.rgb	= diffuseColor;
	sample.buffer0Sample.a		= specluarBrightness;
	sample.buffer1Sample.rgb	= selfIlluminationColor;
	sample.buffer1Sample.a		= selfIlluminationFactor / 32.0f;
	sample.buffer2Sample.xy		= normal;
	sample.buffer2Sample.z		= specluarIntensity;
	sample.buffer2Sample.w		= specluarPower;
	return sample;
}

float3 getDiffuseColor( GeometryBufferSample sample )
{
	return sample.buffer0Sample.rgb;
}

float3 getSelfIlluminationColor( GeometryBufferSample sample )
{
	return sample.buffer1Sample.rgb;
}

float getSelfIlluminationFactor( GeometryBufferSample sample )
{
	return sample.buffer1Sample.a * 32.0f;
}

float2 getCompressedNormal( GeometryBufferSample sample )
{
	return sample.buffer2Sample.xy;
}

float3 getNormal( GeometryBufferSample sample )
{
	return decodeNormal( getCompressedNormal( sample ) );
}

float getSpecluarBrightness( GeometryBufferSample sample )
{
	return sample.buffer0Sample.a;
}

float getSpecluarIntensity( GeometryBufferSample sample )
{
	return sample.buffer2Sample.z;
}

float getSpecluarPower( GeometryBufferSample sample )
{
	return sample.buffer2Sample.a;
}

GeometryBufferSample sampleGeometryBuffer( TIKI_TEXTURE2D gBuffer0, TIKI_TEXTURE2D gBuffer1, TIKI_TEXTURE2D gBuffer2, TIKI_SAMPLER sam, float2 texCoord )
{
	GeometryBufferSample sample;
	sample.buffer0Sample = TIKI_TEX2D( gBuffer0, sam, texCoord );
	sample.buffer1Sample = TIKI_TEX2D( gBuffer1, sam, texCoord );
	sample.buffer2Sample = TIKI_TEX2D( gBuffer2, sam, texCoord );

	return sample;
}

float3 sampleDiffuseColor( TIKI_TEXTURE2D gBuffer0, TIKI_SAMPLER sam, float2 texCoord )
{
	return TIKI_TEX2D( gBuffer0, sam, texCoord ).rgb;
}

float3 sampleSelfIlluminationColor( TIKI_TEXTURE2D gBuffer1, TIKI_SAMPLER sam, float2 texCoord )
{
	return TIKI_TEX2D( gBuffer1, sam, texCoord ).rgb;
}

float3 sampleNormal( TIKI_TEXTURE2D gBuffer2, TIKI_SAMPLER sam, float2 texCoord )
{
	return decodeNormal( TIKI_TEX2D( gBuffer2, sam, texCoord ).xy );
}

#endif // __TIKI_GBUFFER_HPP_INCLUDED__
