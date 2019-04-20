#include "tiki/generic_data_converter/generic_data_converter.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/fourcc.hpp"
#include "tiki/converter_base/conversion_types.hpp"
#include "tiki/converter_base/resource_writer.hpp"
#include "tiki/tool_generic_data/generic_data_document.hpp"
#include "tiki/tool_generic_data/generic_data_type_resource.hpp"
#include "tiki/tool_project/package.hpp"
#include "tiki/tool_project/project.hpp"

namespace tiki
{
	static const char* s_pGenericDataTypeName = "generic_data";
	static const crc32 s_genericDataTypeCrc = crcString( s_pGenericDataTypeName );

	GenericDataConverter::GenericDataConverter()
	{
	}

	GenericDataConverter::~GenericDataConverter()
	{
	}

	bool GenericDataConverter::setProject( const Project* pProject )
	{
		for( const Package& package : pProject->getPackages() )
		{
			if( !m_collection.addPackage( package ) )
			{
				return false;
			}
		}

		return true;
	}

	uint32 GenericDataConverter::getConverterRevision( crc32 typeCrc ) const
	{
		return 1u;
		//const GenericDataType* pType = nullptr;
		//if ( m_resourceTypeMap.findValue( &pType, typeCrc ) )
		//{
		//	return pType->getTypeCrc();
		//}
		//
		//return (uint32)-1;
	}

	bool GenericDataConverter::canConvertType( crc32 typeCrc ) const
	{
		return typeCrc == s_genericDataTypeCrc;
		//if ( m_resourceTypeMap.hasKey( typeCrc ) )
		//{
		//	return true;
		//}
		//
		//return false;
	}

	void GenericDataConverter::getInputExtensions( List< string >& extensions ) const
	{
		extensions.pushBack( ".generic_data.disabled" );
	}

	const char* GenericDataConverter::getOutputType() const
	{
		return s_pGenericDataTypeName;
	}

	bool GenericDataConverter::initializeConverter()
	{
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
	}

	bool GenericDataConverter::startConversionJob( ConversionResult& result, const ConversionAsset& asset, const ConversionContext& context ) const
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