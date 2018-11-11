#ifndef TIKI_SKYBOX_SHADER_HPP_INCLUDED__
#define TIKI_SKYBOX_SHADER_HPP_INCLUDED__

#include "tiki/graphics/graphicstypes.hpp"

TIKI_GRAPHICS_NAMESPACE_BEGIN

struct SkyboxVertexConstantData
{
	GraphicsMatrix44	mvpMatrix;
};

struct SkyboxPixelConstantData
{
	GraphicsMatrix44	inverseVpMatrix;
};

TIKI_GRAPHICS_NAMESPACE_END

#endif // TIKI_SKYBOX_SHADER_HPP_INCLUDED__
