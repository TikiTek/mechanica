#pragma once
#ifndef TIKI_TEXTUREWRITER_HPP__INCLUDED
#define TIKI_TEXTUREWRITER_HPP__INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/graphicsbase/pixelformat.hpp"

namespace tiki
{
	class HdrImage;
	class ResourceWriter;

	class TextureWriter
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( TextureWriter );

	public:

		void		create();
		void		dispose();

		void		writeTexture( ResourceWriter& writer, const HdrImage& image, const PixelFormat format );

	};
}

#endif // TIKI_TEXTUREWRITER_HPP__INCLUDED
