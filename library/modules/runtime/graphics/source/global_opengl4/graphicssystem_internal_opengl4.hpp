#pragma once
#ifndef __TIKI_GRAPHICSSYSTEM_INTERNAL_OPENGL4_HPP_INCLUDED__
#define __TIKI_GRAPHICSSYSTEM_INTERNAL_OPENGL4_HPP_INCLUDED__

#include <windows.h>

#include "GL/glew.h"
#include "GL/GL.h"

#include "graphicstypes_opengl4.hpp"

namespace tiki
{
	class ShaderLinker;

	namespace graphics
	{
		bool			checkError();

		ShaderLinker&	getShaderLinker( GraphicsSystem& graphicsSystem );
	}
}

#endif // __TIKI_GRAPHICSSYSTEM_INTERNAL_OPENGL4_HPP_INCLUDED__
