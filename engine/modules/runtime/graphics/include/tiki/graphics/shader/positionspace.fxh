#ifndef __TIKI_POSITIONSPACE_FXH_INCLUDED__
#define __TIKI_POSITIONSPACE_FXH_INCLUDED__

#include "shader/platform.fxh"
#include "shader/cameraparameter.hpp"

float reconstructLinearDepth( TIKI_IN float depth, TIKI_IN CameraParameter camera )
{
	return camera.param0.x / ( depth * camera.param0.y - camera.param0.z );
}

float reconstructLinearDepth( TIKI_IN TIKI_TEXTURE2D tex, TIKI_IN TIKI_SAMPLER sam, TIKI_IN float2 screenSpacePosition, TIKI_IN CameraParameter camera )
{
	return reconstructLinearDepth( TIKI_TEX2D( tex, sam, screenSpacePosition ).x, camera );
}

float2 reconstructClipSpacePosition( TIKI_IN float2 screenSpacePosition )
{
	return screenSpacePosition * float2( 2.0, -2.0 ) - float2( 1.0, -1.0 );
}

float4 reconstructProjectionSpacePosition( TIKI_IN float2 screenSpacePosition, TIKI_IN float depth )
{
	return float4( screenSpacePosition * float2( 2.0, -2.0 ) - float2( 1.0, -1.0 ), depth, 1.0 );
}

float3 reconstructViewSpacePosition( TIKI_IN float2 clipSpacePosition, TIKI_IN float linearDepth, TIKI_IN CameraParameter camera )
{
	float2 viewPosition = ( ( clipSpacePosition * linearDepth ) * camera.param1.xz / camera.param0.w - linearDepth * camera.param1.yw / camera.param0.w );
	return float3( viewPosition, linearDepth );
}

float3 reconstructWorldSpacePosition( TIKI_IN float4 projectedPosition, TIKI_IN CameraParameter camera )
{
	float4 worldPosition = TIKI_MUL( projectedPosition, camera.inverseViewProjection );
	return worldPosition.xyz / worldPosition.w;  
}

#endif // __TIKI_POSITIONSPACE_FXH_INCLUDED__
