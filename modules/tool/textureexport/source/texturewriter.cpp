
#include "tiki/textureexport/texturewriter.hpp"

#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/graphics/texturedescription.hpp"
#include "tiki/textureexport/hdrimage.hpp"

namespace tiki
{
	TextureWriter::TextureWriter()
	{
		m_pImage = nullptr;
	}

	TextureWriter::~TextureWriter()
	{
		TIKI_ASSERT( m_pImage == nullptr );
	}

	bool TextureWriter::create( const HdrImage& image, PixelFormat targetFormat, uint mipMapCount )
	{
		m_pImage		= &image;

		m_description.width			= m_pImage->getWidth();
		m_description.height		= m_pImage->getHeight();
		m_description.depth			= 1u;
		m_description.arrayCount	= 1u;
		m_description.mipCount		= mipMapCount;

		m_description.format		= targetFormat;
		m_description.type			= TextureType_2d;
		m_description.flags			= TextureFlags_ShaderInput;

		return true;
	}

	void TextureWriter::dispose()
	{
		m_pImage = nullptr;
	}

	ReferenceKey TextureWriter::writeTextureData( ResourceWriter& writer )
	{
		TIKI_ASSERT( m_pImage != nullptr );
		
		const PixelFormat format = (PixelFormat)m_description.format;

		writer.openDataSection( 0u, AllocatorType_MainMemory );
		const ReferenceKey dataKey = writer.addDataPoint();

		uint width	= m_description.width;
		uint height	= m_description.height;

		for (uint mipLevel = 0u; mipLevel <= m_description.mipCount; ++mipLevel)
		{
			HdrImage mipImage;
			mipImage.createFromImage( *m_pImage );

			if ( mipLevel != 0u )
			{
				mipImage.resizeImage( width, height );
			}

			Array< uint8 > bitmap;
			mipImage.convertTo( bitmap, format );

			writer.writeData( bitmap.getData(), bitmap.getCount() );

			bitmap.dispose();
			mipImage.dispose();

			width	/= 2u;
			height	/= 2u;
		}

		writer.closeDataSection();

		return dataKey;
	}
}