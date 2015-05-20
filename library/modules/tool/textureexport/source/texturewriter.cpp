
#include "tiki/textureexport/texturewriter.hpp"

#include "tiki/base/structs.hpp"
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
		
		if( parameters.targetType == TextureType_1d ||
			parameters.targetType == TextureType_2d )
		{
			// nothing to do here
		}
		else if ( parameters.targetType == TextureType_3d )
		{
			if( parameters.data.texture3d.sliceWidth == 0 ||
				parameters.data.texture3d.sliceHeight == 0 )
			{
				TIKI_TRACE_ERROR( "[TextureWriter] slice size can't be zero." );
				return false;
			}

			if( parameters.data.texture3d.sliceWidth > m_description.width ||
				parameters.data.texture3d.sliceHeight > m_description.height )
			{
				TIKI_TRACE_ERROR( "[TextureWriter] slice size can't be greater as image size." );
				return false;
			}

			if( m_description.width % parameters.data.texture3d.sliceWidth != 0 ||
				m_description.height % parameters.data.texture3d.sliceHeight != 0 )
			{
				TIKI_TRACE_ERROR( "[TextureWriter] invalid 3d texture size. width or height is not dividable by slice size." );
				return false;
			}

			const uint sliceCount = (m_pImage->getWidth() / parameters.data.texture3d.sliceWidth) * (m_pImage->getHeight() / parameters.data.texture3d.sliceHeight);
			m_description.width = uint16( parameters.data.texture3d.sliceWidth );
			m_description.height = uint16( parameters.data.texture3d.sliceHeight );
			m_description.depth	= uint16( sliceCount );
		}
		else if ( parameters.targetType == TextureType_Cube )
		{
			if ( image.getWidth() / 4u != image.getHeight() / 3u )
			{
				TIKI_TRACE_ERROR("[TextureWriter] cube texture has a invalid ratio. ratio must be 4:3!");
				return false;
			}

			m_description.width			= uint16( m_pImage->getWidth() / 4u );
			m_description.height		= uint16( m_pImage->getHeight() / 3u );
			m_description.arrayCount	= m_description.mipCount * 6u;
		}

		if ( parameters.mipMapCount == 0u )
		{
			TIKI_TRACE_ERROR( "[TextureWriter] MipMapCount == 0 is not allowed.\n" );
			return false;
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

		List<uint4> sourceRects;
		if( m_parameters.targetType == TextureType_1d ||
			m_parameters.targetType == TextureType_2d )
		{
			createUint4( sourceRects.add(), 0u, 0u, m_description.width, m_description.height );
		}
		else if ( m_parameters.targetType == TextureType_3d )
		{
			for (uint32 i = 0u; i < m_description.depth; ++i)
			{
				createUint4( sourceRects.add(), m_description.width * i, 0u, m_description.width, m_description.height );
			}
		}
		else if ( m_parameters.targetType == TextureType_Cube )
		{
			const uint32 sliceWidth = uint32( m_pImage->getWidth() / 4u );
			const uint32 sliceHeight = uint32( m_pImage->getHeight() / 3u );

			createUint4( sourceRects.add(), sliceWidth * 2, sliceHeight * 1, sliceWidth, sliceHeight );	// x+
			createUint4( sourceRects.add(), sliceWidth * 0, sliceHeight * 1, sliceWidth, sliceHeight );	// x-
			createUint4( sourceRects.add(), sliceWidth * 1, sliceHeight * 2, sliceWidth, sliceHeight );	// y+
			createUint4( sourceRects.add(), sliceWidth * 1, sliceHeight * 0, sliceWidth, sliceHeight );	// y-
			createUint4( sourceRects.add(), sliceWidth * 1, sliceHeight * 1, sliceWidth, sliceHeight );	// z+
			createUint4( sourceRects.add(), sliceWidth * 3, sliceHeight * 1, sliceWidth, sliceHeight );	// z-
		}

		uint width	= m_description.width;
		uint height	= m_description.height;
		for (uint mipLevel = 0u; mipLevel < m_description.mipCount; ++mipLevel)
		{
			for (uint sourceRectIndex = 0u; sourceRectIndex < sourceRects.getCount(); ++sourceRectIndex)
			{
				HdrImage mipImage;
				mipImage.createFromImage( *m_pImage );
				mipImage.cropImage( sourceRects[ sourceRectIndex ] );
				mipImage.resizeImage( width, height );
				//mipImage.flipImage( HdrImage::FlipDirection_Vertical );

				Array< uint8 > bitmap;
				mipImage.convertTo( bitmap, format );

				switch ( m_parameters.targetApi )
				{
				case GraphicsApi_D3D11:
				case GraphicsApi_D3D12:
					writer.writeData( bitmap.getBegin(), bitmap.getCount() );
					break;

				case GraphicsApi_OpenGL4:
					{
						const uint bytesPerPixel = getBitsPerPixel( format ) / 8u;
						const uint bytesPerLine = bytesPerPixel * width;

						for (uint y = height - 1u; y < height; --y)
						{
							const uint8* pSourceData = bitmap.getBegin() + (bytesPerLine * y);
							writer.writeData( pSourceData, bytesPerLine );
						}
					}
					break;

				default:
					TIKI_TRACE_ERROR( "[TextureWriter] Graphics API not supported.\n" );
					break;
				}
				
				bitmap.dispose();
				mipImage.dispose();

				if ( m_parameters.targetType == TextureType_3d )
				{
					sourceRectIndex += mipLevel;
				}
			}

			width	= TIKI_MAX( width / 2u, 1u );
			height	= TIKI_MAX( height / 2u, 1u );
		}

		writer.closeDataSection();

		return dataKey;
	}
}
