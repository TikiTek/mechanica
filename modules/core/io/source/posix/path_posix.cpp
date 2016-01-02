#include "tiki/io/path.hpp"

#include <unistd.h>

namespace tiki
{
	string path::getCurrentDir()
	{
		char aBuffer[ TIKI_MAX_PATH ];
		getcwd( aBuffer, TIKI_COUNT( aBuffer ) );

		return aBuffer;
	}

	string path::getExecutablePath()
	{
		char aBuffer[ TIKI_MAX_PATH ];
		readlink( "/proc/self/exe", aBuffer, TIKI_COUNT( aBuffer ) );
		
		return path::getDirectoryName( aBuffer );
	}
}