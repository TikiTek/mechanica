#ifndef __TIKI_POSITIONSPACE_FXH_INCLUDED__
#define __TIKI_POSITIONSPACE_FXH_INCLUDED__

#include "shader/platform.fxh"
#include "shader/cameraparameter.hpp"

float reconstructLinearDepth( float depth, CameraParameter camera )
{
	return camera.param0.x / ( depth * camera.param0.y - camera.param0.z );
}

float reconstructLinearDepth( TIKI_TEXTURE2D tex, TIKI_SAMPLER sam, float2 screenSpacePosition, CameraParameter camera )
{
	return reconstructLinearDepth( TIKI_TEX2D( tex, sam, screenSpacePosition ).x, camera );
}

float2 reconstructClipSpacePosition( float2 screenSpacePosition )
{
	return screenSpacePosition * float2( 2.0f, -2.0f ) - float2( 1.0f, -1.0f );
}

float4 reconstructProjectionSpacePosition( float2 screenSpacePosition, float depth )
{
	return float4( screenSpacePosition * float2( 2.0f, -2.0f ) - float2( 1.0f, -1.0f ), depth, 1.0f );
}

float3 reconstructViewSpacePosition( float2 clipSpacePosition, float linearDepth, CameraParameter camera )
{
	float2 viewPosition = ( ( clipSpacePosition * linearDepth ) * camera.param1.xz / camera.param0.w - linearDepth * camera.param1.yw / camera.param0.w );
	return float3( viewPosition, linearDepth );
}

float3 reconstructWorldSpacePosition( float4 projectedPosition, CameraParameter camera )
{
	float4 worldPosition = mul( projectedPosition, camera.inverseViewProjection );
	return worldPosition.xyz / worldPosition.w;  
}

#endif // __TIKI_POSITIONSPACE_FXH_INCLUDED__
