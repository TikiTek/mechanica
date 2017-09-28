#include "tiki/converterbase/converterbase.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/converterbase/converter_manager.hpp"
#include "tiki/converterbase/resourcewriter.hpp"
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
		const string fullPath = path::getAbsolutePath( path::combine( m_pManager->getOutputPath(), realName ) );

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

	List< ResourceDefinition > ConverterBase::getResourceDefinitions() const
	{
		List< ResourceDefinition > definitions;

		//for (uint platformIndex = 0u; platformIndex < PlatformType_Count; ++platformIndex)
		//{
		//	for (uint graphicsIndex = 0u; graphicsIndex < GraphicsApi_Count; ++graphicsIndex)
		//	{
		//		ResourceDefinition& definition = definitions.add();
		//		definition.setPlatformType( (PlatformType)platformIndex );
		//		definition.setGraphicsApi( (GraphicsApi)graphicsIndex );
		//	}
		//}

		ResourceDefinition& definition = definitions.add();
		definition.setPlatformType( PlatformType_Win );
		definition.setGraphicsApi( GraphicsApi_D3D11 );

		return definitions;
	}
}
