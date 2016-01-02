
#include "tiki/base/debug.hpp"

#include <string.h>
#include <stdio.h>

namespace tiki
{
	void debug::nativeTrace( const char* pText )
	{
		printf( pText );

		if ( !debug::isDebuggerActive() )
		{
			FILE* pFile = fopen( "debug.log", "a" );
			fwrite( pText, strlen( pText ), 1u, pFile );
			fclose( pFile );
		}
	}

	bool debug::isDebuggerActive()
	{
		return true;
	}

	void debug::breakPoint()
	{
		if ( isDebuggerActive() )
		{
			__builtin_trap();
		}
		else
		{
			nativeTrace( "Breakpoint has triggered.\n" );
			while (true);
		}
	}

	void debug::breakOnAlloc( int number )
	{
		/*_CrtSetBreakAlloc( number );*/
	}

	void debug::dumpMemoryStats()
	{
	}
}
