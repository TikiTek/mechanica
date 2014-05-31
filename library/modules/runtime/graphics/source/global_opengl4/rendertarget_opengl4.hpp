#pragma once
#ifndef __TIKI_RENDERTARGET_OPENGL4_HPP_INCLUDED__
#define __TIKI_RENDERTARGET_OPENGL4_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/graphics/graphissystemlimits.hpp"

#include "../../../source/global_opengl4/graphicstypes_opengl4.hpp"

namespace tiki
{
	struct RenderTargetPlatformData
	{
		RenderTargetPlatformData()
		{
			//for (uint i = 0u; i < TIKI_COUNT( pColorViews ); ++i)
			//{
			//	pColorViews[ i ] = nullptr;
			//}

			//pDepthView = nullptr;
		}

		//TGRenderTargetView*		pColorViews[ GraphicsSystemLimits_RenderTargetSlots ];
		//TGDepthStencilView*		pDepthView;
	};
}

#endif // __TIKI_RENDERTARGET_OPENGL4_HPP_INCLUDED__
