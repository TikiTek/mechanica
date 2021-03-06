#ifndef TIKI_IMMEDIATERENDERER_SHADER_HPP__INCLUDED
#define TIKI_IMMEDIATERENDERER_SHADER_HPP__INCLUDED

#include "tiki/graphics/shader/graphicstypes.hpp"

TIKI_GRAPHICS_NAMESPACE_BEGIN

struct ImmediateRendererConstantData
{
	GraphicsMatrix44 projection;
};

TIKI_GRAPHICS_NAMESPACE_END

#endif