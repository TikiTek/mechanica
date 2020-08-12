
#include "tiki/io/path.hpp"

#include <windows.h>

namespace tiki
{
	string path::getCurrentDir()
	{
		char buffer[ 256u ];
		GetCurrentDirectoryA( TIKI_COUNT( buffer ), buffer );

		return buffer;
	}

	string path::getExecutablePath()
	{
		HMODULE currentModule = GetModuleHandleA( nullptr );

		char buffer[ 256u ];
		GetModuleFileNameA( currentModule, buffer, TIKI_COUNT( buffer ) );

		return path::getDirectoryName( buffer );
	}
}