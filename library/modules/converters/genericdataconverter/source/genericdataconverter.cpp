#include "tiki/genericdataconverter/genericdataconverter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/genericdatadocument.hpp"

namespace tiki
{
	GenericDataConverter::GenericDataConverter()
	{
	}

	GenericDataConverter::~GenericDataConverter()
	{
	}

	uint16 GenericDataConverter::getConverterRevision() const 
	{
		return 1u;
	}

	crc32 GenericDataConverter::getInputType() const
	{
		return crcString( "genericdata" ); 
	}

	crc32 GenericDataConverter::getOutputType() const
	{
		return crcString( "genericdata" );
	}

	void GenericDataConverter::getDependingType( List< crc32 >& types ) const
	{
	}

	bool GenericDataConverter::initializeConverter()
	{
		return true;
	}

	void GenericDataConverter::disposeConverter()
	{
	}

	bool GenericDataConverter::startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const
	{
		for( size_t i = 0u; i < parameters.inputFiles.getCount(); ++i )
		{
			const ConversionParameters::InputFile& file = parameters.inputFiles[ i ];

			XmlReader reader;
			if ( !reader.create( file.fileName.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[GenericDataConverter::startConversionJob] Unable to parse '%s'.\n", file.fileName.cStr() );
				continue;
			}

			GenericDataDocument document;
			if ( !document.importFromXml( reader ) )
			{
				TIKI_TRACE_ERROR( "[GenericDataConverter::startConversionJob] Unable to load '%s'.\n", file.fileName.cStr() );
				continue;
			}

			ResourceWriter writer;
			openResourceWriter( writer, result, parameters.outputName, "material", PlatformType_Win );

			writer.openResource( parameters.outputName + ".material", TIKI_FOURCC( 'M', 'A', 'T', 'E' ), 1u );

			//const ReferenceKey materialDataKey = material.writeResource( writer );

			writer.openDataSection( 0u, AllocatorType_InitializaionMemory );
			//writer.writeReference( &materialDataKey );
			writer.closeDataSection();

			writer.closeResource();

			closeResourceWriter( writer );
		}
		return true;
	}
}