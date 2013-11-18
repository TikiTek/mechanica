
#include "tiki/io/gamebuildfilesystem.hpp"

#include "tiki/base/file.hpp"
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
		return GetFileAttributesA( pFileName ) != INVALID_FILE_ATTRIBUTES;
	}

	DataStream* GamebuildFileSystem::open( cstring pFileName, DataAccessMode accessMode )
	{
		for (uint i = 0u; i < m_fileStreams.getCount(); ++i)
		{
			FileStream& stream = m_fileStreams[ i ];

			if ( stream.isOpen() == false )
			{
				if ( stream.open( pFileName, accessMode ) )
				{
					return &stream;
				}
				break;
			}
		} 

		return nullptr;
	}
}