
#include "tiki/textureexport/texturewriter.hpp"

#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/graphicsbase/texturedescription.hpp"
#include "tiki/textureexport/hdrimage.hpp"

namespace tiki
{
	void TextureWriter::create()
	{
	}

	void TextureWriter::dispose()
	{
	}

	void TextureWriter::writeTexture( ResourceWriter& writer, const HdrImage& image, const PixelFormat format )
	{
		TextureDescription description;
		description.width		= image.getWidth();
		description.height		= image.getHeight();
		description.depth		= 1u;
		description.arrayCount	= 1u;

		description.format		= format;
		description.type		= TextureType_2d;
		description.flags		= TextureFlags_ShaderInput;

		Array< uint8 > bitmap;
		image.convertTo( bitmap, format );

		writer.writeUInt32( bitmap.getCount() + sizeof( TextureDescription ) );
		writer.writeData( &description, sizeof( description ) );
		writer.writeData( bitmap.getData(), bitmap.getCount() );

		bitmap.dispose();
	}

}