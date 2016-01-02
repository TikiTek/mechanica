#include "tiki/io/directory.hpp"

#include <Windows.h>

namespace tiki
{
	bool directory::create( const char* pPath )
	{
		return CreateDirectoryA( pPath, nullptr ) != 0;
	}

	bool directory::remove( const char* pPath )
	{
		return RemoveDirectoryA( pPath ) != 0;
	}

	bool directory::exists( const char* pPath )
	{
		DWORD att = GetFileAttributesA( pPath );
		return (att != INVALID_FILE_ATTRIBUTES && (att & FILE_ATTRIBUTE_DIRECTORY) != 0);
	}
}