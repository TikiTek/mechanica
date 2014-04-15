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

		const TextureData*	pGBufferDiffuse;
		const TextureData*	pGBufferSelfIllu;
		const TextureData*	pGBufferNormal;
		const TextureData*	pDepthBuffer;
	};

	struct FrameData
	{
		float		nearPlane;
		float		farPlane;
		float		aspectRatio;
		Camera		mainCamera;
	};
}

#endif // __TIKI_RENDERERCONTEXT_HPP_INCLUDED__
