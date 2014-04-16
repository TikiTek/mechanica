#pragma once
#ifndef __TIKI_SCENERENDEREFFECTDATA_HPP_INCLUDED__
#define __TIKI_SCENERENDEREFFECTDATA_HPP_INCLUDED__

#include "tiki/renderer/rendereffectdata.hpp"

namespace tiki
{
	TIKI_REFLECTION_INHERITANCE_STRUCT(
		SceneRenderEffectData,
		RenderEffectData,
		SceneRenderEffectData()
		{
			renderEffectId			= RenderEffectId_Fallback;
			renderPassMask			= 0u;
			renderFlags				= 0u;
			defaultTextureOffset	= (uint16)TIKI_OFFSETOF( SceneRenderEffectData, diffuseMap );
		}
		TextureReference	diffuseMap;
		TextureReference	normalMap;
		TextureReference	selfilluMap;
	);
}

#endif // __TIKI_SCENERENDEREFFECTDATA_HPP_INCLUDED__
