#include "tiki/converterbase/converterbase.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/converterbase/resource_writer.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/path.hpp"

namespace tiki
{
	ConverterBase::ConverterBase()
	{
		m_pTaskSystem = nullptr;
	}

	ConverterBase::~ConverterBase()
	{
		TIKI_ASSERT( m_pTaskSystem == nullptr );
	}

	void ConverterBase::create( const Path& outputPath, TaskSystem* pTaskSystem )
	{
		TIKI_ASSERT( m_pTaskSystem == nullptr );

		m_outputPath	= outputPath;
		m_pTaskSystem	= pTaskSystem;

		initializeConverter();
	}

	void ConverterBase::dispose()
	{
		TIKI_ASSERT( m_pTaskSystem != nullptr );

		disposeConverter();

		m_pTaskSystem = nullptr;
		m_outputPath.clear();
	}

	void ConverterBase::convert( ConversionResult& result, const ConversionAsset& asset ) const
	{
		TIKI_ASSERT( m_pTaskSystem != nullptr );

		if ( !startConversionJob( result, asset ) )
		{
			TIKI_TRACE_ERROR( "converter returns an error.\n" );
		}
	}

	void ConverterBase::openResourceWriter( ResourceWriter& writer, ConversionResult& result, const char* pFileName, const char* pExtension ) const
	{
		TIKI_ASSERT( m_pTaskSystem != nullptr );

		Path fullPath = m_outputPath;
		fullPath.setFilename( pFileName );
		fullPath.setExtension( pExtension );
		result.addOutputFile( fullPath );

		writer.create( fullPath );
	}

	void ConverterBase::closeResourceWriter( ResourceWriter& writer ) const
	{
		writer.dispose();
	}

	TaskId ConverterBase::queueTask( TaskFunc pFunc, void* pData, TaskId dependingTaskId /*= InvalidTaskId */ ) const
	{
		TIKI_ASSERT( m_pTaskSystem != nullptr );
		return m_pTaskSystem->queueTask( pFunc, pData, dependingTaskId );
	}

	void ConverterBase::waitForTask( TaskId taskId ) const
	{
		TIKI_ASSERT( m_pTaskSystem != nullptr );
		m_pTaskSystem->waitForTask( taskId );
	}

	List< ResourceDefinition > ConverterBase::getResourceDefinitions( FlagMask8<ResourceDefinitionFeature> features ) const
	{
		const uint minPlatformType	= features.isFlagSet( ResourceDefinitionFeature_Platform ) ? 1u : 0u;
		const uint minGraphicsApi	= features.isFlagSet( ResourceDefinitionFeature_GraphicsApi ) ? 1u : 0u;
		const uint minSoundApi		= features.isFlagSet( ResourceDefinitionFeature_SoundApi ) ? 1u : 0u;
		const uint maxPlatformType	= features.isFlagSet( ResourceDefinitionFeature_Platform ) ? PlatformType_Count : PlatformType_Invalid + 1u;
		const uint maxGraphicsApi	= features.isFlagSet( ResourceDefinitionFeature_GraphicsApi ) ? GraphicsApi_Count : GraphicsApi_Invalid + 1u;
		const uint maxSoundApi		= features.isFlagSet( ResourceDefinitionFeature_SoundApi ) ? SoundApi_Count : SoundApi_Invalid + 1u;

		List< ResourceDefinition > definitions;
		for (uint platformIndex = minPlatformType; platformIndex < maxPlatformType; ++platformIndex)
		{
			for (uint graphicsIndex = minGraphicsApi; graphicsIndex < maxGraphicsApi; ++graphicsIndex)
			{
				for( uint soundIndex = minSoundApi; soundIndex < maxSoundApi; ++soundIndex )
				{
					ResourceDefinition& definition = definitions.add();
					definition.setPlatformType( (PlatformType)platformIndex );
					definition.setGraphicsApi( (GraphicsApi)graphicsIndex );
					definition.setSoundApi( (SoundApi)soundIndex );
				}
			}
		}

		return definitions;
	}
}
