
#include "tiki/fontconverter/fontconverter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/numbers.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/graphicsbase/fontchar.hpp"
#include "tiki/textureexport/hdrimage.hpp"
#include "tiki/textureexport/texturewriter.hpp"
#include "tiki/toolbase/list.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

namespace tiki
{
	crc32 FontConverter::getInputType() const
	{
		return crcString( "font" );
	}

	crc32 FontConverter::getOutputType() const
	{
		return crcString( "font" );
	}

	void FontConverter::getDependingType( List< crc32 >& types ) const
	{
	}

	bool FontConverter::initializeConverter()
	{
		return true;
	}

	void FontConverter::disposeConverter()
	{
	}

	bool FontConverter::startConversionJob( const ConversionParameters& params ) const
	{
		FT_Library library;
		int error = FT_Init_FreeType( &library );

		if ( error )
		{
			TIKI_TRACE_ERROR( "[fontconverter] FreeType initialization failed.\n" );
			return false;
		}

		const int fontSize = params.arguments.getOptionalInt( "font_size", 16 ) ;

		for (size_t i = 0u; i < params.inputFiles.getCount(); ++i)
		{			
			const ConversionParameters::InputFile& file = params.inputFiles[ i ];

			FT_Face face;
			error = FT_New_Face( library, file.fileName.cStr(), 0u, &face );;

			if ( error )
			{
				TIKI_TRACE_ERROR( "[fontconverter] Font loading has currupted an error. File: '%s'\n", file.fileName.cStr() );
				continue;
			}

			error = FT_Set_Pixel_Sizes( face, 0, fontSize );

			FT_GlyphSlot slot = face->glyph;

			const size_t baseSize	= (size_t)sqrtf( (float)fontSize * (float)fontSize * 256.0f );
			const size_t imageSize	= nextPowerOfTwo( baseSize );
			const float floatSize	= (float)imageSize;

			HdrImage image;
			image.create( imageSize, imageSize, HdrImage::GammaType_Linear );

			uint2 imagePos = { 3u, 3u };

			const size_t rowPitch = ( image.getWidth() * image.getChannelCount() );

			List< FontChar > chars;
			for (size_t j = 0u; j < 256u; ++j)
			{
				int glyph_index = FT_Get_Char_Index( face, j );

				error = FT_Load_Glyph( face, glyph_index,  FT_LOAD_DEFAULT ); 

				if ( error )
				{
					continue;
				}

				error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );

				if ( error )
				{
					continue;
				}

				if ( imagePos.x + slot->bitmap.width + 3u >= image.getWidth() )
				{
					imagePos.x	= 3u;
					imagePos.y += fontSize + 6u;
				}
				
				FontChar& inst = chars.add();
				inst.width	= (float)slot->bitmap.width + 2.0f;
				inst.height	= (float)fontSize;
				inst.x1		= u16::floatToUnorm( (float)imagePos.x / floatSize );
				inst.y1		= u16::floatToUnorm( (float)imagePos.y / floatSize );
				inst.x2		= u16::floatToUnorm( ( (float)( imagePos.x + slot->bitmap.width ) + 2.0f ) / floatSize );
				inst.y2		= u16::floatToUnorm( (float)( imagePos.y + fontSize ) / floatSize );

				if ( j == ' ' )
				{
					inst.width = (float)fontSize / 4.0f;
				}

				float* pRow = image.getData() + ( imagePos.y * rowPitch ) + ( imagePos.x * image.getChannelCount() ) + ( ( fontSize - slot->bitmap.rows ) * rowPitch );
				
				for (size_t y = 0u; y < (size_t)slot->bitmap.rows; ++y)
				{
					for (size_t x = 0u; x < (size_t)slot->bitmap.width; ++x)
					{
						const size_t index = ( y * slot->bitmap.pitch ) + x;

						pRow[ x * image.getChannelCount() ] = (float)slot->bitmap.buffer[ index ] / 255.0f;
					}

					pRow += rowPitch;
				}

				imagePos.x += slot->bitmap.width + 3u;
			}

			TextureWriter textureWriter;
			textureWriter.create( image, PixelFormat_R8, 0u );
			
			ResourceWriter writer;
			openResourceWriter( writer, params.outputName, "font", params.targetPlatform );
			writer.openResource( params.outputName + ".font", TIKI_FOURCC( 'F', 'O', 'N', 'T' ), getConverterRevision() );

			const ReferenceKey textureDataKey = textureWriter.writeTextureData( writer );

			// write chars
			writer.openDataSection( 0u, AllocatorType_MainMemory );
			const ReferenceKey charArrayKey = writer.addDataPoint();
			writer.writeData( chars.getData(), chars.getCount() * sizeof( FontChar ) );
			writer.closeDataSection();

			writer.openDataSection( 0u, AllocatorType_InitializaionMemory );
			writer.writeData( &textureWriter.getDescription(), sizeof( textureWriter.getDescription() ) );
			writer.writeReference( &textureDataKey );
			writer.writeUInt32( chars.getCount() );
			writer.writeReference( &charArrayKey );
			writer.closeDataSection();

			closeResourceWriter( writer );

			textureWriter.dispose();
			image.dispose();
		}

		FT_Done_FreeType( library );

		return true;
	}

}