#pragma once
#ifndef __TIKI_GRAPHICSCONTEXT_OPENGL4_HPP_INCLUDED__
#define __TIKI_GRAPHICSCONTEXT_OPENGL4_HPP_INCLUDED__

#include "../../../source/global_opengl4/graphicstypes_opengl4.hpp"

#include "graphicssystem_internal_opengl4.hpp"

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
#	define TIKI_CHECK_GRAPHICS graphics::checkError()
#else
#	define TIKI_CHECK_GRAPHICS
#endif

namespace tiki
{
	struct GraphicsContextPlatformData
	{
		GraphicsContextPlatformData()
		{
			primitiveTopology	= GL_INVALID_ENUM;

			indexType			= GL_INVALID_ENUM;
		}

		GLenum		primitiveTopology;

		GLuint		vertexShaderId;
		GLuint		pixelShaderId;

		GLenum		indexType;
	};
}

#endif // __TIKI_GRAPHICSCONTEXT_OPENGL4_HPP_INCLUDED__
