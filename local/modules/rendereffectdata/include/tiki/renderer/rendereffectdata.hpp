#pragma once
#ifndef __TIKI_RENDEREFFECTDATA_HPP_INCLUDED__
#define __TIKI_RENDEREFFECTDATA_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/reflection.hpp"
#include "tiki/renderer/rendererdefinition.hpp"

namespace tiki
{
	//enum
	//{
	//	InvalidTextureOffset = 0xffu
	//};

	TIKI_REFLECTION_STRUCT(
		RenderEffectData,
		uint16	renderEffectId;
		uint16	renderPassMask;
		uint16	renderFlags;
		uint16	defaultTextureOffset;
	);
}

#endif // __TIKI_RENDEREFFECTDATA_HPP_INCLUDED__
