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
			const ConversionParameters::InputFile& file = params.inputFiles[ i ];
			
			ToolMaterial material;			
			if( !material.create( file.fileName ) )
			{
				material.dispose();
				continue;
			}

			ResourceWriter writer;
			openResourceWriter( writer, params.outputName, "material", PlatformType_Win );

			writer.openResource( params.outputName + ".material", TIKI_FOURCC( 'M', 'A', 'T', 'E' ), 1u );

			const ReferenceKey materialDataKey = material.writeResource( writer );

			writer.openDataSection( 0u, AllocatorType_InitializaionMemory );
			writer.writeReference( &materialDataKey );
			writer.closeDataSection();

			writer.closeResource();

			closeResourceWriter( writer );

			material.dispose();
		}
		return true;
	}

}