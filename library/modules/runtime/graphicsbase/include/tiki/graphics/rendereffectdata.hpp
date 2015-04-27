#pragma once
#ifndef __TIKI_RENDEREFFECTDATA_HPP_INCLUDED__
#define __TIKI_RENDEREFFECTDATA_HPP_INCLUDED__

#include "tiki/base/reflection.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/graphics_references.hpp"

namespace tiki
{
	enum
	{
		RenderEffectDataInvalidTextureOffset = 0xffffu
	};

	TIKI_REFLECTION_STRUCT(
		RenderEffectData,
		uint16	renderEffectId;
		uint16	renderPassMask;
		uint16	renderFlags;
		uint16	defaultTextureOffset;
	);
}

#endif // __TIKI_RENDEREFFECTDATA_HPP_INCLUDED__
