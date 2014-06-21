#pragma once
#ifndef __TIKI_GRAPHICSSTATES_OPENGL4_HPP_INCLUDED__
#define __TIKI_GRAPHICSSTATES_OPENGL4_HPP_INCLUDED__

#include "tiki/graphics/graphissystemlimits.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	struct BlendStatePlatformData
	{
		BlendStatePlatformData()
		{
			blendEnabled			= GL_FALSE;
			sourceBlend				= GL_INVALID_ENUM;
			destinationBlend		= GL_INVALID_ENUM;
			blendOperation			= GL_INVALID_ENUM;
			colorWriteMask[ 0u ]	= GL_FALSE;
			colorWriteMask[ 1u ]	= GL_FALSE;
			colorWriteMask[ 2u ]	= GL_FALSE;
			colorWriteMask[ 3u ]	= GL_FALSE;
		}

		GLboolean	blendEnabled;
		GLenum		sourceBlend;
		GLenum		destinationBlend;
		GLenum		blendOperation;
		GLboolean	colorWriteMask[ 4u ];
	};

	struct DepthStencilStatePlatformData
	{
		struct StencilPlatformData
		{
			StencilPlatformData()
			{
				stencilFunction			= GL_INVALID_ENUM;

				depthFailOperation		= GL_INVALID_ENUM;
				stencilFailOperation	= GL_INVALID_ENUM;
				stencilPassOperation	= GL_INVALID_ENUM;
			}

			GLenum	stencilFunction;

			GLenum	depthFailOperation;
			GLenum	stencilFailOperation;
			GLenum	stencilPassOperation;
		};

		DepthStencilStatePlatformData()
		{
			depthEnabled		= GL_FALSE;
			depthWriteEnabled	= GL_FALSE;
			stencilEnabled		= GL_FALSE;

			depthFunction		= GL_INVALID_ENUM;

			stencilReadMask		= GL_INVALID_ENUM;
			stencilWriteMask	= GL_INVALID_ENUM;
			stencilRefernce		= GL_INVALID_ENUM;
		}

		GLboolean				depthEnabled;
		GLboolean				depthWriteEnabled;
		GLboolean				stencilEnabled;

		GLenum					depthFunction;

		GLuint					stencilReadMask;
		GLuint					stencilWriteMask;
		GLuint					stencilRefernce;
		StencilPlatformData		frontFace;
		StencilPlatformData		backFace;
	};

	struct RasterizerStatePlatformData
	{
		RasterizerStatePlatformData()
		{
			fillMode		= GL_FALSE;
			fillMode		= GL_INVALID_ENUM;
			cullMode		= GL_INVALID_ENUM;
			windingOrder	= GL_INVALID_ENUM;
		}

		GLenum		fillMode;
		GLboolean	cullEnabled;
		GLenum		cullMode;
		GLenum		windingOrder;
	};

	struct SamplerStatePlatformData
	{
		SamplerStatePlatformData()
		{
			samplerId = GL_INVALID_ENUM;
		}

		GLuint	samplerId;
	};

	struct VertexInputBindingPlatformData
	{
		struct VertexAttribute
		{
			GLuint		elementCount;
			GLenum		elementType;
			GLuint		elementOffset;
			GLboolean	elementNormalized;
		};

		VertexInputBindingPlatformData()
		{
			attributeCount = 0u;
		}

		VertexAttribute		aAttributes[ GraphicsSystemLimits_MaxVertexAttributes ];
		uint				attributeCount;

		GLuint				vertexStride;
	};
}

#endif // __TIKI_GRAPHICSSTATES_OPENGL4_HPP_INCLUDED__
