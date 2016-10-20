#ifndef __TIKI_BLOOM_SHADER_HPP_INCLUDED__
#define __TIKI_BLOOM_SHADER_HPP_INCLUDED__

#include "tiki/graphics/graphicstypes.hpp"

TIKI_GRAPHICS_NAMESPACE_BEGIN

struct BloomBlurPixelConstantData
{
	float4 param0;	// xy: pixel offset
};

#ifndef __cplusplus

float2 getBlurPixelOffset( BloomBlurPixelConstantData data )
{
	return data.param0.xy;
}

#endif

TIKI_GRAPHICS_NAMESPACE_END

#endif // __TIKI_BLOOM_SHADER_HPP_INCLUDED__
