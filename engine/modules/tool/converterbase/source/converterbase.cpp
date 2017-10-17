#include "tiki/converterbase/converterbase.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/converterbase/converter_manager.hpp"
#include "tiki/converterbase/resource_writer.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/path.hpp"

namespace tiki
{
	ConverterBase::ConverterBase()
	{
		m_pManager = nullptr;
	}

	ConverterBase::~ConverterBase()
	{
		TIKI_ASSERT( m_pManager == nullptr );
	}

	void ConverterBase::create( ConverterManager* pManager )
	{
		TIKI_ASSERT( m_pManager == nullptr );
		m_pManager = pManager;
		m_pManager->registerConverter( this );

		initializeConverter();
	}

	void ConverterBase::dispose()
	{
		TIKI_ASSERT( m_pManager != nullptr );

		disposeConverter();

		m_pManager->unregisterConverter( this );
		m_pManager = nullptr;
	}

	void ConverterBase::convert( ConversionResult& result, const ConversionAsset& asset ) const
	{
		TIKI_ASSERT( m_pManager != nullptr );

		if ( !startConversionJob( result, asset ) )
		{
			TIKI_TRACE_ERROR( "converter returns an error.\n" );
		}
	}

	void ConverterBase::openResourceWriter( ResourceWriter& writer, ConversionResult& result, const string& fileName, const string& extension ) const
	{
		TIKI_ASSERT( m_pManager != nullptr );

		const string realName = fileName + "." + extension;

		Path fullPath;
		fullPath.setCompletePath( m_pManager->getOutputPath().cStr() );
		fullPath.setFilenameWithExtension( realName.cStr() );
		result.addOutputFile( fullPath );

		writer.create( fullPath );
	}

	void ConverterBase::closeResourceWriter( ResourceWriter& writer ) const
	{
		writer.dispose();
	}

	TaskId ConverterBase::queueTask( TaskFunc pFunc, void* pData, TaskId dependingTaskId /*= InvalidTaskId */ ) const
	{
		TIKI_ASSERT( m_pManager != nullptr );

		return m_pManager->queueTask( pFunc, pData, dependingTaskId );
	}

	void ConverterBase::waitForTask( TaskId taskId ) const
	{
		TIKI_ASSERT( m_pManager != nullptr );

		m_pManager->waitForTask( taskId );
	}

	List< ResourceDefinition > ConverterBase::getResourceDefinitions( FlagMask8<ResourceDefinitionFeature> features ) const
	{
		const uint maxPlatformType	= features.isFlagSet( ResourceDefinitionFeature_Platform ) ? PlatformType_Count : PlatformType_Invalid + 1u;
		const uint maxGraphicsApi	= features.isFlagSet( ResourceDefinitionFeature_GraphicsApi ) ? GraphicsApi_Count : GraphicsApi_Invalid + 1u;
		const uint maxSoundApi		= features.isFlagSet( ResourceDefinitionFeature_SoundApi ) ? SoundApi_Count : SoundApi_Invalid + 1u;

		List< ResourceDefinition > definitions;
		for (uint platformIndex = 0u; platformIndex < maxPlatformType; ++platformIndex)
		{
			for (uint graphicsIndex = 0u; graphicsIndex < maxGraphicsApi; ++graphicsIndex)
			{
				for( uint soundIndex = 0u; soundIndex < maxSoundApi; ++soundIndex )
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
