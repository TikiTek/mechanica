#ifndef __TIKI_FUNCTIONS_FXH_INCLUDED__
#define __TIKI_FUNCTIONS_FXH_INCLUDED__

#include "tiki/graphics/graphicstypes.hpp"

float2 encodeNormal( float3 normal )
{
	float p = sqrt( normal.z * 8.0f + 8.0f );
    return normal.xy / p + 0.5f;
}

float3 decodeNormal( float2 compressedNormal )
{
	half2 fenc	= compressedNormal * 4.0f - 2.0f;
    half f		= dot( fenc, fenc );
    half g		= sqrt( 1.0f - f / 4.0f );

    half3 normal;
    normal.xy	= fenc * g;
    normal.z	= 1 - f / 2.0f;

    return normal;
}

#endif // __TIKI_FUNCTIONS_FXH_INCLUDED__