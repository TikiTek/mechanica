#pragma once
#ifndef __TIKI_RENDERERCONTEXT_HPP_INCLUDED__
#define __TIKI_RENDERERCONTEXT_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/container/fixedsizedarray.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/vector.hpp"

#include "renderer.hpp"

namespace tiki
{
	class GraphicsSystem;
	class RenderTarget;
	class TextureData;
	
	struct RendererContext
	{
		GraphicsSystem*	pGraphicsSystem;

		uint			rendererWidth;
		uint			rendererHeight;

		const TextureData*	pGBuffer0;
		const TextureData*	pGBuffer1;
		const TextureData*	pGBuffer2;
		const TextureData*	pAccumulationBuffer;
		const TextureData*	pDepthBuffer;
	};
}

#endif // __TIKI_RENDERERCONTEXT_HPP_INCLUDED__
