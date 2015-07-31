#include "tiki/genericdataconverter/genericdataconverter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/converterbase/conversionparameters.hpp"
#include "tiki/converterbase/convertermanager.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
#include "tiki/io/xmlreader.hpp"
#include "tiki/toolgenericdata/genericdatadocument.hpp"
#include "tiki/toolgenericdata/genericdatatyperesource.hpp"

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

	bool GenericDataConverter::canConvertType( crc32 typeCrc ) const
	{
		return m_resourceTypeMap.hasKey( typeCrc );
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
		if ( !m_collection.create( getManager()->getSourcePath(), true ) )
		{
			TIKI_TRACE_ERROR( "[GenericDataConverter::initializeConverter] Unable to initialize Type collection.\n" );
			return false;
		}

		List< const GenericDataType* > resourceTypes;
		m_collection.findTypesByType( resourceTypes, GenericDataTypeType_Resource );

		for (uint i = 0u; i < resourceTypes.getCount(); ++i)
		{
			const GenericDataTypeResource* pResourceType = static_cast< const GenericDataTypeResource* >( resourceTypes[ i ] );

			const crc32 typeCrc = crcString( pResourceType->getPostFix() );
			m_resourceTypeMap.set( typeCrc, pResourceType );
		}

		return true;
	}

	void GenericDataConverter::disposeConverter()
	{
		m_resourceTypeMap.dispose();
		m_collection.dispose();
	}

	bool GenericDataConverter::startConversionJob( ConversionResult& result, const ConversionParameters& parameters ) const
	{
		bool ok = true;
		for( size_t i = 0u; i < parameters.inputFiles.getCount(); ++i )
		{
			const ConversionParameters::InputFile& file = parameters.inputFiles[ i ];

			XmlReader reader;
			if ( !reader.create( file.fileName.cStr() ) )
			{
				TIKI_TRACE_ERROR( "[GenericDataConverter::startConversionJob] Unable to parse '%s'.\n", file.fileName.cStr() );
				continue;
			}

			GenericDataDocument document( *const_cast< GenericDataTypeCollection* >( &m_collection ) );
			if ( !document.importFromXml( reader ) )
			{
				TIKI_TRACE_ERROR( "[GenericDataConverter::startConversionJob] Unable to load '%s'.\n", file.fileName.cStr() );
				ok = false;
				continue;
			}

			const string& extension = document.getType()->getPostFix();

			ResourceWriter writer;
			openResourceWriter( writer, result, parameters.outputName, extension, PlatformType_Win );

			writer.openResource( parameters.outputName + "." + extension, document.getType()->getFourCC(), 1u );

			ReferenceKey dataKey;
			if ( !document.writeToResource( dataKey, writer ) )
			{
				TIKI_TRACE_ERROR( "[GenericDataConverter::startConversionJob] Unable to write resource.\n" );
				ok = false;
				continue;
			}

			writer.openDataSection( 0u, AllocatorType_InitializaionMemory );
			writer.writeReference( &dataKey );
			writer.closeDataSection();

			writer.closeResource();

			closeResourceWriter( writer );
		}

		return ok;
	}
}