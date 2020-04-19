#include "tiki/generic_data_converter/generic_data_converter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/converter_base/conversion_types.hpp"
#include "tiki/converter_base/resource_writer.hpp"
#include "tiki/tool_generic_data/generic_data_document.hpp"
#include "tiki/tool_generic_data/generic_data_document_collection.hpp"
#include "tiki/tool_generic_data/generic_data_type_resource.hpp"
#include "tiki/tool_project/package.hpp"
#include "tiki/tool_project/project.hpp"

namespace tiki
{
	static const char* s_pGenericDataTypeName = "generic_data";

	GenericDataConverter::GenericDataConverter()
	{
	}

	GenericDataConverter::~GenericDataConverter()
	{
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
		return m_resourceTypeMap.hasKey( typeCrc );
	}

	void GenericDataConverter::getInputExtensions( List< DynamicString >& extensions ) const
	{
		extensions.pushBack( ".generic_data" );
	}

	const char* GenericDataConverter::getOutputType() const
	{
		return s_pGenericDataTypeName;
	}

	bool GenericDataConverter::initializeConverter( const ConversionContext& context )
	{
		for( const Package& package : context.pProject->getPackages() )
		{
			if( !m_typeCollection.addPackage( package ) )
			{
				return false;
			}
		}

		List< const GenericDataType* > resourceTypes;
		m_typeCollection.findTypesByType( resourceTypes, GenericDataTypeType_Resource );

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
	}

	bool GenericDataConverter::startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const
	{
		GenericDataDocumentCollection documentCollection( const_cast< GenericDataTypeCollection& >( m_typeCollection ) );
		documentCollection.create( context.pProject );

		GenericDataDocument* pDocument = documentCollection.loadDocument( asset.inputFilePath.getCompletePath() );
		if( pDocument == nullptr )
		{
			return false;
		}

		const GenericDataTypeResource* pResourceType = pDocument->getResourceType();
		if( pResourceType == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataConverter::startConversionJob] '%s' doesn't contain a resource.\n", asset.inputFilePath.getCompletePath() );
			return false;
		}

		const DynamicString& extension = pResourceType->getPostFix();

		ResourceWriter resourceWriter;
		openResourceWriter( resourceWriter, result, asset.assetName.cStr(), extension.cStr() );

		bool ok = true;
		for( const ResourceDefinition& definition : getResourceDefinitions( FlagMask8< ResourceDefinitionFeature >() ) )
		{
			resourceWriter.openResource( asset.assetName + "." + extension, pResourceType->getFourCC(), definition, (uint16)pResourceType->getTypeCrc() );

			ResourceSectionWriter sectionWriter;
			resourceWriter.openDataSection( sectionWriter, SectionType_Main );

			ReferenceKey dataKey;
			const bool result = pDocument->writeToResource( dataKey, sectionWriter );

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
		return ok;
	}
}