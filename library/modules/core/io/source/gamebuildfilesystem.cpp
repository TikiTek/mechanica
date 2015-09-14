
#include "tiki/io/gamebuildfilesystem.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/base/file.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/string.hpp"
#include "tiki/io/directoryiterator.hpp"

namespace tiki
{
	bool GamebuildFileSystem::create( const char* pGamebuildPath, uint maxStreamCount /*= 4u */ )
	{
		copyString( m_gamebuildPath, sizeof( m_gamebuildPath ), pGamebuildPath );

		DirectoryIterator iterator;
		if ( !iterator.create( m_gamebuildPath ) )
		{
			return false;
		}

		while ( iterator.findNextFile() )
		{
			const char* pFilename = iterator.getCurrentFileName();
			const uint filenameSize = getStringSize( pFilename );

			GamebuildFile* pFile = (GamebuildFile*)TIKI_MEMORY_ALLOC( sizeof( GamebuildFile ) + filenameSize );
			*pFile = GamebuildFile();
			pFile->filenameCrc = crcString( pFilename );
			copyString( pFile->aFileName, filenameSize + 1, pFilename );

			m_files.push( pFile );
		}

		m_fileStreams.create( maxStreamCount );

		return true;
	}

	void GamebuildFileSystem::dispose()
	{
		while ( !m_files.isEmpty() )
		{
			GamebuildFile& file = *m_files.getBegin();
			m_files.removeSortedByValue( file );

			TIKI_MEMORY_FREE( &file );
		}

		m_fileStreams.dispose();
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

	bool GamebuildFileSystem::exists( cstring pFileName ) const
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

	DataStream* GamebuildFileSystem::open( cstring pFileName, DataAccessMode accessMode )
	{
		const string fullPath = path::combine( m_gamebuildPath, pFileName );

		for (uint i = 0u; i < m_fileStreams.getCount(); ++i)
		{
			FileStream& stream = m_fileStreams[ i ];

			if ( !stream.isOpen() )
			{
				if ( stream.open( fullPath.cStr(), accessMode ) )
				{
					return &stream;
				}

				break;
			}
		} 

		return nullptr;
	}
}