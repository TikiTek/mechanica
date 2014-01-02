#pragma once
#ifndef __TIKI_RENDERCOMMAND_HPP_INCLUDED__
#define __TIKI_RENDERCOMMAND_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	struct RenderCommand
	{
		const ModelGeometry*	pGeometry;
		//const SkinningData*		pSkinningData;
		Matrix43				worldTransform;
	};

	struct RenderSequence
	{
		uint8					renderEffectId;
		uint8					commandCount;

		uint16					sortValue;

		const RenderCommand*	pCommands;
	};
}

#endif // __TIKI_RENDERCOMMAND_HPP_INCLUDED__
