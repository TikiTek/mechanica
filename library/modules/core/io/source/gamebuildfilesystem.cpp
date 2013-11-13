
#include "tiki/io/gamebuildfilesystem.hpp"

namespace tiki
{
	bool GamebuildFileSystem::exists( cstring pFileName ) const
	{
		return false;
	}

	DataStream* GamebuildFileSystem::open( cstring pFileName, DataAccessMode accessMode )
	{
		return nullptr;
	}
}