
#include "tiki/io/gamebuildfilesystem.hpp"

#include "tiki/base/file.hpp"
#include "tiki/base/string.hpp"

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
		return false;
	}

	DataStream* GamebuildFileSystem::open( cstring pFileName, DataAccessMode accessMode )
	{
		return nullptr;
	}
}