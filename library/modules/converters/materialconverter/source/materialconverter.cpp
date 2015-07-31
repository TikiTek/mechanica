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

	uint16 MaterialConverter::getConverterRevision() const 
	{
		return 1u;
	}

	bool MaterialConverter::canConvertType( crc32 typeCrc ) const
	{
		static const crc32 s_typeCrc = crcString( "material" );
		return typeCrc == s_typeCrc;
	}

	crc32 MaterialConverter::getOutputType() const
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

	bool MaterialConverter::startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const
	{
		for( size_t i = 0u; i < parameters.inputFiles.getCount(); ++i )
		{
			const ConversionParameters::InputFile& file = parameters.inputFiles[ i ];
			
			ToolMaterial material;			
			if( !material.create( file.fileName ) )
			{
				material.dispose();
				continue;
			}

			ResourceWriter writer;
			openResourceWriter( writer, result, parameters.outputName, "material", PlatformType_Win );

			writer.openResource( parameters.outputName + ".material", TIKI_FOURCC( 'M', 'A', 'T', 'E' ), 1u );

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