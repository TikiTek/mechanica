#pragma once
#ifndef __TIKI_GRAPHICSCONTEXT_OPENGL4_HPP_INCLUDED__
#define __TIKI_GRAPHICSCONTEXT_OPENGL4_HPP_INCLUDED__

#include "../../../source/global_opengl4/graphicstypes_opengl4.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	struct GraphicsContextPlatformData
	{
		GraphicsContextPlatformData()
		{
			maxVertexAttributeCount	= 0;

			primitiveTopology		= GL_INVALID_ENUM;

			vertexShaderId			= GL_INVALID_ENUM;
			pixelShaderId			= GL_INVALID_ENUM;

			indexType				= GL_INVALID_ENUM;
		}

		GLint		maxVertexAttributeCount;

		GLenum		primitiveTopology;

		GLuint		vertexShaderId;
		GLuint		pixelShaderId;

		GLenum		indexType;
	};
}

#endif // __TIKI_GRAPHICSCONTEXT_OPENGL4_HPP_INCLUDED__
