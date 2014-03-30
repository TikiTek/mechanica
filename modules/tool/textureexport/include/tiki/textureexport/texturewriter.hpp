#pragma once
#ifndef TIKI_TEXTUREWRITER_HPP__INCLUDED
#define TIKI_TEXTUREWRITER_HPP__INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/graphics/texturedescription.hpp"

namespace tiki
{
	class HdrImage;
	class ResourceWriter;

	class TextureWriter
	{
		TIKI_NONCOPYABLE_CLASS( TextureWriter );

	public:

		TextureWriter();
		~TextureWriter();

		bool						create( const HdrImage& image, PixelFormat targetFormat, uint mipMapCount );
		void						dispose();

		const TextureDescription&	getDescription() const { return m_description; }

		ReferenceKey				writeTextureData( ResourceWriter& writer );

	private:

		TextureDescription	m_description;
		const HdrImage*		m_pImage;

	};
}

#endif // TIKI_TEXTUREWRITER_HPP__INCLUDED
