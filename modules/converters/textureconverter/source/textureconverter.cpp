
#include "tiki/textureconverter/textureconverter.hpp"

#include "tiki/base/array.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/stringparse.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/convertermanager.hpp"
#include "tiki/converterbase/resourcewriter.hpp"

#include "tiki/base/reflection.hpp"
#include "tiki/textureexport/hdrimage.hpp"
#include "tiki/textureexport/texturewriter.hpp"

#include "base.hpp"

namespace tiki
{
	uint16 TextureConverter::getConverterRevision() const
	{
		return 5u;
	}

	bool TextureConverter::canConvertType( crc32 typeCrc ) const
	{
		static const crc32 s_typeCrc = crcString( "texture" );
		return typeCrc == s_typeCrc;
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
	
	bool TextureConverter::startConversionJob( ConversionResult& result, const ConversionParameters& params ) const
	{
		for (size_t i = 0u; i < params.inputFiles.getCount(); ++i)
		{			
			const ConversionParameters::InputFile& file = params.inputFiles[ i ];

			HdrImage image;
			
			if ( image.createFromFile( file.fileName.cStr() ) == false )
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
					uint4 rect = { 0, 0, uint32( image.getWidth() ), uint32( image.getHeight() ) };
					uint32* pRect = &rect.x;
					for (size_t k = 0u; k < 4u; ++k)
					{
						pRect[ k ] = ParseString::parseUInt32( values[ k ].cStr() );
					}

					image.cropImage( rect );
				}

				values.dispose();
			}

			if ( params.arguments.getOptionalString( "gamma_mode", "" ) == "linear" )
			{
				image.covertGamma( HdrImage::GammaType_Linear );
			}

			const string scaleX = params.arguments.getOptionalString( "width", "/1" );
			const string scaleY = params.arguments.getOptionalString( "height", "/1" );

			if ( ( scaleX[ 0u ] == '/' || scaleX[ 0u ] == '*' ) && ( scaleY[ 0u ] == '/' || scaleY[ 0u ] == '*' ) )
			{
				uint2 scale;
				scale.x = uint32( image.getWidth() );
				scale.y = uint32( image.getHeight() );

				const uint32 modX = ParseString::parseUInt32( scaleX.subString( 1u ).cStr() );
				const uint32 modY = ParseString::parseUInt32( scaleY.subString( 1u ).cStr() );

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

			TextureWriterParameters writerParameters;
			writerParameters.targetFormat	= PixelFormat_R8G8B8A8;
			writerParameters.targetApi		= params.targetApi;
				
			writerParameters.mipMapCount = 1u;
			if ( params.arguments.getOptionalBool( "generate_mipmaps", true ) )
			{
				writerParameters.mipMapCount = ( 63u - countLeadingZeros64( TIKI_MIN( image.getWidth(), image.getHeight() ) ) ) + 1u;
			}

			const string dimentionsString = params.arguments.getOptionalString( "type" , "2d" ).toLower();
			if ( dimentionsString == "1d" )
			{
				writerParameters.targetType = TextureType_1d;
			}
			else if ( dimentionsString == "2d" )
			{
				writerParameters.targetType = TextureType_2d;
			}
			else if ( dimentionsString == "3d" )
			{
				writerParameters.targetType = TextureType_3d;
				writerParameters.data.texture3d.sliceWidth = params.arguments.getInt( "slice_size" );
				writerParameters.data.texture3d.sliceHeight = image.getHeight();
			}
			else if ( dimentionsString == "cube" )
			{
				writerParameters.targetType = TextureType_Cube;
			}
			else
			{
				TIKI_TRACE_ERROR( "texture type '%s' not supported.\n", dimentionsString.cStr() );
				continue;
			}

			TextureWriter textureWriter;
			if ( !textureWriter.create( image, writerParameters ) )
			{
				continue;
			}

			ResourceWriter writer;
			openResourceWriter( writer, result, params.outputName, "texture", params.targetPlatform );
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