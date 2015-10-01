
#include "tiki/io/directory.hpp"

#include <Windows.h>

namespace tiki
{
	namespace directory
	{
		bool create( const string& path )
		{
			return CreateDirectoryA( path.cStr(), nullptr ) != 0;
		}

		bool remove( const string& path )
		{
			return RemoveDirectoryA( path.cStr() ) != 0;
		}

		bool exists( const string& path )
		{
			DWORD att = GetFileAttributesA( path.cStr() );
			return ( att != INVALID_FILE_ATTRIBUTES && ( att & FILE_ATTRIBUTE_DIRECTORY ) != 0 );
		}
	}
}