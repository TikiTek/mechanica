#ifndef __TIKI_LIGHTING_SHADER_HPP_INCLUDED__
#define __TIKI_LIGHTING_SHADER_HPP_INCLUDED__

#include "tiki/graphics/graphicstypes.hpp"

#ifdef __cplusplus
#	include "tiki/graphics/color.hpp"
#endif

TIKI_GRAPHICS_NAMESPACE_BEGIN

#define TIKI_MAX_DIRECTIONAL_LIGHT_COUNT	4
#define TIKI_MAX_POINT_LIGHT_COUNT			16
#define TIKI_MAX_SPOT_LIGHT_COUNT			4

struct DirectionalLightConstantData
{
	float4	param0;		// xyz: direction
	float4	param1;		// xyz: color
};

struct PointLightConstantData
{
	float4	param0;		// xyz: view space position
	float4	param1;		// xyz: color, w: inverse range
};

struct SpotLightConstantData
{
	float4	param0;		// xyz: view space position, w: theta
	float4	param1;		// xyz: direction, w: phi
	float4	param2;		// xyz: color, w: inverse range
};

struct LightingPixelConstantData
{
	GraphicsMatrix44				inverseProjection;
	float4							param0;

	DirectionalLightConstantData	directionalLights[ TIKI_MAX_DIRECTIONAL_LIGHT_COUNT ];
	PointLightConstantData			pointLights[ TIKI_MAX_POINT_LIGHT_COUNT ];
	SpotLightConstantData			spotLights[ TIKI_MAX_SPOT_LIGHT_COUNT ];
};

#ifdef __cplusplus

DirectionalLightConstantData fillDirectionalLightData( const Vector3& direction, Color color )
{
	DirectionalLightConstantData data;
	createFloat4( data.param0, direction.x, direction.y, direction.z, 0.0f );
	color::toFloat4( data.param1, color );

	return data;
}

PointLightConstantData fillPointLightData( const Vector3& worldPosition, const Matrix44& viewMatrix, const Vector3& cameraPosition, Color color, float range )
{
	PointLightConstantData data;
	
	Vector3 viewPosition = worldPosition;
	//vector::sub( viewPosition, cameraPosition );
	//matrix::transform( viewPosition, viewMatrix );
	createFloat4( data.param0, viewPosition.x, viewPosition.y, viewPosition.z, 0.0f );

	color::toFloat4( data.param1, color );
	data.param1.w = 1.0f / range;

	return data;
}

SpotLightConstantData fillSpotLightData( const Vector3& worldPosition, const Matrix44& viewMatrix, const Vector3& direction, Color color, float range, float theta, float phi )
{
	SpotLightConstantData data;

	Vector3 viewPosition = worldPosition;
	//vector::negate( viewPosition );
	//vector::sub( viewPosition, worldPosition );
	//matrix::transform( viewPosition, viewMatrix );
	
	createFloat4( data.param0, viewPosition.x, viewPosition.y, viewPosition.z, theta );
	createFloat4( data.param1, direction.x, direction.y, direction.z, phi );
	color::toFloat4( data.param2, color );
	data.param2.w = 1.0f / range;
	
	return data;
}

#else

float3 getCameraWorldPosition( LightingPixelConstantData data )
{
	return data.param0.xyz;
}

float3 getDirectionalLightDirection( DirectionalLightConstantData data )
{
	return data.param0.xyz;
}

float3 getDirectionalLightColor( DirectionalLightConstantData data )
{
	return data.param1.xyz;
}

float3 getPointLightPosition( PointLightConstantData data )
{
	return data.param0.xyz;
}

float3 getPointLightColor( PointLightConstantData data )
{
	return data.param1.xyz;
}

float getPointLightInverseRange( PointLightConstantData data )
{
	return data.param1.w;
}

float3 getSpotLightPosition( SpotLightConstantData data )
{
	return data.param0.xyz;
}

float3 getSpotLightDirection( SpotLightConstantData data )
{
	return data.param1.xyz;
}

float3 getSpotLightColor( SpotLightConstantData data )
{
	return data.param1.xyz;
}

float getSpotLightTheta( SpotLightConstantData data )
{
	return data.param0.w;
}

float getSpotLightPhi( SpotLightConstantData data )
{
	return data.param1.w;
}

float getSpotLightInverseRange( SpotLightConstantData data )
{
	return data.param2.w;
}

#endif

TIKI_GRAPHICS_NAMESPACE_END

#endif // __TIKI_LIGHTING_SHADER_HPP_INCLUDED__
