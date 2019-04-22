#include "tiki/texture_converter/texture_converter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/base/string_tools.hpp"
#include "tiki/container/array.hpp"
#include "tiki/converter_base/conversion_types.hpp"
#include "tiki/converter_base/resource_writer.hpp"
#include "tiki/texture_export/hdr_image.hpp"
#include "tiki/texture_export/texture_writer.hpp"

#include "base.hpp"

namespace tiki
{
	static const char* s_pTextureTypeName = "texture";
	static const crc32 s_textureTypeCrc = crcString( s_pTextureTypeName );

	uint32 TextureConverter::getConverterRevision( crc32 typeCrc ) const
	{
		return 9u;
	}

	bool TextureConverter::canConvertType( crc32 typeCrc ) const
	{
		return typeCrc == s_textureTypeCrc;
	}

	void TextureConverter::getInputExtensions( List< string >& extensions ) const
	{
		extensions.pushBack( ".psd" );
		extensions.pushBack( ".png" );
	}

	const char* TextureConverter::getOutputType() const
	{
		return s_pTextureTypeName;
	}

	bool TextureConverter::initializeConverter()
	{
		return true;
	}

	void TextureConverter::disposeConverter()
	{
	}

	bool TextureConverter::startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const
	{
		HdrImage image;
		if( !image.createFromFile( asset.inputFilePath.getCompletePath() ) )
		{
			return false;
		}

		string mapCrop = asset.parameters.getOptionalString( "crop", "" );
		if ( !mapCrop.isEmpty() )
		{
			Array< string > values;
			mapCrop.split( values, "," );

			if ( values.getCount() != 4u )
			{
				TIKI_TRACE_WARNING( "crop parameter has a wrong value. usage: 'top,left,width,height'" );
				return false;
			}
			else
			{
				uint4 rect = { 0, 0, uint32( image.getWidth() ), uint32( image.getHeight() ) };
				uint32* pRect = &rect.x;
				for (size_t k = 0u; k < 4u; ++k)
				{
					pRect[ k ] = string_tools::parseUInt32( values[ k ].cStr() );
				}

				image.cropImage( rect );
			}

			values.dispose();
		}

		if( asset.parameters.getOptionalString( "gamma_mode", "" ) == "linear" )
		{
			image.covertGamma( HdrImage::GammaType_Linear );
		}

		const string scaleX = asset.parameters.getOptionalString( "width", "/1" );
		const string scaleY = asset.parameters.getOptionalString( "height", "/1" );

		if ( ( scaleX[ 0u ] == '/' || scaleX[ 0u ] == '*' ) && ( scaleY[ 0u ] == '/' || scaleY[ 0u ] == '*' ) )
		{
			uint2 scale;
			scale.x = uint32( image.getWidth() );
			scale.y = uint32( image.getHeight() );

			const uint32 modX = string_tools::parseUInt32( scaleX.subString( 1u ).cStr() );
			const uint32 modY = string_tools::parseUInt32( scaleY.subString( 1u ).cStr() );

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
		writerParameters.targetFormat = image.getGammaType() == HdrImage::GammaType_SRGB ? PixelFormat_R8G8B8A8_Gamma : PixelFormat_R8G8B8A8;

		writerParameters.mipMapCount = 1u;
		if ( asset.parameters.getOptionalBool( "generate_mipmaps", true ) )
		{
			writerParameters.mipMapCount = ( 63u - countLeadingZeros64( TIKI_MIN( image.getWidth(), image.getHeight() ) ) ) + 1u;
		}

		const string dimentionsString = asset.parameters.getOptionalString( "type" , "2d" ).toLower();
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
			writerParameters.data.texture3d.sliceWidth = asset.parameters.getInt( "slice_size" );
			writerParameters.data.texture3d.sliceHeight = image.getHeight();
		}
		else if ( dimentionsString == "cube" )
		{
			writerParameters.targetType = TextureType_Cube;
		}
		else
		{
			TIKI_TRACE_ERROR( "texture type '%s' not supported.\n", dimentionsString.cStr() );
			return false;
		}

		TextureWriter textureWriter;
		if( !textureWriter.create( image, writerParameters ) )
		{
			return false;
		}

		ResourceWriter writer;
		openResourceWriter( writer, result, asset.assetName.cStr(), "texture" );
		List< ResourceDefinition > definitions = getResourceDefinitions( FlagMask8< ResourceDefinitionFeature >( ResourceDefinitionFeature_GraphicsApi ) );
		for( const ResourceDefinition& definition : definitions )
		{
			writer.openResource( asset.assetName + ".texture", TIKI_FOURCC( 'T', 'E', 'X', 'R' ), definition, getConverterRevision( s_textureTypeCrc ) );

			const ReferenceKey& textureDataKey = textureWriter.writeTextureData( writer, definition.getGraphicsApi() );

			ResourceSectionWriter sectionWriter;
			writer.openDataSection( sectionWriter, SectionType_Initializaion );
			sectionWriter.writeData( &textureWriter.getDescription(), sizeof( textureWriter.getDescription() ) );
			sectionWriter.writeReference( &textureDataKey );
			writer.closeDataSection( sectionWriter );
			writer.closeResource();
		}

		closeResourceWriter( writer );
		textureWriter.dispose();
		image.dispose();

		return true;
	}
}