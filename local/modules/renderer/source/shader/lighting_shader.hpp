#ifndef __TIKI_LIGHTING_SHADER_HPP_INCLUDED__
#define __TIKI_LIGHTING_SHADER_HPP_INCLUDED__

#include "tiki/graphics/graphicstypes.hpp"

TIKI_GRAPHICS_NAMESPACE_BEGIN

struct LightingPixelConstantData
{
	GraphicsMatrix44	inverseProjection;
	float4				param0;				// xyz: direction
	float4				param1;				// xyz: color
	float4				param2;				// xyz: camera position
};

#ifndef __cplusplus

float3 getLightDirection( LightingPixelConstantData data )
{
	return data.param0.xyz;
}

float3 getLightColor( LightingPixelConstantData data )
{
	return data.param1.xyz;
}

float3 getCameraColor( LightingPixelConstantData data )
{
	return data.param1.xyz;
}

#endif

TIKI_GRAPHICS_NAMESPACE_END

#endif // __TIKI_LIGHTING_SHADER_HPP_INCLUDED__
