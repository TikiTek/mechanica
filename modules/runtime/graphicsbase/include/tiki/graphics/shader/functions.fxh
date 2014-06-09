#ifndef __TIKI_FUNCTIONS_FXH_INCLUDED__
#define __TIKI_FUNCTIONS_FXH_INCLUDED__

#include "tiki/graphics/graphicstypes.hpp"

float2 encodeNormal( float3 normal )
{
	float p = sqrt( normal.z * 8.0 + 8.0 );
    return normal.xy / p + 0.5;
}

float3 decodeNormal( float2 compressedNormal )
{
	half2 fenc	= compressedNormal * 4.0 - 2.0;
    half f		= dot( fenc, fenc );
    half g		= sqrt( 1.0 - f / 4.0 );

    half3 normal;
    normal.xy	= fenc * g;
    normal.z	= 1.0 - f / 2.0;

    return normal;
}

#endif // __TIKI_FUNCTIONS_FXH_INCLUDED__