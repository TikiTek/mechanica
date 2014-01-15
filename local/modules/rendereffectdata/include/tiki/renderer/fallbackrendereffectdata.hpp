#pragma once
#ifndef __TIKI_FALLBACKRENDEREFFECTDATA_HPP_INCLUDED__
#define __TIKI_FALLBACKRENDEREFFECTDATA_HPP_INCLUDED__

#include "tiki/renderer/rendereffectdata.hpp"

#include "tiki/resource/resourcefile.hpp"

namespace tiki
{
	class Texture;

	TIKI_REFLECTION_INHERITANCE_STRUCT(
		FallbackRenderEffectData,
		RenderEffectData,
		FallbackRenderEffectData()
		{
			renderEffectId			= RenderEffectId_Fallback;
			renderPassMask			= 0u;
			renderFlags				= 0u;
			defaultTextureOffset	= (uint16)TIKI_OFFSETOF( FallbackRenderEffectData, pDiffuseTexture );
		}
		ResRef< Texture >	pDiffuseTexture;
		//TIKI_REFLECTION_FIELD( const Texture*, pDiffuseTexture )
	);
}

#endif // __TIKI_FALLBACKRENDEREFFECTDATA_HPP_INCLUDED__
