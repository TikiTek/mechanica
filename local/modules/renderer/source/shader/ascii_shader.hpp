#pragma once
#ifndef __TIKI_ASCII_SHADER_HPP_INCLUDED__
#define __TIKI_ASCII_SHADER_HPP_INCLUDED__

#include "tiki/graphics/graphicstypes.hpp"

TIKI_GRAPHICS_NAMESPACE_BEGIN

struct AsciiPixelConstantData
{
	GraphicsMatrix44	inverseProjection;
	float4				param0;				// x: far
};

float getFarPlane( AsciiPixelConstantData data )
{
	return data.param0.x;
}

TIKI_GRAPHICS_NAMESPACE_END


#endif // __TIKI_ASCII_SHADER_HPP_INCLUDED__
