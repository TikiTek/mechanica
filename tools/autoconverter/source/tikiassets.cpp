
#include "tiki/tikiassets/tikiassets.hpp"

#include "tiki/base/iopath.hpp"
#include "tiki/toolbase/directory.hpp"

namespace tiki
{
	TikiAssets::TikiAssets()
	{
	}

	void TikiAssets::create( const string& sourcePath, const string& outputPath )
	{
		m_sourcePath	= sourcePath;

		m_manager.create( outputPath );

		//m_animationConverter.create( &m_manager );
		m_fontConverter.create( &m_manager );
		//m_materialConverter.create( &m_manager );
		m_modelConverter.create( &m_manager );
		//m_navmeshConverter.create( &m_manager );
		m_shaderConverter.create( &m_manager );
		m_textureConverter.create( &m_manager );

		TIKI_TRACE( "TikiAssets: started\n" );
	}

	void TikiAssets::dispose()
	{
		//m_animationConverter.dispose();
		m_fontConverter.dispose();
		//m_materialConverter.dispose();
		m_modelConverter.dispose();
		//m_navmeshConverter.dispose();
		m_shaderConverter.dispose();
		m_textureConverter.dispose();

		m_manager.dispose();

		TIKI_TRACE( "TikiAssets: finish\n" );
	}

	int TikiAssets::run()
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

	void TikiAssets::resolveDependencies()
	{

	}

	void TikiAssets::findFiles( const string& path,  List< string >& files, const string& ext ) const
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