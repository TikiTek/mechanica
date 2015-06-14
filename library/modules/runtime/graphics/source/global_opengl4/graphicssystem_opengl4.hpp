#pragma once
#ifndef __TIKI_GRAPHICSSYSTEM_OPENGL4_HPP_INCLUDED__
#define __TIKI_GRAPHICSSYSTEM_OPENGL4_HPP_INCLUDED__

#include "tiki/base/platform.hpp"
#include "tiki/graphics/texturedata.hpp"

#include "graphicstypes_opengl4.hpp"
#include "shaderlinker_opengl4.hpp"

namespace tiki
{
	enum
	{
		GraphicsSystemLimits_MaxProgramCount = 1024u
	};

	struct GraphicsSystemPlatformData
	{
		GraphicsSystemPlatformData()
		{
		}

		WindowHandle		windowHandle;

		int					pixelFormatHandle;
		DeviceContextHandle	deviceContextHandle;
		RenderContextHandle	renderContextHandle;

		GLuint				frameBufferId;
		GLuint				depthBufferId;
		TextureData			backBufferData;

		ShaderLinker		shaderLinker;
	};
}

#endif // __TIKI_GRAPHICSSYSTEM_OPENGL4_HPP_INCLUDED__
