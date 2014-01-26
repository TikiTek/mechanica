
#include "assetconverter.hpp"

#include "tiki/base/iopath.hpp"
#include "tiki/base/reflection.hpp"
#include "tiki/toolbase/tooldirectory.hpp"

namespace tiki
{
	IAssetConverter* createAssetConverter()
	{
		reflection::initialize();
		return TIKI_NEW AssetConverter();
	}

	void disposeAssetConverter( IAssetConverter* pObject )
	{
		TIKI_DEL pObject;
		reflection::shutdown();
	}

	AssetConverter::AssetConverter()
	{
	}

	void AssetConverter::create( const AssetConverterParamter& parameters )
	{
		m_sourcePath	= parameters.sourcePath;

		ConverterManagerParameter managerParameters;
		managerParameters.outputPath	= parameters.outputPath;
		managerParameters.forceRebuild	= parameters.forceRebuild;
		m_manager.create( managerParameters );

		//m_animationConverter.create( &m_manager );
		//m_navmeshConverter.create( &m_manager );
		m_fontConverter.create( &m_manager );
		m_materialConverter.create( &m_manager );
		m_modelConverter.create( &m_manager );
		m_shaderConverter.create( &m_manager );
		m_textureConverter.create( &m_manager );

		TIKI_TRACE( "AssetConverter: started\n" );
	}

	void AssetConverter::dispose()
	{
		//m_animationConverter.dispose();
		//m_navmeshConverter.dispose();
		m_fontConverter.dispose();
		m_materialConverter.dispose();
		m_modelConverter.dispose();
		m_shaderConverter.dispose();
		m_textureConverter.dispose();

		m_manager.dispose();

		TIKI_TRACE( "AssetConverter: finish\n" );
	}

	int AssetConverter::convertAll()
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

		return m_manager.startConversion();
	}

	void AssetConverter::startWatch()
	{
		m_fileWatcher.create( m_sourcePath.cStr(), 32u, true );
		m_converterMutex.create();
		
		if ( m_watchThread.create( watchThreadStaticEntryPoint, 8192u, "AssetConverter" ) == true )
		{
			m_watchThread.start( this );
		}
	}

	void AssetConverter::stopWatch()
	{
		m_watchThread.requestExit();
		m_watchThread.waitForExit();

		m_converterMutex.dispose();
		m_fileWatcher.dispose();
	}

	bool AssetConverter::getChangedFiles( Array< string >& changedFiles )
	{
		MutexStackLock lock( m_converterMutex );
		if ( m_changedFiles.getCount() != 0u )
		{
			changedFiles.create( m_changedFiles.getData(), m_changedFiles.getCount() );
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
		while ( thread.isExitRequested() == false )
		{
			FileWatcherEvent fileEvent;
			if ( m_fileWatcher.popEvent( fileEvent ) == true && fileEvent.eventType == FileWatcherEventType_Modified )
			{
				MutexStackLock lock( m_converterMutex );

				List< string > outputFiles;
				if ( m_manager.startConvertFile( fileEvent.fileName, outputFiles ) )
				{
					m_changedFiles.addRange( outputFiles.getData(), outputFiles.getCount() );
				}
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