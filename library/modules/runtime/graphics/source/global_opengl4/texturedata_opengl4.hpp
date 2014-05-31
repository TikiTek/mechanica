#pragma once
#ifndef TIKI_TEXTUREDATA_OPENGL4_HPP__INCLUDED
#define TIKI_TEXTUREDATA_OPENGL4_HPP__INCLUDED

#include "graphicstypes_opengl4.hpp"

namespace tiki
{
	struct TextureDataPlatform
	{
		GLuint	textureId;
	};

	namespace graphics
	{
		GLenum	getGlFormat( PixelFormat pixelFormat );
		GLenum	getGlChannelType( PixelFormat pixelFormat );
	}
}

#endif // TIKI_TEXTUREDATA_OPENGL4_HPP__INCLUDED
