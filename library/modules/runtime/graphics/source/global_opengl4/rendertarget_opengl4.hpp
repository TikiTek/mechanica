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
			frameBufferId = 0u;

			for (uint i = 0u; i < TIKI_COUNT( aColorBufferIds ); ++i)
			{
				aColorBufferIds[ i ] = 0u;
			}

			depthBufferId = 0u;
		}

		GLuint		aColorBufferIds[ GraphicsSystemLimits_RenderTargetSlots ];
		GLuint		depthBufferId;

		GLuint		frameBufferId;
	};
}

#endif // __TIKI_RENDERTARGET_OPENGL4_HPP_INCLUDED__
