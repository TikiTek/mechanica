
#include "tiki/textureconverter/textureconverter.hpp"

#include "tiki/base/array.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/base/stringparse.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/convertermanager.hpp"
#include "tiki/converterbase/resourcewriter.hpp"

#include "tiki/base/reflection.hpp"
#include "tiki/textureexport/hdrimage.hpp"
#include "tiki/textureexport/texturewriter.hpp"

namespace tiki
{
	crc32 TextureConverter::getInputType() const
	{
		return crcString( "texture" );
	}

	crc32 TextureConverter::getOutputType() const
	{
		return crcString( "texture" );
	}

	void TextureConverter::getDependingType( List< crc32 >& types ) const
	{
	}

	bool TextureConverter::initializeConverter()
	{
		return true;
	}

	void TextureConverter::disposeConverter()
	{
	}
	
	bool TextureConverter::startConversionJob( const ConversionParameters& params ) const
	{
		for (size_t i = 0u; i < params.inputFiles.getCount(); ++i)
		{
			const ConversionInputFile& file = params.inputFiles[ i ];

			HdrImage image;
			
			if ( image.createFromFile( file.fileName ) == false )
			{
				continue;
			}

			string mapCrop = params.arguments.getOptionalString( "crop", "" );
			if ( !mapCrop.isEmpty() )
			{
				Array< string > values;
				mapCrop.split( values, "," );

				if ( values.getCount() != 4u )
				{
					TIKI_TRACE_WARNING( "crop parameter has a wrong value. usage: 'top,left,width,height'" );
					continue;
				}
				else
				{
					uint4 rect = { 0, 0, image.getWidth(), image.getHeight() };
					uint* pRect = &rect.x;
					for (size_t k = 0u; k < 4u; ++k)
					{
						pRect[ k ] = ParseString::parseUInt32( values[ k ] );
					}

					image.cropImage( rect );
				}

				values.dispose();
			}

			const string scaleX = params.arguments.getOptionalString( "width", "/1" );
			const string scaleY = params.arguments.getOptionalString( "height", "/1" );

			if ( scaleX[ 0u ] == '/' && scaleY[ 0u ] == '/')
			{
				uint2 scale;
				scale.x = image.getWidth() / ParseString::parseUInt32( scaleX.substring( 1u ) );
				scale.y = image.getHeight() / ParseString::parseUInt32( scaleY.substring( 1u ) );

				if ( scale.x != image.getWidth() || scale.y != image.getHeight() )
				{
					image.resizeImage( scale );
				}
			}


			ResourceWriter writer;
			openResourceWriter( &writer, params.outputName, "ttx", params.targetPlatform );
			writer.openResource( params.outputName, TIKI_FOURCC( 'T', 'E', 'X', 'R' ), getConverterRevision() );

			TextureWriter textureWriter;
			textureWriter.create();
			const ReferenceKey textureDataKey = textureWriter.writeTexture( writer, image, PixelFormat_R8G8B8A8 );
			textureWriter.dispose();

			writer.openDataSection( 0u, AllocatorType_InitializaionMemory );
			writer.writeReference( textureDataKey );
			writer.closeDataSection();

			writer.closeResource();
			closeResourceWriter( &writer );
			image.dispose();
		}

		return true;
	}
}