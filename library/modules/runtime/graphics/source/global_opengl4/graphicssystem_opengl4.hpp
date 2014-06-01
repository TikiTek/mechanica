#pragma once
#ifndef __TIKI_GRAPHICSSYSTEM_OPENGL4_HPP_INCLUDED__
#define __TIKI_GRAPHICSSYSTEM_OPENGL4_HPP_INCLUDED__

#include "tiki/graphics/texturedata.hpp"

#include "../../../source/global_opengl4/graphicstypes_opengl4.hpp"

namespace tiki
{
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

	};
}

#endif // __TIKI_GRAPHICSSYSTEM_OPENGL4_HPP_INCLUDED__
