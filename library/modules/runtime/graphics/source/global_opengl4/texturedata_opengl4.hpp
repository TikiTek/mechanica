#pragma once
#ifndef TIKI_TEXTUREDATA_OPENGL4_HPP__INCLUDED
#define TIKI_TEXTUREDATA_OPENGL4_HPP__INCLUDED

#include "graphicstypes_opengl4.hpp"

namespace tiki
{
	struct TextureDataPlatform
	{
		TextureDataPlatform()
		{
			textureId = 0u;
		}

		GLuint	textureId;
		GLenum	textureType;
	};

	namespace graphics
	{
		GLenum	getGlFormat( PixelFormat pixelFormat );
		GLenum	getGlChannelType( PixelFormat pixelFormat );
		GLenum	getGlTextureType( TextureType type );
	}
}

#endif // TIKI_TEXTUREDATA_OPENGL4_HPP__INCLUDED
