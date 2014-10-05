#pragma once
#ifndef __TIKI_GRAPHICSCONTEXT_OPENGL4_HPP_INCLUDED__
#define __TIKI_GRAPHICSCONTEXT_OPENGL4_HPP_INCLUDED__

#include "graphicstypes_opengl4.hpp"
#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	class VertexInputBinding;
	struct ShaderPlatformData;

	struct GraphicsContextPlatformData
	{
		GraphicsContextPlatformData()
		{
			maxVertexAttributeCount	= 0;

			indexType					= GL_INVALID_ENUM;
			primitiveTopology			= GL_INVALID_ENUM;

			pVertexShaderData			= nullptr;
			pPixelShaderData			= nullptr;

			pApplyedVertexInutBinding	= nullptr;

			programCrc					= InvalidCrc32;
		}

		uint						maxVertexAttributeCount;

		GLenum						indexType;
		GLenum						primitiveTopology;

		const ShaderPlatformData*	pVertexShaderData;
		const ShaderPlatformData*	pPixelShaderData;

		GLuint						aVertexConstantBufferIds[ GraphicsSystemLimits_VertexShaderConstantSlots ];
		GLuint						aPixelConstantBufferIds[ GraphicsSystemLimits_PixelShaderConstantSlots ];
		const VertexInputBinding*	pApplyedVertexInutBinding;

		GLuint						programId;
		crc32						programCrc;
	};
}

#endif // __TIKI_GRAPHICSCONTEXT_OPENGL4_HPP_INCLUDED__
