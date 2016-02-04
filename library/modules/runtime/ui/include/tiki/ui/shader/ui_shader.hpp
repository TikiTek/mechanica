#ifndef TIKI_UI_SHADER_HPP_INCLUDED
#define TIKI_UI_SHADER_HPP_INCLUDED

#include "tiki/graphics/graphicstypes.hpp"

TIKI_GRAPHICS_NAMESPACE_BEGIN

#define TIKI_UI_MODE_COLOR		0
#define TIKI_UI_MODE_TEXTURE	1
#define TIKI_UI_MODE_FONT		2

struct UiVertexConstantData
{
	GraphicsMatrix44 projection;
};

TIKI_GRAPHICS_NAMESPACE_END

#endif // TIKI_UI_SHADER_HPP_INCLUDED
