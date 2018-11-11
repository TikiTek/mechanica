#pragma once
#ifndef __TIKI_RENDERCOMMAND_HPP_INCLUDED__
#define __TIKI_RENDERCOMMAND_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/math/matrix.hpp"

namespace tiki
{
	class ModelGeometry;
	struct RenderEffectData;
	struct SkinningData;

	struct RenderCommand
	{
		const ModelGeometry*	pGeometry;
		const RenderEffectData*	pRenderEffectData;
		const SkinningData*		pSkinningData;
		Matrix43				worldTransform;
	};

	struct RenderSequence
	{
		uint8					renderEffectId;
		uint8					renderPassMask;
		uint8					renderFlags;
		uint8					commandCount;

		uint32					sortValue;

		const RenderCommand*	pCommands;
	};
}

#endif // __TIKI_RENDERCOMMAND_HPP_INCLUDED__
