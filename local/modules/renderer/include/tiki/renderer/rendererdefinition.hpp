#pragma once
#ifndef __TIKI_RENDERERDEFINITION_HPP_INCLUDED__
#define __TIKI_RENDERERDEFINITION_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	enum RenderEffectId
	{
		RenderEffectId_Invalid = -1,

		RenderEffectId_Fallback,
		RenderEffectId_Scene,

		RenderEffectId_Count
	};

	enum RenderPass
	{
		RenderPass_Invalid = -1,

		RenderPass_Geometry,
		RenderPass_ShadowVolume,

		RenderPass_Count
	};

	enum RenderPassMask
	{
		RenderPassMask_Invalid		= 0,

		RenderPassMask_Geometry		= 1u << RenderPass_Geometry,
		RenderPassMask_ShadowVolume	= 1u << RenderPass_ShadowVolume
	};
}

#endif // __TIKI_RENDERERDEFINITION_HPP_INCLUDED__
