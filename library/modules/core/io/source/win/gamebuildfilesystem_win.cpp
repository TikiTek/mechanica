
#include "tiki/io/gamebuildfilesystem.hpp"

#include "tiki/base/file.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/string.hpp"

#include <windows.h>

namespace tiki
{
	void GamebuildFileSystem::create( cstring pGamebuildPath, uint maxStreamCount /*= 4u */ )
	{
		copyString( m_gamebuildPath, sizeof( m_gamebuildPath ), pGamebuildPath );

		m_fileStreams.create( maxStreamCount );
	}

	void GamebuildFileSystem::dispose()
	{
		m_fileStreams.dispose();
	}

	bool GamebuildFileSystem::exists( cstring pFileName ) const
	{
		const string fullPath = path::combine( m_gamebuildPath, pFileName );
		return GetFileAttributesA( fullPath.cStr() ) != INVALID_FILE_ATTRIBUTES;
	}

	DataStream* GamebuildFileSystem::open( cstring pFileName, DataAccessMode accessMode )
	{
		const string fullPath = path::combine( m_gamebuildPath, pFileName );

		for (uint i = 0u; i < m_fileStreams.getCount(); ++i)
		{
			FileStream& stream = m_fileStreams[ i ];

			if ( stream.isOpen() == false )
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