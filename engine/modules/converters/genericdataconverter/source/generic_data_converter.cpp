#include "tiki/genericdataconverter/generic_data_converter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/converterbase/conversion_asset.hpp"
#include "tiki/converterbase/resource_writer.hpp"
#include "tiki/toolgenericdata/generic_data_document.hpp"
#include "tiki/toolgenericdata/generic_data_type_resource.hpp"
#include "tiki/toolproject/package.hpp"
#include "tiki/toolproject/project.hpp"

namespace tiki
{
	GenericDataConverter::GenericDataConverter()
	{
	}

	GenericDataConverter::~GenericDataConverter()
	{
	}

	void GenericDataConverter::setProject( const Project* pProject )
	{
		for( const Package& package : pProject->getPackages() )
		{
			m_collection.addPackage( package );
		}
	}

	uint32 GenericDataConverter::getConverterRevision( crc32 typeCrc ) const
	{
		const GenericDataType* pType = nullptr;
		if ( m_resourceTypeMap.findValue( &pType, typeCrc ) )
		{
			return pType->getTypeCrc();
		}

		return (uint32)-1;
	}

	bool GenericDataConverter::canConvertType( crc32 typeCrc ) const
	{
		if ( m_resourceTypeMap.hasKey( typeCrc ) )
		{
			return true;
		}

		return false;
	}

	void GenericDataConverter::getInputExtensions( List< string >& extensions ) const
	{
		extensions.pushBack( ".tikigenericobjects" );
	}

	crc32 GenericDataConverter::getOutputType() const
	{
		return crcString( "genericdata" );
	}

	bool GenericDataConverter::initializeConverter()
	{
		if ( !m_collection.create() )
		{
			TIKI_TRACE_ERROR( "[GenericDataConverter::initializeConverter] Unable to initialize Type collection.\n" );
			return false;
		}

		List< const GenericDataType* > resourceTypes;
		m_collection.findTypesByType( resourceTypes, GenericDataTypeType_Resource );

		for (uint i = 0u; i < resourceTypes.getCount(); ++i)
		{
			const GenericDataTypeResource* pResourceType = static_cast< const GenericDataTypeResource* >( resourceTypes[ i ] );
			if ( pResourceType->getBaseType() == nullptr )
			{
				continue;
			}

			const crc32 typeCrc = crcString( pResourceType->getPostFix().toLower() );
			m_resourceTypeMap.set( typeCrc, pResourceType );
		}

		return true;
	}

	void GenericDataConverter::disposeConverter()
	{
		m_resourceTypeMap.dispose();
		m_collection.dispose();
	}

	bool GenericDataConverter::startConversionJob( ConversionResult& result, const ConversionAsset& asset ) const
	{
		bool ok = true;

		GenericDataDocument document( *const_cast< GenericDataTypeCollection* >( &m_collection ) );
		if ( document.importFromFile( asset.inputFilePath.getCompletePath() ) )
		{
			const string& extension = document.getType()->getPostFix();

			ResourceWriter resourceWriter;
			openResourceWriter( resourceWriter, result, asset.assetName.cStr(), extension.cStr() );

			for( const ResourceDefinition& definition : getResourceDefinitions( FlagMask8< ResourceDefinitionFeature >() ) )
			{
				resourceWriter.openResource( asset.assetName + "." + extension, document.getType()->getFourCC(), definition, (uint16)document.getType()->getTypeCrc() );

				ResourceSectionWriter sectionWriter;
				resourceWriter.openDataSection( sectionWriter, SectionType_Main );

				ReferenceKey dataKey;
				const bool result = document.writeToResource( dataKey, sectionWriter );

				resourceWriter.closeDataSection( sectionWriter );

				if( result )
				{
					resourceWriter.openDataSection( sectionWriter, SectionType_Initializaion );
					sectionWriter.writeReference( &dataKey );
					resourceWriter.closeDataSection( sectionWriter );
				}
				else
				{
					TIKI_TRACE_ERROR( "[GenericDataConverter::startConversionJob] Unable to write resource.\n" );
					ok = false;
				}

				resourceWriter.closeResource();
			}

			closeResourceWriter( resourceWriter );
		}
		else
		{
			TIKI_TRACE_ERROR( "[GenericDataConverter::startConversionJob] Unable to load '%s'.\n", asset.inputFilePath.getCompletePath() );
			ok = false;
		}

		document.dispose();
		return ok;
	}
}