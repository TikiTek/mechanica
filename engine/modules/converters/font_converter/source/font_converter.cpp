#include "tiki/font_converter/font_converter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/numbers.hpp"
#include "tiki/container/list.hpp"
#include "tiki/converter_base/conversion_types.hpp"
#include "tiki/converter_base/resource_writer.hpp"
#include "tiki/graphics/font_char.hpp"
#include "tiki/texture_export/hdr_image.hpp"
#include "tiki/texture_export/texture_writer.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

#include <math.h>

#include "base.hpp"

namespace tiki
{
	static const char* s_pFontTypeName = "font";
	static const crc32 s_fontTypeCrc = crcString( s_pFontTypeName );

	uint32 FontConverter::getConverterRevision( crc32 typeCrc ) const
	{
		return 2u;
	}

	bool FontConverter::canConvertType( crc32 typeCrc ) const
	{
		return typeCrc == s_fontTypeCrc;
	}

	void FontConverter::getInputExtensions( List< string >& extensions ) const
	{
		extensions.pushBack( ".ttf" );
	}

	const char* FontConverter::getOutputType() const
	{
		return s_pFontTypeName;
	}

	bool FontConverter::initializeConverter()
	{
		return true;
	}

	void FontConverter::disposeConverter()
	{
	}

	bool FontConverter::startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const
	{
		FT_Library library;
		int error = FT_Init_FreeType( &library );

		if ( error )
		{
			TIKI_TRACE_ERROR( "[fontconverter] FreeType initialization failed.\n" );
			return false;
		}

		string allChars = asset.parameters.getOptionalString( "chars", "" );
		if ( allChars.isEmpty() )
		{
			allChars = string( 256u );
			for (uint charIndex = 0u; charIndex < allChars.getLength(); ++charIndex)
			{
				allChars[ charIndex ] = (char)charIndex;
			}
		}

		const bool mode3D	= asset.parameters.getOptionalBool( "3d_mode",  false );
		const int fontSize	= asset.parameters.getOptionalInt( "font_size", 16 );

		FT_Face face;
		error = FT_New_Face( library, asset.inputFilePath.getCompletePath(), 0u, &face );
		if ( error )
		{
			TIKI_TRACE_ERROR( "[fontconverter] Font loading has currupted an error. File: '%s'\n", asset.inputFilePath.getCompletePath() );
			return false;
		}
		error = FT_Set_Pixel_Sizes( face, 0, fontSize );

		FT_GlyphSlot slot = face->glyph;

		const size_t baseSize	= (size_t)sqrtf( (float)fontSize * (float)fontSize * allChars.getLength() );
		const size_t imageSize	= getNextPowerOfTwo( baseSize );
		const float floatSize	= (float)imageSize;

		const uint imageWidth	= ( mode3D ? fontSize * allChars.getLength() : imageSize );
		const uint imageHeight	= ( mode3D ? fontSize : imageSize );

		HdrImage image;
		image.create( imageWidth, imageHeight, HdrImage::GammaType_Linear );

		uint2 imagePos = { 3u, 3u };

		const size_t rowPitch = ( image.getWidth() * image.getChannelCount() );

		List< FontChar > chars;
		for (size_t charIndex = 0u; charIndex < allChars.getLength(); ++charIndex)
		{
			const char c = allChars[ charIndex ];
			const int glyph_index = FT_Get_Char_Index( face, c );

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

			const uint imagePosX = imagePos.x + slot->bitmap_left;
			const uint imagePosY = imagePos.y + slot->bitmap_top;
			const uint charWidth = slot->bitmap.width + slot->bitmap_left + 2u;
			const uint charHeight = slot->bitmap.rows + (fontSize - slot->bitmap_top);

			if ( imagePos.x + charWidth + 1u >= image.getWidth() )
			{
				imagePos.x = 3u;
				imagePos.y += fontSize + 6u;
			}

			if ( !mode3D )
			{
				FontChar& inst = chars.add();
				inst.width	= (float)charWidth;
				inst.height	= (float)charHeight;
				inst.x1		= u16::floatToUnorm( float( imagePos.x )/ floatSize );
				inst.y1		= u16::floatToUnorm( float( imagePos.y ) / floatSize );
				inst.x2		= u16::floatToUnorm( float( imagePos.x + charWidth ) / floatSize );
				inst.y2		= u16::floatToUnorm( float( imagePos.y + charHeight ) / floatSize );

				if ( charIndex == ' ' )
				{
					inst.width = (float)fontSize / 4.0f;
				}
			}

			const uint xOffset = (mode3D ? ((fontSize * charIndex) + (fontSize / 2u)) - (slot->bitmap.width / 2u) : imagePos.x);
			const uint yOffset = (mode3D ? (fontSize / 2u) - (slot->bitmap.rows / 2u) : imagePos.y + (fontSize - slot->bitmap_top));

			float* pRow = image.getData() + ( yOffset * rowPitch ) + ( xOffset * image.getChannelCount() );
			for (size_t y = 0u; y < (size_t)slot->bitmap.rows; ++y)
			{
				for (size_t x = 0u; x < (size_t)slot->bitmap.width; ++x)
				{
					const size_t index = ( y * slot->bitmap.pitch ) + x;

					pRow[ x * image.getChannelCount() ] = (float)slot->bitmap.buffer[ index ] / 255.0f;
				}

				pRow += rowPitch;
			}

			imagePos.x += uint32( charWidth ) + 1u;
		}

		TextureWriterParameters writerParameters;
		writerParameters.targetFormat	= PixelFormat_R8;
		writerParameters.targetType		= (mode3D ? TextureType_3d : TextureType_2d );
		writerParameters.mipMapCount	= 1u;
		writerParameters.data.texture3d.sliceWidth	= fontSize;
		writerParameters.data.texture3d.sliceHeight	= imageHeight;

		TextureWriter textureWriter;
		if( !textureWriter.create( image, writerParameters ) )
		{
			TIKI_TRACE_ERROR( "[fontconverter] Texture writer failed initilaztion.\n" );
			return false;
		}

		ResourceWriter writer;
		openResourceWriter( writer, result, asset.assetName.cStr(), "font" );

		FlagMask8< ResourceDefinitionFeature > features;
		features.setFlag( ResourceDefinitionFeature_GraphicsApi );

		for (const ResourceDefinition& definition : getResourceDefinitions( features ))
		{
			writer.openResource( asset.assetName + ".font", TIKI_FOURCC( 'F', 'O', 'N', 'T' ), definition, getConverterRevision( s_fontTypeCrc ) );

			const ReferenceKey textureDataKey = textureWriter.writeTextureData( writer, definition.getGraphicsApi() );

			// write chars
			ResourceSectionWriter sectionWriter;
			writer.openDataSection( sectionWriter, SectionType_Main );
			const ReferenceKey charArrayKey = sectionWriter.addDataPoint();
			sectionWriter.writeData( chars.getBegin(), chars.getCount() * sizeof( FontChar ) );
			writer.closeDataSection( sectionWriter );

			writer.openDataSection( sectionWriter, SectionType_Initializaion );
			sectionWriter.writeData( &textureWriter.getDescription(), sizeof( textureWriter.getDescription() ) );
			sectionWriter.writeReference( &textureDataKey );
			sectionWriter.writeUInt32( uint32( chars.getCount() ) );
			sectionWriter.writeReference( &charArrayKey );
			writer.closeDataSection(sectionWriter);

			writer.closeResource();
		}

		closeResourceWriter( writer );
		textureWriter.dispose();
		image.dispose();

		FT_Done_FreeType( library );
		return true;
	}
}
