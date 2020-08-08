#include "tiki/io/game_build_file_system.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/string.hpp"
#include "tiki/io/directory_iterator.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/path.hpp"

namespace tiki
{
	bool GamebuildFileSystem::create( const Path& assetBuildPath, uint maxStreamCount /* = 4u */ )
	{
		m_assetBuildPath = assetBuildPath;

		m_fileStreams.create( maxStreamCount );

		return rescan();
	}

	void GamebuildFileSystem::dispose()
	{
		disposeFiles();
		m_fileStreams.dispose();
	}

	bool GamebuildFileSystem::rescan()
	{
		DirectoryIterator iterator;
		if( !iterator.create( m_assetBuildPath.getCompletePath() ) )
		{
			return false;
		}

		disposeFiles();

		while( iterator.findNextFile() )
		{
			const char* pFilename = iterator.getCurrentFileName();
			const uint filenameSize = getStringSize( pFilename );

			GamebuildFile* pFile = (GamebuildFile*)TIKI_ALLOC( sizeof( GamebuildFile ) + filenameSize );
			*pFile = GamebuildFile();
			pFile->filenameCrc = crcString( pFilename );
			copyString( pFile->aFileName, filenameSize + 1, pFilename );

			m_files.push( pFile );
		}

		return true;
	}

	const char* GamebuildFileSystem::getFilenameByCrc( crc32 filenameCrc ) const
	{
		for ( const GamebuildFile& file : m_files )
		{
			if ( file.filenameCrc == filenameCrc )
			{
				return file.aFileName;
			}
		}

		return nullptr;
	}

	bool GamebuildFileSystem::exists( const char* pFileName ) const
	{
		const crc32 filenameCrc = crcString( pFileName );

		for ( const GamebuildFile& file : m_files )
		{
			if ( file.filenameCrc == filenameCrc )
			{
				return true;
			}
		}

		return false;
	}

	DataStream* GamebuildFileSystem::open( const char* pFileName, DataAccessMode accessMode )
	{
		Path fullPath = m_assetBuildPath;
		fullPath.push( pFileName );

		for (uint i = 0u; i < m_fileStreams.getCount(); ++i)
		{
			FileStream& stream = m_fileStreams[ i ];

			if ( !stream.isOpen() )
			{
				if ( stream.create( fullPath.getCompletePath(), accessMode ) )
				{
					return &stream;
				}

				break;
			}
		}

		return nullptr;
	}

	void GamebuildFileSystem::disposeFiles()
	{
		while( !m_files.isEmpty() )
		{
			GamebuildFile& file = *m_files.getBegin();
			m_files.removeSortedByValue( file );

			TIKI_FREE( &file );
		}
	}
}