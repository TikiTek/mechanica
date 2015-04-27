#pragma once
#ifndef __TIKI_FALLBACKRENDEREFFECTDATA_HPP_INCLUDED__
#define __TIKI_FALLBACKRENDEREFFECTDATA_HPP_INCLUDED__

#include "tiki/graphics/rendereffectdata.hpp"
#include "tiki/renderer/rendererdefinition.hpp"

namespace tiki
{
	TIKI_REFLECTION_INHERITANCE_STRUCT(
		FallbackRenderEffectData,
		RenderEffectData,
		FallbackRenderEffectData()
		{
			renderEffectId			= RenderEffectId_Fallback;
			renderPassMask			= 0u;
			renderFlags				= 0u;
			defaultTextureOffset	= (uint16)TIKI_OFFSETOF( FallbackRenderEffectData, defaultTexture );
		}
		TextureReference	defaultTexture;
	);
}

#endif // __TIKI_FALLBACKRENDEREFFECTDATA_HPP_INCLUDED__
