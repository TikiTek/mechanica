
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

	bool TextureWriter::create( const HdrImage& image, const TextureWriterParameters& parameters )
	{
		m_pImage					= &image;
		m_parameters				= parameters;

		m_description.width			= uint16( m_pImage->getWidth() );
		m_description.height		= uint16( m_pImage->getHeight() );
		m_description.depth			= 1u;
		m_description.arrayCount	= 1u;
		m_description.mipCount		= uint16( parameters.mipMapCount );

		m_description.format		= parameters.targetFormat;
		m_description.type			= parameters.targetType;
		m_description.flags			= TextureFlags_ShaderInput;

		if ( parameters.targetType == TextureType_3d )
		{
			m_description.width = uint16( parameters.data.texture3d.sliceSize );
			m_description.depth	= uint16( m_pImage->getWidth() / m_description.width );
		}

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
		uint depth	= TIKI_MAX( m_description.depth, 1u );

		uint sliceSize = m_description.width;
		if ( m_parameters.targetType == TextureType_3d ) 
		{
			sliceSize = m_parameters.data.texture3d.sliceSize;
		}

		for (uint mipLevel = 0u; mipLevel <= m_description.mipCount; ++mipLevel)
		{
			for (uint depthLevel = 0u; depthLevel < depth; ++depthLevel)
			{
				HdrImage mipImage;
				mipImage.createFromImage( *m_pImage );

				uint4 cropData;
				cropData.x	= uint32( depthLevel * sliceSize );
				cropData.y	= 0u;
				cropData.z	= uint32( sliceSize );
				cropData.w	= m_description.height;
				mipImage.cropImage( cropData );

				mipImage.resizeImage( width, height );

				Array< uint8 > bitmap;
				mipImage.convertTo( bitmap, format );

				writer.writeData( bitmap.getBegin(), bitmap.getCount() );

				bitmap.dispose();
				mipImage.dispose();
			} 

			width	/= 2u;
			height	/= 2u;
			depth	= TIKI_MAX( depth / 2u, 1u );
		}

		writer.closeDataSection();

		return dataKey;
	}
}