#ifndef TIKI_2D_SPRITE_SHADER_HPP_INCLUDED
#define TIKI_2D_SPRITE_SHADER_HPP_INCLUDED

#include "tiki/graphics/shader/graphicstypes.hpp"

#ifdef __cplusplus
#	include "tiki/graphics/color.hpp"
#endif

TIKI_GRAPHICS_NAMESPACE_BEGIN

struct SpriteVertexConstantData
{
	GraphicsMatrix44	projection;
};

TIKI_GRAPHICS_NAMESPACE_END

#endif