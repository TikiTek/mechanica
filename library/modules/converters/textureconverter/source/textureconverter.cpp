
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
			const ConversionParameters::InputFile& file = params.inputFiles[ i ];

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
					uint32* pRect = &rect.x;
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

			if ( ( scaleX[ 0u ] == '/' || scaleX[ 0u ] == '*' ) && ( scaleY[ 0u ] == '/' || scaleY[ 0u ] == '*' ) )
			{
				uint2 scale;
				scale.x = image.getWidth();
				scale.y = image.getHeight();

				const uint modX = ParseString::parseUInt32( scaleX.substring( 1u ) );
				const uint modY = ParseString::parseUInt32( scaleY.substring( 1u ) );

				if ( scaleX[ 0u ] == '/' )
				{
					scale.x /= modX;
				}
				else
				{
					scale.x *= modX;
				}

				if ( scaleY[ 0u ] == '/' )
				{
					scale.y /= modY;
				}
				else
				{
					scale.y *= modY;
				}

				if ( scale.x != image.getWidth() || scale.y != image.getHeight() )
				{
					image.resizeImage( scale );
				}
			}

			uint mipMapCount = 1u;
			if ( params.arguments.getOptionalBool( "generate_mipmaps", true ) )
			{
				mipMapCount = 63u - countLeadingZeros64( TIKI_MAX( image.getWidth(), image.getHeight() ) );
			}

			TextureWriter textureWriter;
			textureWriter.create( image, PixelFormat_R8G8B8A8, mipMapCount );

			ResourceWriter writer;
			openResourceWriter( writer, params.outputName, "texture", params.targetPlatform );
			writer.openResource( params.outputName + ".texture", TIKI_FOURCC( 'T', 'E', 'X', 'R' ), getConverterRevision() );

			const ReferenceKey& textureDataKey = textureWriter.writeTextureData( writer );

			writer.openDataSection( 0u, AllocatorType_InitializaionMemory );
			writer.writeData( &textureWriter.getDescription(), sizeof( textureWriter.getDescription() ) );
			writer.writeReference( &textureDataKey );
			writer.closeDataSection();

			writer.closeResource();
			closeResourceWriter( writer );

			textureWriter.dispose();
			image.dispose();
		}

		return true;
	}
}