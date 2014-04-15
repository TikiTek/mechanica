#ifndef __TIKI_FUNCTIONS_FXH_INCLUDED__
#define __TIKI_FUNCTIONS_FXH_INCLUDED__

#include "tiki/graphics/graphicstypes.hpp"

float4 getProjectedPosition( float2 screenSpacePosition, float depth )
{
    float2 clipSpacePosition = screenSpacePosition * float2( 2.0f, -2.0f ) - float2( 1.0f, -1.0f );
    return float4( clipSpacePosition, depth, 1.0f );
}

float3 reconstructViewSpacePosition( float4 projectedPosition, GraphicsMatrix44 inverseProjection )
{
    float4 viewPosition = mul( projectedPosition, inverseProjection );
    return viewPosition.xyz / viewPosition.w;  
}

#endif // __TIKI_FUNCTIONS_FXH_INCLUDED__