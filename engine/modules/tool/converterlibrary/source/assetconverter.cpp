#include "assetconverter.hpp"

#include "tiki/io/path.hpp"
#include "tiki/toolbase/directory_tool.hpp"

namespace tiki
{
	TIKI_ASSET_CONVERTER_METHOD IAssetConverter* createAssetConverter()
	{
		return TIKI_NEW( AssetConverter );
	}

	TIKI_ASSET_CONVERTER_METHOD void disposeAssetConverter( IAssetConverter* pObject )
	{
		TIKI_DELETE( pObject );
		Thread::shutdownSystem();
	}

	AssetConverter::AssetConverter()
	{
	}

	AssetConverter::~AssetConverter()
	{
	}

	bool AssetConverter::create( const AssetConverterParamter& parameters )
	{
		m_sourcePath = parameters.sourcePath;

		ConverterManagerParameter managerParameters;
		managerParameters.sourcePath		= parameters.sourcePath;
		managerParameters.outputPath		= parameters.outputPath;
		managerParameters.packageName		= parameters.packageName;
		managerParameters.pChangedFilesList	= &m_changedFiles;
		managerParameters.forceRebuild		= parameters.forceRebuild;

		if( !m_manager.create( managerParameters ) )
		{
			return false;
		}

		m_converterMutex.create();

		//m_navmeshConverter.create( &m_manager );

		TIKI_TRACE_INFO( "AssetConverter: started\n" );

		if ( (m_manager.isNewDatabase() && parameters.rebuildOnMissingDatabase) || parameters.waitForConversion )
		{
			if ( !convertAll() )
			{
				TIKI_TRACE_ERROR( "AssetConverter: Initial Asset conversion failed. Shutting down!\n" );

				return false;
			}
		}

		return true;
	}

	void AssetConverter::dispose()
	{
		m_manager.dispose();
		m_converterMutex.dispose();

		TIKI_TRACE_INFO( "AssetConverter: finish\n" );
	}

	bool AssetConverter::convertAll()
	{
		List< string > assetFiles;
		findFiles( m_sourcePath, assetFiles, ".xasset" );

		for (size_t i = 0u; i < assetFiles.getCount(); ++i)
		{
			m_manager.queueFile( assetFiles[ i ] );
		}
		TIKI_TRACE_INFO( "[AssetConverter] Complete scan finish!\n" );

		const bool result = m_manager.startConversion( &m_converterMutex );
		TIKI_TRACE_INFO( "[AssetConverter] Conversion %s!\n", result ? "successful" : "failed" );

		return result;
	}

	void AssetConverter::startWatch()
	{
		m_fileWatcher.create( path::getDirectoryName( m_sourcePath ).cStr(), 32u );
		m_watchThread.create( watchThreadStaticEntryPoint, this, 8192u, "AssetConverter" );
	}

	void AssetConverter::stopWatch()
	{
		if ( m_watchThread.isCreated() )
		{
			m_watchThread.requestExit();
			m_watchThread.waitForExit();
			m_watchThread.dispose();
		}

		m_fileWatcher.dispose();
	}

	bool AssetConverter::getChangedFiles( Array< string >& changedFiles )
	{
		MutexStackLock lock( m_converterMutex );
		if ( m_changedFiles.getCount() != 0u )
		{
			changedFiles.create( m_changedFiles.getBegin(), m_changedFiles.getCount() );
			m_changedFiles.clear();

			return true;
		}

		return false;
	}

	void AssetConverter::lockConversion()
	{
		m_converterMutex.lock();
	}

	void AssetConverter::unlockConversion()
	{
		m_converterMutex.unlock();
	}

	void AssetConverter::watchThreadEntryPoint( const Thread& thread )
	{
		convertAll();

		while ( thread.isExitRequested() == false )
		{
			FileWatcherEvent fileEvent;
			if ( m_fileWatcher.popEvent( fileEvent ) && fileEvent.eventType == FileWatcherEventType_Modified )
			{
				MutexStackLock lock( m_converterMutex );

				m_manager.queueFile( fileEvent.fileName );
				m_manager.startConversion();
			}
			else
			{
				Thread::sleepCurrentThread( 10u );
			}
		}
	}

	int AssetConverter::watchThreadStaticEntryPoint( const Thread& thread )
	{
		AssetConverter* pConverter = static_cast< AssetConverter* >( thread.getArgument() );
		pConverter->watchThreadEntryPoint( thread );
		return 0u;
	}
}