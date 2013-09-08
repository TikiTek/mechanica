#ifndef TIKI_IMMEDIATERENDERER_SHADER_HPP__INCLUDED
#define TIKI_IMMEDIATERENDERER_SHADER_HPP__INCLUDED

#include "tiki/graphicsbase/graphicstypes.hpp"

#ifdef __cplusplus
namespace tiki
{
#endif // __cplusplus

	struct ImmediateRendererConstantData
	{
		GraphicsMatrix44	projection;
	};

#ifdef __cplusplus
}
#endif

#endif // TIKI_IMMEDIATERENDERER_SHADER_HPP__INCLUDED
