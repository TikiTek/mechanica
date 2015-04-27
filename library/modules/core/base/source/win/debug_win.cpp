
#include "tiki/base/debug.hpp"

#include <crtdbg.h>
#include <stdio.h>
#include <windows.h>

namespace tiki
{
	void debug::nativeTrace( const char* pText )
	{
		OutputDebugStringA( pText );

		if ( !debug::isDebuggerActive() )
		{
			printf( pText );

			//FILE* pFile = fopen( "debug.log", "a" );
			//fwrite( pText, strlen( pText ), 1u, pFile );
			//fclose( pFile );
		}
	}

	bool debug::isDebuggerActive()
	{
		return IsDebuggerPresent() != 0u;
	}

	void debug::breakPoint()
	{
#if TIKI_ENABLED( TIKI_BUILD_MSVC )
		if ( isDebuggerActive() )
		{
			__debugbreak();
		}
		else
#endif
		{
			debug::nativeTrace( "Breakpoint has triggered.\n" );
			while (true);
		}
	}

	void debug::breakOnAlloc( int number )
	{
#if TIKI_ENABLED( TIKI_BUILD_MSVC )
		_CrtSetBreakAlloc( number );
#endif
	}

	void debug::dumpMemoryStats()
	{
#if TIKI_ENABLED( TIKI_BUILD_DEBUG ) && TIKI_ENABLED( TIKI_BUILD_MSVC )
		_CrtMemState memState;
		_CrtMemCheckpoint( &memState );

		debug::nativeTrace( "\nMemory Leaks:\n\n" );

		if ( memState.lCounts[ 1u ] == 0u )
		{
			debug::nativeTrace( "No Leaks!\n" );
		}
		else
		{
			_CrtDumpMemoryLeaks();
		}

		debug::nativeTrace( "\nMemory Stats:\n\n" );
		_CrtMemDumpStatistics( &memState );
		debug::nativeTrace( "\n" );
#endif
	}
}
