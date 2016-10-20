#ifndef __TIKI_ASCII_SHADER_HPP_INCLUDED__
#define __TIKI_ASCII_SHADER_HPP_INCLUDED__

#include "tiki/graphics/graphicstypes.hpp"

TIKI_GRAPHICS_NAMESPACE_BEGIN

struct AsciiPixelConstantData
{
	GraphicsMatrix44	inverseProjection;
	float4				param0;				// x: far, y: screenWidth, z: screenHeight
};

#ifndef __cplusplus

float getFarPlane( TIKI_IN AsciiPixelConstantData data )
{
	return data.param0.x;
}

float2 getScreenSize( TIKI_IN AsciiPixelConstantData data )
{
	return data.param0.yz;
}

#endif

TIKI_GRAPHICS_NAMESPACE_END


#endif // __TIKI_ASCII_SHADER_HPP_INCLUDED__
