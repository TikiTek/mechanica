
#include "assetconverter.hpp"

#include "tiki/io/path.hpp"
#include "tiki/toolbase/directory_tool.hpp"

namespace tiki
{
	IAssetConverter* createAssetConverter()
	{
		return TIKI_NEW( AssetConverter );
	}

	void disposeAssetConverter( IAssetConverter* pObject )
	{
		TIKI_MEMORY_DELETE_OBJECT( pObject );
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
		m_sourcePath	= parameters.sourcePath;

		ConverterManagerParameter managerParameters;
		managerParameters.sourcePath		= parameters.sourcePath;
		managerParameters.outputPath		= parameters.outputPath;
		managerParameters.pChangedFilesList	= &m_changedFiles;
		managerParameters.forceRebuild		= parameters.forceRebuild;
		m_converterMutex.create();
		m_manager.create( managerParameters );

		//m_navmeshConverter.create( &m_manager );
		m_animationConverter.create( &m_manager );
		m_fontConverter.create( &m_manager );
		m_genericDataConverter.create( &m_manager );
		m_modelConverter.create( &m_manager );
		m_shaderConverter.create( &m_manager );
		m_textureConverter.create( &m_manager );
		
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
		//m_navmeshConverter.dispose();
		m_animationConverter.dispose();
		m_fontConverter.dispose();
		m_genericDataConverter.dispose();
		m_modelConverter.dispose();
		m_shaderConverter.dispose();
		m_textureConverter.dispose();

		m_manager.dispose();
		m_converterMutex.dispose();

		TIKI_TRACE_INFO( "AssetConverter: finish\n" );
	}

	bool AssetConverter::convertAll()
	{
		List< string > assetFiles;
		List< string > templateFiles;
		findFiles( m_sourcePath, assetFiles, ".xasset" );
		findFiles( m_sourcePath, templateFiles, ".xtemplate" );

		for (size_t i = 0u; i < templateFiles.getCount(); ++i)
		{
			m_manager.addTemplate( templateFiles[ i ] );
		}

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

	void AssetConverter::findFiles( const string& path,  List< string >& files, const string& ext ) const
	{
		List< string > dirFiles;
		directory::getFiles( path, dirFiles );
		for (size_t i = 0u; i < dirFiles.getCount(); ++i)
		{
			if ( path::getExtension( dirFiles[ i ] ) != ext )
			{
				continue;
			}

			files.add( path::combine( path, dirFiles[ i ] ) );
		}		

		List< string > dirDirectories;
		directory::getDirectories( path, dirDirectories );
		for (size_t i = 0u; i < dirDirectories.getCount(); ++i)
		{
			findFiles( path::combine( path, dirDirectories[ i ] ), files, ext );
		}		
	}
}