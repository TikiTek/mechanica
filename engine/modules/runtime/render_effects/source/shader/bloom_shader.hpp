#ifndef __TIKI_BLOOM_SHADER_HPP_INCLUDED__
#define __TIKI_BLOOM_SHADER_HPP_INCLUDED__

#include "tiki/graphics/shader/graphicstypes.hpp"

TIKI_GRAPHICS_NAMESPACE_BEGIN

struct BloomCutoffPixelConstantData
{
	float4 param0;	// xyz: cutoff threshold per channel, w: unused
};

#ifndef __cplusplus

float3 getCutoffThresold( BloomCutoffPixelConstantData data )
{
	return data.param0.xyz;
}

#endif

TIKI_GRAPHICS_NAMESPACE_END

#endif