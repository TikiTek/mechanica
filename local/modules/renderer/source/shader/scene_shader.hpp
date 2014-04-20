#ifndef __TIKI_SCENE_SHADER_HPP_INCLUDED__
#define __TIKI_SCENE_SHADER_HPP_INCLUDED__

#include "tiki/graphics/graphicstypes.hpp"

TIKI_GRAPHICS_NAMESPACE_BEGIN

struct SceneVertexConstantData
{
	GraphicsMatrix44	mvpMatrix;
	GraphicsMatrix33	modelMatrix;
};

struct ScenePixelConstantData
{
	float selfIlluminationFactor;
	float specluarBrightness;
	float specluarIntensity;
	float specluarPower;
};


TIKI_GRAPHICS_NAMESPACE_END

#endif // __TIKI_SCENE_SHADER_HPP_INCLUDED__
