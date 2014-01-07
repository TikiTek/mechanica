#include "tiki/materialconverter/materialconverter.hpp"
#include "tiki/materialconverter/toolmaterial.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/resourcewriter.hpp"


namespace tiki
{
	MaterialConverter::MaterialConverter()
	{
	}

	MaterialConverter::~MaterialConverter()
	{
	}

	tiki::crc32 MaterialConverter::getInputType() const
	{
		return crcString( "material" ); 
	}

	tiki::crc32 MaterialConverter::getOutputType() const
	{
		return crcString( "material" );
	}

	void MaterialConverter::getDependingType( List< crc32 >& types ) const
	{
	}

	bool MaterialConverter::initializeConverter()
	{
		return true;
	}

	void MaterialConverter::disposeConverter()
	{
	}

	bool MaterialConverter::startConversionJob( const ConversionParameters& params ) const
	{
		for( size_t i = 0u; i < params.inputFiles.getCount(); ++i )
		{
			const ConversionInputFile& file = params.inputFiles[ i ];
			ToolMaterial material;
			
			if( !material.create( file.fileName ) )
			{
				material.dispose();
				continue;
			}

			ResourceWriter writer;
			openResourceWriter( writer, params.outputName, "material", PlatformType_Win );

			writer.openResource( params.outputName + ".material", TIKI_FOURCC( 'M', 'A', 'T', 'E' ), 1u );

			writer.openDataSection( 0u, AllocatorType_InitializaionMemory );

			const ReferenceKey& textureKey = writer.addResourceLink( "checker.texture", crcString( "checker.texture" ), TIKI_FOURCC( 'T', 'E', 'X', 'R' ) );

			writer.writeUInt8( 0u );	// renderEffectId
			writer.writeUInt8( 1u );	// renderPassMask
			writer.writeUInt8( 0u );	// renderFlags
			writer.writeUInt8( 4u );	// defaultTextureOffset
			writer.writeReference( &textureKey );

			writer.closeDataSection();

			//const uint vertexLength = material.getVertexShader().length();
			//writer.writeUInt32( vertexLength );
			//writer.writeData( material.getVertexShader().cStr(), vertexLength );

			//const uint pixelLength = material.getPixelShader().length();
			//writer.writeUInt32( pixelLength );
			//writer.writeData( material.getPixelShader().cStr(), pixelLength );

			//List<TextureAsset>& assets = material.getAssets();
			//writer.writeUInt32( assets.getCount() );

			//for( uint i = 0; i < assets.getCount(); ++i )
			//{
			//	const TextureAsset& asset = assets[ i ];
			//	const uint length = asset.fileName.length();
			//	writer.writeUInt32( length );
			//	writer.writeData( asset.fileName.cStr(), length );
			//	writer.writeUInt32( asset.slot );
			//}

			writer.closeResource();

			closeResourceWriter( writer );

			material.dispose();
		}
		return true;
	}

}