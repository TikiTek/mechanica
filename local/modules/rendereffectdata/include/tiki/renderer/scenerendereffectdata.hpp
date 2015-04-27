#pragma once
#ifndef __TIKI_SCENERENDEREFFECTDATA_HPP_INCLUDED__
#define __TIKI_SCENERENDEREFFECTDATA_HPP_INCLUDED__

#include "tiki/graphics/rendereffectdata.hpp"
#include "tiki/renderer/rendererdefinition.hpp"

namespace tiki
{
	TIKI_REFLECTION_INHERITANCE_STRUCT(
		SceneRenderEffectData,
		RenderEffectData,
		SceneRenderEffectData()
		{
			renderEffectId			= RenderEffectId_Scene;
			renderPassMask			= 0u;
			renderFlags				= 0u;
			defaultTextureOffset	= (uint16)TIKI_OFFSETOF( SceneRenderEffectData, diffuseMap );
			selfIlluminationFactor	= 1.0f;
			specluarBrightness		= 0.0f;
			specluarIntensity		= 0.0f;
			specluarPower			= 0.0f;
		}
		TextureReference	diffuseMap;
		TextureReference	normalMap;
		TextureReference	selfilluMap;
		float				selfIlluminationFactor;
		float				specluarBrightness;
		float				specluarIntensity;
		float				specluarPower;
	);
}

#endif // __TIKI_SCENERENDEREFFECTDATA_HPP_INCLUDED__
