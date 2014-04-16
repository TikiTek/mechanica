#pragma once
#ifndef __TIKI_GBUFFER_HPP_INCLUDED__
#define __TIKI_GBUFFER_HPP_INCLUDED__

struct GeometryBufferSample
{
	float4	buffer1Sample;
	float4	buffer2Sample;
	float4	buffer3Sample;
};

GeometryBufferSample createGeometryBufferSample( float3 diffuseColor, float3 selfIlluminationColor, float selfIlluminationFactor, float2 normal, float specluarBrightness, float specluarIntensity, float specluarPower )
{
	GeometryBufferSample sample;
	sample.buffer1Sample.rgb	= diffuseColor;
	sample.buffer1Sample.a		= specluarBrightness;
	sample.buffer2Sample.rgb	= selfIlluminationColor;
	sample.buffer2Sample.a		= specluarIntensity;
	sample.buffer3Sample.xy		= normal;
	sample.buffer3Sample.z		= selfIlluminationFactor;
	sample.buffer3Sample.w		= specluarPower;
	return sample;
}

float3 getDiffuseColor( GeometryBufferSample sample )
{
	return sample.buffer1Sample.rgb;
}

float3 getSelfIlluminationColor( GeometryBufferSample sample )
{
	return sample.buffer2Sample.rgb;
}

float getSelfIlluminationFactor( GeometryBufferSample sample )
{
	return sample.buffer3Sample.z;
}

float2 getNormal( GeometryBufferSample sample )
{
	return sample.buffer3Sample.xy;
}

float getSpecluarBrightness( GeometryBufferSample sample )
{
	return sample.buffer1Sample.a;
}

float getSpecluarIntensity( GeometryBufferSample sample )
{
	return sample.buffer2Sample.a;
}

float getSpecluarPower( GeometryBufferSample sample )
{
	return sample.buffer3Sample.a;
}

#endif // __TIKI_GBUFFER_HPP_INCLUDED__
