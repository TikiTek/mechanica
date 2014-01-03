#pragma once
#ifndef __TIKI_RENDERERCONTEXT_HPP_INCLUDED__
#define __TIKI_RENDERERCONTEXT_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/renderer/rendererdefinition.hpp"

namespace tiki
{
	class GraphicsSystem;

	struct RendererContext
	{
		GraphicsSystem*	pGraphicsSystem;

		uint			rendererWidth;
		uint			rendererHeight;
	};

	struct FrameData
	{
		Camera		mainCamera;
	};
}

#endif // __TIKI_RENDERERCONTEXT_HPP_INCLUDED__
